import { css, html, LitElement, unsafeCSS } from "lit";

import crc32 from 'crc-32';
import WaveformData from 'waveform-data';
import * as d3 from 'd3';

import "./icon-button.js";
import { midiManager } from './midi-manager.js';
import { AlertsMixin } from "./mixin-alerts-pub.js";
import { SettingsMixin } from "./mixin-settings.js";


class VDJWaveform extends AlertsMixin(SettingsMixin(LitElement)) {
  render() {
    return html`
      <div class="waveform-container" @click="${this.handleClick}">
        <div class="deck-label">Deck ${this.deck}</div>
        <div class="vocal stem-label">vocal</div>
        <div class="hihat stem-label">hihat</div>
        <div class="bass stem-label">bass</div>
        <div class="instruments stem-label">instruments</div>
        <div class="kick stem-label">kick</div>
        ${this.trackLoaded ? html`
          <div id="waveform-wrapper" class="waveform-wrapper ${this.isPlaying ? 'playing' : 'paused'}">
            <div id="waveform-canvas-1" class="waveform loaded"></div>
            <div id="waveform-canvas-2" class="waveform loaded"></div>
            <div id="waveform-canvas-3" class="waveform loaded"></div>
            <div id="waveform-canvas-4" class="waveform loaded"></div>
            <div id="waveform-canvas-5" class="waveform loaded"></div>
          </div>
          <div class="progress-indicator"></div>
          <div class="time-display">
            ${this.formatTime(this.displayTime)} / ${this.formatTime(this.duration)}
          </div>
        ` : html`
          <div class="no-track">
            No track loaded on Deck ${this.deck}
          </div>
        `}
      </div>
    `;
  }

  static get properties() {
    return {
      deck: { type: String },
      displayTime: { type: Number },
      duration: { type: Number },
      // isPlaying: { type: Boolean },
      trackLoaded: { type: Boolean },
      // bpm: { type: Number },
      // absoluteBpm: { type: Number },
      // transitionDuration: { type: Number },
      // animationDuration: { type: Number }
    };
  }

  static waveformHeight = 22;

  constructor() {
    super();
    this.fs = require('fs/promises');
    this.os = require('os');
    this.path = require('path');
    const util = require('util');
    this.exec = util.promisify(require('child_process').exec);
    this.displayTime = 0;
    this.duration = 0;
    this.isPlaying = false;
    this.trackLoaded = false;
    this.updateInterval = null;
    this.displayTimeInterval = null;
    this.stemNames = ['vocal', 'hihat', 'bass', 'instruments', 'kick'];
    this.loadDelay = 200;
    this.currentFilepath = undefined;

    // Settings for generated waveforms
    this.audioScale = 1024;
    this.audioAmplitudeScale = 2.1;

    // BPM and smooth animation properties
    this.bpm = 0;
    this.absoluteBpm = 0;
    this.playbackRate = 1;
    this.animationDuration = 2; // Duration for continuous movement when playing
    this.lastSyncTime = 0;
    this.lastSyncTimestamp = 0;
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('enable-stem-waveforms', e => {
      if (this.settings.getBool('virtualdj.active') && e.detail) {
        this.startUpdating();
        this.registerMidiCallbacks();
      } else {
        this.stopUpdating();
        this.stopDisplayTimeUpdate();
      }
    });
  }

  disconnectedCallback() {
    super.disconnectedCallback();
    this.stopUpdating();
    this.stopDisplayTimeUpdate();
    window.removeEventListener('enable-stem-waveforms');
  }

  updatePlaybackRate() {
    if (this.absoluteBpm > 0 && this.bpm > 0) {
      this.playbackRate = this.bpm / this.absoluteBpm;
      // Update animation duration based on playback rate
      this.animationDuration = 2 / this.playbackRate; // Adjust base duration by playback rate
      console.log(`Playback rate: ${this.playbackRate} (BPM: ${this.bpm}/${this.absoluteBpm})`);
      // this.requestUpdate();
    }
  }

  startDisplayTimeUpdate() {
    this.stopDisplayTimeUpdate();
    this.displayTimeInterval = setInterval(() => {
      const now = Date.now();
      const elapsedMs = now - this.lastSyncTimestamp;
      const expectedProgress = elapsedMs * this.playbackRate;
      this.displayTime = Math.min(this.lastSyncTime + expectedProgress, this.duration);
    }, 250);  // 50ms for smooth. 250ms for more or less.
  }

  stopDisplayTimeUpdate() {
    if (this.displayTimeInterval) {
      clearInterval(this.displayTimeInterval);
      this.displayTimeInterval = null;
    }
  }

