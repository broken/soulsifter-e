import { css, html, LitElement, unsafeCSS } from "lit";

import crc32 from 'crc-32';
import WaveformData from 'waveform-data';
import * as d3 from 'd3';

import "./icon-button.js";
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
      // trackLoaded: { type: Boolean },
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
    this.audioScale = 1024;

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
    if (this.settings.getBool('virtualdj.active'))
      this.style.setProperty('--vdj-waveform-display', 'flex');
    else
      this.style.setProperty('--vdj-waveform-display', 'none');
  }

  disconnectedCallback() {
    super.disconnectedCallback();
    this.stopUpdating();
    this.stopDisplayTimeUpdate();
  }

  firstUpdated() {
    this.startUpdating();
  }

  updateTimeSync(newTime) {
    this.displayTime = newTime;
    this.lastSyncTime = newTime;
    this.lastSyncTimestamp = Date.now();
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

  handlePlayStateChange() {
    if (this.isPlaying) {
      this.startDisplayTimeUpdate();
    } else {
      this.stopDisplayTimeUpdate();
    }
  }

  startDisplayTimeUpdate() {
    this.stopDisplayTimeUpdate();
    this.displayTimeInterval = setInterval(() => {
      const now = Date.now();
      const elapsedMs = now - this.lastSyncTimestamp;
      const expectedProgress = elapsedMs * this.playbackRate;
      this.displayTime = Math.min(this.lastSyncTime + expectedProgress, this.duration);
    }, 250); // 50ms for smooth. 250ms for more or less.
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

  startUpdating() {
    this.updateWaveform();
    this.updateInterval = setInterval(() => {
      this.updatePlaybackInfo();
      this.updateBpmData(); // Update BPM data periodically
    }, 5000);  // Sync frequency
  }

  stopUpdating() {
    if (this.updateInterval) {
      clearInterval(this.updateInterval);
      this.updateInterval = null;
    }
  }

  async updateWaveform() {
    try {
      const data = await window.vdj.query(`deck ${this.deck} get_filepath`);
      console.log('data: ' + data);
      if (data) {
        if (data !== this.currentFilepath) {
          this.currentFilepath = data;
          this.generateWaveformPattern();
          const durationData = await window.vdj.query(`deck ${this.deck} get_time total `);
          console.log('duration: ' + durationData);
          this.duration = parseInt(durationData) || 0;
          this.trackLoaded = true;
          // Get BPM data when track loads
          this.updateBpmData();

          setTimeout(() => {

            // 1. Create the animation object using the Web Animations API
            const w = this.shadowRoot.getElementById('waveform-wrapper');
            this.animation = w.animate(
              [
                { transform: 'translateX(0)' }, // Keyframe 1: Start at 0
                { transform: `translateX(-${this.waveformWidth}px)` } // Keyframe 2: End at a negative width
              ],
              {
                duration: this.duration, // Duration in milliseconds
                fill: 'forwards', // Keeps the final state after the animation ends
                easing: 'linear', // Ensures a constant speed
              }
            );
            this.animation.pause();
          }, 8000);
        }
    } else {
      this.trackLoaded = false;
    }
    } catch (error) {
      console.warn('Failed to get get filepath:', error);
    }
  }

  async updatePlaybackInfo() {
    try {
      const timeData = await window.vdj.query(`deck ${this.deck} get_time elapsed`);
      console.log('time: ' + timeData);
      const newTime = parseInt(timeData) || 0;
      this.animation.currentTime = newTime;
      this.updateTimeSync(newTime);
      const data = await window.vdj.query(`deck ${this.deck} play`);
      console.log('playing: ' + data);
      const wasPlaying = this.isPlaying;
      this.isPlaying = data === 'yes';

      if (this.isPlaying !== wasPlaying) {
        this.handlePlayStateChange();
        if (this.isPlaying) {
          // this.shadowRoot.getElementById('waveform-wrapper').style.animationPlayState = 'running';
          this.animation.play();
        } else {
          // this.shadowRoot.getElementById('waveform-wrapper').style.animationPlayState = 'paused';
          this.animation.pause();
        }
      }
    } catch (error) {
      console.warn('Failed to update playback info:', error);
    }
  }

  async generateWaveformPattern() {
    const waveDir = this.settings.getString('dir.vdjStemWaveforms');
    if (!waveDir) {
      return '';
    }

    await this.processStems(this.currentFilepath);
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

      // Check if all waveform cache files exist
      const cacheFilesExist = await this.checkWaveformCache(originalSubpath);
      if (cacheFilesExist) {
        console.log('Loading cached waveforms for', filepath);
        await this.loadCachedWaveforms(originalSubpath);
        return;
      }

      console.log('Processing new waveforms for', filepath);
      let fullFilepath = this.settings.getString('dir.vdjStems') + this.getDirChecksumAndSuffix(this.path.dirname(filepath)) + this.path.basename(filepath) + '.vdjstems';

      const tmpPath = await this.fs.mkdtemp(this.path.join(this.os.tmpdir(), 'ss-stems-'));
      await this.exec(`yes y | ffmpeg -i "${fullFilepath}" -map 0:a:0 -c copy ${tmpPath}/1.m4a -map 0:a:1 -c copy ${tmpPath}/2.m4a -map 0:a:2 -c copy ${tmpPath}/3.m4a -map 0:a:3 -c copy ${tmpPath}/4.m4a -map 0:a:4 -c copy ${tmpPath}/5.m4a`);

      for (let i = 1; i <= 5; i++) {
        const audioContext = new AudioContext();
        const response = await fetch(`file://${tmpPath}/${i}.m4a`)
        const buffer = await response.arrayBuffer();
        const options = {
          audio_context: audioContext,
          array_buffer: buffer,
          scale: this.audioScale,
          amplitude_scale: 2.5,
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

        console.log(`Waveform has ${waveform.channels} channels`);
        console.log(`Waveform has length ${waveform.length} points`);

        // Save waveform data to cache
        await this.saveWaveformCache(originalSubpath, i, waveform.toArrayBuffer());

        // Render the waveform
        this.renderWaveform(waveform, i);
      }

      // Clean up temp directory
      await this.exec(`rm -rf "${tmpPath}"`);

    } catch(err) {
      this.addAlert('WaveformData failed creating waveform for ' + filepath + ' : ' + err, 8);
    }
  }

  async checkWaveformCache(originalSubpath) {
    try {
      for (let i = 1; i <= 5; i++) {
        const cacheFile = this.getWaveformCacheFilepath(originalSubpath, i);
        const exists = await ipcRenderer.invoke('existsfilepath', cacheFile);
        if (!exists) {
          return false;
        }
      }
      return true;
    } catch (error) {
      console.warn('Error checking waveform cache:', error);
      return false;
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
      console.warn('Error loading cached waveforms:', error);
      // If cache loading fails, fall back to regeneration
      throw error;
    }
  }

  async saveWaveformCache(originalSubpath, stemIndex, waveformData) {
    try {
      const cacheFile = this.getWaveformCacheFilepath(originalSubpath, stemIndex);
      const cacheDir = this.path.dirname(cacheFile);

      // Ensure cache directory exists
      // TODO await ipcRenderer.invoke('ensuredirectory', cacheDir);

      // Save waveform data as arraybuffer
      await this.fs.writeFile(cacheFile, Buffer.from(waveformData));
      console.log(`Saved waveform cache: ${cacheFile}`);
    } catch (error) {
      console.warn('Error saving waveform cache:', error);
      // Non-fatal error, continue without caching
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
    // Remove leading slash if present
    songFilepath = songFilepath.replace(/^\//, '');
    const cacheDir = this.settings.getString('dir.vdjStemWaveforms') || (this.settings.getString('dir.cache') + '/waveforms');
    return this.path.join(cacheDir, songFilepath.replace(/\.[^.]+$/, `_stem${stemIndex}_${this.audioScale}waveform.data`));
  }

  getVdjStemWaveformFilepath(songFilepath, stemIndex) {
    songFilepath = songFilepath.replace(this.settings.getString('dir.music'), '');
    return this.settings.getString('dir.vdjStemWaveforms') + songFilepath.replace(/\.[^.]+$/, '_' + stemIndex + '.webp');
  }

  static get styles() {
    return [
      css`
        :host {
          display: var(--vdj-waveform-display);
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
          left: 50%;
          width: 100%;
          height: 100%;
          position: relative;
          animation: scroll-waveform linear; /* Name the animation and set a linear timing function */
          animation-play-state: paused; Start the animation paused
        }

        /* @keyframes scroll-waveform {
          from {
            transform: translateX(0);
          }
          to {
            transform: translateX(-1680px); /* This should match the negative width of the waveform-track */
          }
        } */
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
          left: 50%;
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
          right: calc(100% / 2 + 10px);
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