  async updateBpmData() {
    try {
      const bpmData = await window.vdj.query(`deck ${this.deck} get_bpm`);
      console.log('bpm: ' + bpmData);
      this.bpm = parseFloat(bpmData) || 0;
      this.updatePlaybackRate();
      const data = await window.vdj.query(`deck ${this.deck} get_bpm absolute`);
      console.log('absolute-bpm: ' + data);
      this.absoluteBpm = parseFloat(data) || 0;
      this.updatePlaybackRate();
    } catch (error) {
      console.warn('Failed to get BPM data:', error);
    }
  }

  registerMidiCallbacks() {
    const load = () => {
      this.updateWaveform();
      this.updatePlaybackInfo();
      this.updateBpmData();
    };
    // TODO: update from drag&drop and button clicks
    midiManager.registerInput(
        this.settings.getString('midi.loadLeft'),
        e => setTimeout(this.updateWaveform.bind(this), this.loadDelay)
    );
    midiManager.registerInput(
        this.settings.getString('midi.loadRight'),
        e => setTimeout(this.updateWaveform.bind(this), this.loadDelay)
    );
    // midiManager.registerInput(
    //     this.settings.getString('virtualdj.midi.playPause'),
    //     e => setTimeout(load, this.loadDelay)
    // );
  }

  startUpdating() {
    this.updateInterval = setInterval(() => {
      this.updateWaveform();
      this.updatePlaybackInfo();
      this.updateBpmData(); // Update BPM data periodically
    }, 15000);  // Sync frequency
  }

  stopUpdating() {
    if (this.updateInterval) {
      clearInterval(this.updateInterval);
      this.updateInterval = null;
    }
  }

  async updateWaveform() {
    try {
      // get filepath for deck
      const data = await window.vdj.query(`deck ${this.deck} get_filepath`);
      if (!data || data.startsWith('error')) {
        this.trackLoaded = false;
        return;
      }

      // only update waveform if the song has changed
      if (data === this.currentFilepath) return;
      console.log(`deck ${this.deck} loaded new song ${data}`);
      this.trackLoaded = true;
      this.currentFilepath = data;

      // get duration of the song
      const durationData = await window.vdj.query(`deck ${this.deck} get_time total`);
      this.duration = parseInt(durationData) || 0;

      // generate the waveform
      await this.processStems(this.currentFilepath);

      // create the animation object using the Web Animations API
      const w = this.shadowRoot.getElementById('waveform-wrapper');
      this.animation = w.animate(
        [
          { transform: 'translateX(0)' },    // Keyframe 1: Start at 0
          { transform: `translateX(-${this.waveformWidth}px)` }  // Keyframe 2: End at a negative width set from renderWaveform
        ],
        {
          duration: this.duration,  // Duration in milliseconds
          fill: 'forwards',  // Keeps the final state after the animation ends
          easing: 'linear',  // Ensures a constant speed
        }
      );
      this.animation.pause();
    } catch (error) {
      console.warn('Failed to get get filepath and update the waveform. ', error);
    }
  }

  async updatePlaybackInfo() {
    if (!this.trackLoaded) return;
    try {
      // get time of the track and update the animation to the time
      const timeData = await window.vdj.query(`deck ${this.deck} get_time elapsed`);
      const newTime = parseInt(timeData) || 0;
      this.displayTime = newTime;
      this.lastSyncTime = newTime;
      this.lastSyncTimestamp = Date.now();
      this.animation.currentTime = newTime;

      // check if track is playing
      const data = await window.vdj.query(`deck ${this.deck} play`);
      const wasPlaying = this.isPlaying;
      this.isPlaying = data === 'yes';
      if (this.isPlaying !== wasPlaying) {
        if (this.isPlaying) {
          this.animation.play();
          this.startDisplayTimeUpdate();
        } else {
          this.animation.pause();
          this.stopDisplayTimeUpdate();
        }
      }
    } catch (error) {
      console.warn('Failed to update playback info:', error);
    }
  }

  formatTime(ms) {
    const seconds = Math.floor(ms / 1000);
    const minutes = Math.floor(seconds / 60);
    const remainingSeconds = seconds % 60;
    return `${minutes}:${remainingSeconds.toString().padStart(2, '0')}`;
  }

  getStemColor(index) {
    const colors = ['#e91e63', '#ff9800', '#4caf50', '#2196f3', '#9c27b0'];
    return colors[index] || '#64b5f6';
  }

  getStemProgressColor(index) {
    const colors = ['#ad1457', '#f57c00', '#388e3c', '#1976d2', '#7b1fa2'];
    return colors[index] || '#1976d2';
  }

  getDirChecksumAndSuffix(directoryPath) {
    // Remove separators before getting the last 4 characters.
    const dirName = directoryPath.replace(/\\|\//g, '');
    const last4 = dirName.slice(-4);

    // Compute the CRC32 checksum as a 32-bit integer, then format as a hex string.
    const csum = crc32.buf(Buffer.from(directoryPath, 'utf-8'));
    const chksum = (csum >>> 0).toString(16).padStart(8, '0');

    return `User..${last4}-${chksum}/`;
  }

  async processStems(filepath) {
    if (!filepath) return;
    try {
      const originalSubpath = filepath.replace(this.settings.getString('dir.music'), '');

      // Load waveform from the cache if it exists
      let exists = true;
      try {
        for (let i = 1; i <= 5; i++) {
          const cacheFile = this.getWaveformCacheFilepath(originalSubpath, i);
          exists &= await ipcRenderer.invoke('existsfilepath', cacheFile);
          if (!exists) break;
        }
      } catch (error) {
        console.warn('Error checking waveform cache: ', error);
        exists = false;
      }
      if (exists) {
        await this.loadCachedWaveforms(originalSubpath);
        return;
      }

      // Unpack stem file
      const fullFilepath = this.settings.getString('dir.vdjStems') + this.getDirChecksumAndSuffix(this.path.dirname(filepath)) + this.path.basename(filepath) + '.vdjstems';
      const tmpPath = await this.fs.mkdtemp(this.path.join(this.os.tmpdir(), 'ss-stems-'));
      await this.exec(`yes y | ffmpeg -i "${fullFilepath}" -map 0:a:0 -c copy ${tmpPath}/1.m4a -map 0:a:1 -c copy ${tmpPath}/2.m4a -map 0:a:2 -c copy ${tmpPath}/3.m4a -map 0:a:3 -c copy ${tmpPath}/4.m4a -map 0:a:4 -c copy ${tmpPath}/5.m4a`);

      // Write out each stem separately
      for (let i = 1; i <= 5; i++) {
        const audioContext = new AudioContext();
        const response = await fetch(`file://${tmpPath}/${i}.m4a`)
        const buffer = await response.arrayBuffer();
        const options = {
          audio_context: audioContext,
          array_buffer: buffer,
          scale: this.audioScale,
          amplitude_scale: this.audioAmplitudeScale,
          split_channels: false
        };

        // Use waveform-data to parse the audio buffer
        const waveform = await new Promise((resolve, reject) => {
          WaveformData.createFromAudio(options, (err, waveform_arg) => {
            if (err) {
              return reject(err);
            }
            resolve(waveform_arg);
          });
        });
        console.log(`Waveform has ${waveform.length} points (length)`);

        // Save waveform data to cache
        await this.saveWaveformCache(originalSubpath, i, waveform.toArrayBuffer());

        // Render the waveform
        this.renderWaveform(waveform, i);
      }

      // Clean up temp directory
      await this.exec(`rm -rf "${tmpPath}"`);

    } catch(err) {
      this.addAlert('WaveformData failed creating waveform for ' + filepath + ' : ' + err, 8);
      throw err;
    }
  }

  async loadCachedWaveforms(originalSubpath) {
    try {
      for (let i = 1; i <= 5; i++) {
        const cacheFile = this.getWaveformCacheFilepath(originalSubpath, i);
        const waveformData = await this.fs.readFile(cacheFile);
        const waveform = WaveformData.create(waveformData.buffer);
        this.renderWaveform(waveform, i);
      }
    } catch (error) {
      console.warn('Error loading cached waveforms: ', error);
      // TODO: If cache loading fails, fall back to regeneration
      throw error;
    }
  }

  async saveWaveformCache(originalSubpath, stemIndex, waveformData) {
    try {
      // Save waveform data as arraybuffer
      const cacheFile = this.getWaveformCacheFilepath(originalSubpath, stemIndex);
      await this.fs.writeFile(cacheFile, Buffer.from(waveformData));
      console.log(`Saved waveform cache: ${cacheFile}`);
    } catch (error) {
      console.warn('Error saving waveform cache: ', error);
    }
  }

  renderWaveform(waveform, stemIndex) {
    const channel = waveform.channel(0);
    const container = d3.select(this.shadowRoot.getElementById(`waveform-canvas-${stemIndex}`));

    // Clear any existing content
    container.selectAll('*').remove();

    const x = d3.scaleLinear();
    const y = d3.scaleLinear();

    const min = channel.min_array();
    const max = channel.max_array();

    // Set the desired width and height for the SVG container
    const pixelBeatDistance = 115;  // 115
    this.waveformWidth = this.duration * this.bpm / 60 / 1000 * pixelBeatDistance;
    const verticalOffset = VDJWaveform.waveformHeight / 2; // Center the waveform vertically

    x.domain([0, waveform.length]).rangeRound([0, this.waveformWidth]);
    y.domain([d3.min(min), d3.max(max)]).rangeRound([verticalOffset, -verticalOffset]);

    const area = d3.area()
      .x((d, i) => x(i))
      .y0((d, i) => y(min[i]))
      .y1((d, i) => y(d));

    const graph = container.append('svg')
      .style('width', `${this.waveformWidth}px`)
      .style('height', `${VDJWaveform.waveformHeight}px}`)
      .datum(max)
      .append('path')
      .attr('transform', () => `translate(0, ${verticalOffset})`)
      .attr('d', area)
      .attr('stroke', this.getStemColor(stemIndex - 1));
  }

  getWaveformCacheFilepath(songFilepath, stemIndex) {
    const cacheDir = this.settings.getString('dir.vdjStemWaveforms');
    return this.path.join(cacheDir, songFilepath.replace(/\.[^.]+$/, `_stem${stemIndex}_scale${this.audioScale}_amp${this.audioAmplitudeScale}_wf.data`));
  }

  static get styles() {
    return [
      css`
        :host {
          display: flex;
          flex-direction: column;
          width: 100%;
          height: 320px;
          position: relative;
          background: #000;
          border-top: 1px solid #333;
          border-radius: 4px;
          overflow: hidden;
        }
        .outside {
          position: absolute;
          top: -3000px;
          left: -3000px;
        }
        .waveform-container {
          width: 100%;
          height: 100%;
          overflow: hidden;
          position: relative;
          background: linear-gradient(90deg, #1a1a2e, #16213e, #1a1a2e);
        }
        .waveform-wrapper {
          left: 25%;
          width: 100%;
          height: 100%;
          position: relative;
          animation: scroll-waveform linear; /* Name the animation and set a linear timing function */
          animation-play-state: paused;  // Start the animation paused
        }

        .waveform-wrapper.paused {
          transition: transform var(--transition-duration, 0.1)s ease-out;
        }
        .waveform-wrapper.playing {
          transition: transform var(--transition-duration, 2)s linear;
        }
        .waveform {
          width: 100%;
          height: ${VDJWaveform.waveformHeight}px;
          /* background: linear-gradient(45deg, #00d4ff, #ff0080, #00ff88);
          mask-image: var(--waveform-mask);
          mask-size: cover;
          mask-position: center;
          mask-repeat: no-repeat;
          opacity: 0.8;
          transition: opacity 0.3s ease; */
        }
        .waveform.loaded {
          opacity: 1;
        }
        .progress-indicator {
          position: absolute;
          top: 0;
          left: 25%;
          width: 2px;
          height: 100%;
          background: #fff;
          box-shadow: 0 0 10px rgba(255, 255, 255, 0.8);
          transform: translateX(-50%);
          z-index: 10;
        }
        .time-display {
          position: absolute;
          top: 5px;
          right: 10px;
          font-family: monospace;
          font-size: 12px;
          color: #fff;
          background: rgba(0, 0, 0, 0.5);
          padding: 2px 6px;
          border-radius: 3px;
          z-index: 10;
        }
        .no-track {
          position: absolute;
          top: 50%;
          left: 50%;
          transform: translate(-50%, -50%);
          color: #666;
          font-size: 14px;
          text-align: center;
        }
        .deck-label {
          position: absolute;
          top: 5px;
          left: 10px;
          font-size: 12px;
          color: #999;
          background: rgba(0, 0, 0, 0.5);
          padding: 2px 6px;
          border-radius: 3px;
          z-index: 10;
        }
        .stem-label {
          position: absolute;
          top: 5px;
          right: calc(100% - 25% + 10px);
          font-size: 12px;
          color: #999;
          background: rgba(0, 0, 0, 0.5);
          padding: 2px 6px;
          border-radius: 3px;
          z-index: 10;
        }
        .vocal {
          top: 2px;
        }
        .hihat {
          top: 24px;
        }
        .bass {
          top: 46px;
        }
        .instruments {
          top: 68px;
        }
        .kick {
          top: 90px;
        }
      `,
    ];
  }
}

window.customElements.define('vdj-waveform', VDJWaveform);
