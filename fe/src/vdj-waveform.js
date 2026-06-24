import { css, html, LitElement, unsafeCSS } from "lit";

import crc32 from 'crc-32';
import WaveformData from 'waveform-data';
import * as d3 from 'd3';

import "./icon-button.js";
import { midiManager } from './midi-manager.js';
import { AlertsMixin } from "./mixin-alerts-pub.js";
import { KeyboardMixin } from "./mixin-keyboard.js";
import { SettingsMixin } from "./mixin-settings.js";


class VDJWaveform extends AlertsMixin(KeyboardMixin(SettingsMixin(LitElement))) {
  render() {
    return html`
      <div class="waveform-container" @click="${this.handleClick}">
        <div class="deck-label">Deck ${this.deck}</div>
        <div class="vocal stem-label">vocal</div>
        <div class="instruments stem-label">instruments</div>
        <div class="bass stem-label">bass</div>
        <div class="hihat stem-label">hihat</div>
        <div class="kick stem-label">kick</div>
        ${this.trackLoaded ? html`
          <div id="waveform-wrapper" class="waveform-wrapper ${this.isPlaying ? 'playing' : 'paused'}">
            <div id="waveform-canvas-0" class="waveform loaded"></div>
            <div id="waveform-canvas-3" class="waveform loaded"></div>
            <div id="waveform-canvas-2" class="waveform loaded"></div>
            <div id="waveform-canvas-1" class="waveform loaded"></div>
            <div id="waveform-canvas-4" class="waveform loaded"></div>
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
    };
  }

  static waveformHeight = 22;

  constructor() {
    super();
    this.keydownHandlerListener = (e) => this.keydownHandler(e);
    this.handleZoomListener = (e) => this.handleZoom(e);
    this.enableStemWaveformsListener = e => {
      if (this.settings.getBool('virtualdj.active') && e.detail) {
        this.startUpdating();
      } else {
        this.stopUpdating();
        this.stopDisplayTimeUpdate();
      }
    };
    this.registerMidiCallbacksListener = e => this.registerMidiCallbacks();
    this.fs = window.require('fs').promises;
    this.os = window.require('os');
    this.path = window.require('path');
    const util = window.require('util');
    this.exec = util.promisify(window.require('child_process').exec);
    this.displayTime = 0;
    this.duration = 0;
    this.isPlaying = false;
    this.trackLoaded = false;
    this.updateInterval = null;
    this.displayTimeInterval = null;
    this.stemNames = ['vocal', 'hihat', 'bass', 'instruments', 'kick'];
    this.loadDelay = 200;
    this.currentFilepath = undefined;
    this.waveforms = [undefined, undefined, undefined, undefined, undefined];

    // Settings for generated waveforms
    this.audioScale = this.settings.getInt('virtualdj.waveform.audioScale');
    this.audioAmplitudeScale = 2.1;

    // BPM and smooth animation properties
    this.lastSyncTime = 0;
    this.lastSyncTimestamp = 0;
  }

  handleZoom(e) {
    // This is called when the zoom level changes. We do this here so different components can
    // react to the zoom change together.
    if (this.trackLoaded) {
      this.updateWrapperAnimation();
    }
  }

  keydownHandler(e) {
    // Check if we should ignore this event (e.g. user typing in an input field)
    if (this.validateKeyboardShortcut && !this.validateKeyboardShortcut(e)) return;
    if (e.defaultPrevented) return;

    const zoomIn = this.settings.getString('hotkey.waveform.zoomIn');
    const zoomOut = this.settings.getString('hotkey.waveform.zoomOut');

    let newVal = undefined;
    if (zoomIn && zoomIn === e.code) {
      e.preventDefault();
      const currentVal = this.settings.getInt('virtualdj.waveform.pixelBeatDistance');
      newVal = Math.min(1000, Math.max(currentVal + 1, Math.round(currentVal * 1.2)));
    } else if (zoomOut && zoomOut === e.code) {
      e.preventDefault();
      const currentVal = this.settings.getInt('virtualdj.waveform.pixelBeatDistance');
      newVal = Math.max(1, Math.min(currentVal - 1, Math.round(currentVal / 1.2)));
    }

    if (newVal !== undefined) {
      this.settings.putInt('virtualdj.waveform.pixelBeatDistance', newVal);
      this.settings.save();
      window.dispatchEvent(new CustomEvent('vdj-waveform-zoom'));
    }
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('enable-stem-waveforms', this.enableStemWaveformsListener);
    window.addEventListener('register-midi-callbacks', this.registerMidiCallbacksListener);
    window.addEventListener('keydown', this.keydownHandlerListener);
    window.addEventListener('vdj-waveform-zoom', this.handleZoomListener);
  }

  disconnectedCallback() {
    window.removeEventListener('enable-stem-waveforms', this.enableStemWaveformsListener);
    window.removeEventListener('register-midi-callbacks', this.registerMidiCallbacksListener);
    window.removeEventListener('keydown', this.keydownHandlerListener);
    window.removeEventListener('vdj-waveform-zoom', this.handleZoomListener);
    super.disconnectedCallback();
    this.stopUpdating();
    this.stopDisplayTimeUpdate();
  }

  startDisplayTimeUpdate() {
    this.stopDisplayTimeUpdate();
    this.displayTimeInterval = setInterval(() => {
      const now = Date.now();
      const elapsedMs = now - this.lastSyncTimestamp;
      this.displayTime = Math.min(this.lastSyncTime + elapsedMs, this.duration);
    }, 250);  // 50ms for smooth. 250ms for more or less.
  }

  stopDisplayTimeUpdate() {
    if (this.displayTimeInterval) {
      clearInterval(this.displayTimeInterval);
      this.displayTimeInterval = null;
    }
  }

  async updateBpmData(render = true) {
    let changed = false;
    try {
      // bpm
      const bpmData = await window.vdj.query(`deck ${this.deck} get_bpm`);
      console.log('bpm: ' + bpmData);
      let bpm = parseFloat(bpmData) || 0;
      changed |= this.bpm !== bpm;
      this.bpm = bpm;

      // duration
      const durationData = await window.vdj.query(`deck ${this.deck} get_time total`);
      let duration = parseInt(durationData) || 0;
      changed |= this.duration !== duration;
      this.duration = duration;
    } catch (error) {
      console.warn('Failed to get BPM data: ', error);
    }

    if (changed && render) {
      this.updateWrapperAnimation();
      for (let i = 0; i < this.stemNames.length; i++) {
        this.renderWaveform(i);
      }
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
    const position_change_midi = this.settings.getString(`virtualdj.midi.position.deck${this.deck}`);
    for (const midi of position_change_midi.split(',')) {
      const note = midi.trim();
      if (note) {
        midiManager.registerInput(
            note,
            e => setTimeout(this.updatePlaybackInfo.bind(this), 50)
        );
      }
    }
    const bpm_change_midi = this.settings.getString(`virtualdj.midi.bpm.deck${this.deck}`);
    for (const midi of bpm_change_midi.split(',')) {
      const note = midi.trim();
      if (midi.trim()) {
        midiManager.registerInput(
            note,
            e => setTimeout(this.updateBpmData.bind(this), 50)
        );
      }
    }
  }

  startUpdating() {
    this.updateWaveform();
    this.updateInterval = setInterval(() => {
      this.updateWaveform();
      this.updatePlaybackInfo();
      this.updateBpmData();
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
      this.waveformWidth = undefined;
      this.waveforms = [undefined, undefined, undefined, undefined, undefined];

      // Clear canvas elements from previous song
      if (this.shadowRoot) {
        for (let i = 0; i < 5; i++) {
          const container = this.shadowRoot.getElementById(`waveform-canvas-${i}`);
          if (container) {
            container.innerHTML = '';
          }
        }
      }

      // update duration & bpm of the song
      await this.updateBpmData(false);

      // load waveform from the cache if it exists
      const originalSubpath = this.currentFilepath.replace(this.settings.getString('dir.music'), '');
      let exists = true;
      try {
        for (let i = 0; i < 5; i++) {
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
        this.updateWrapperAnimation();
        return;
      }

      // waveform not cached - so generate it
      // first, wait for the stems file to exist
      const fullFilepath = this.settings.getString('dir.vdjStems') + this.getDirChecksumAndSuffix(this.path.dirname(this.currentFilepath)) + this.path.basename(this.currentFilepath) + '.vdjstems';
      let stemsFileExists = false;
      const startTime = Date.now();
      console.log(`Waiting for stems file to exist: ${fullFilepath}`);
      while (Date.now() - startTime < 60000) {
        try {
          await this.fs.access(fullFilepath);
          stemsFileExists = true;
          break;
        } catch (err) {
          await new Promise(resolve => setTimeout(resolve, 1000));
        }
      }
      if (!stemsFileExists) {
        throw new Error(`Stems file does not exist after 60 seconds: ${fullFilepath}`);
      }

      // unpack stem file
      const tmpPath = await this.fs.mkdtemp(this.path.join(this.os.tmpdir(), 'ss-stems-'));
      await this.exec(`yes y | ffmpeg -i "${fullFilepath}" -map 0:a:0 -c copy ${tmpPath}/0.m4a -map 0:a:1 -c copy ${tmpPath}/1.m4a -map 0:a:2 -c copy ${tmpPath}/2.m4a -map 0:a:3 -c copy ${tmpPath}/3.m4a -map 0:a:4 -c copy ${tmpPath}/4.m4a`);

      // write out each stem separately
      for (let i = 0; i < 5; i++) {
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

        // use waveform-data to parse the audio buffer
        const waveform = await new Promise((resolve, reject) => {
          WaveformData.createFromAudio(options, (err, waveform_arg) => {
            if (err) {
              return reject(err);
            }
            resolve(waveform_arg);
          });
        });
        console.log(`Waveform has ${waveform.length} points (length)`);
        this.waveforms[i] = waveform;

        // save waveform data to cache
        await this.saveWaveformCache(originalSubpath, i, waveform.toArrayBuffer());

        // render the waveform
        this.renderWaveform(i);
      }

      this.updateWrapperAnimation();

      // Clean up temp directory
      await this.exec(`rm -rf "${tmpPath}"`);

    } catch (error) {
      this.addAlert('WaveformData failed creating waveform: ' + error, 8);
      console.warn('Failed to update the waveform. ', error);
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
      if (this.animation) {
        // Only sync if drift is significant (> 200ms) to prevent stutter
        const drift = Math.abs(this.animation.currentTime - newTime);
        if (drift > 200) {
          this.animation.currentTime = newTime;
        }
      }

      // check if track is playing
      const data = await window.vdj.query(`deck ${this.deck} play`);
      const wasPlaying = this.isPlaying;
      this.isPlaying = data === 'yes';
      if (this.isPlaying !== wasPlaying) {
        if (this.isPlaying) {
          if (this.animation) this.animation.play();
          this.startDisplayTimeUpdate();
        } else {
          if (this.animation) this.animation.pause();
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
    // Remove drive name (e.g. /Volumes/T7 or /Volume/T7 or C:)
    let pathWithoutDrive = directoryPath;
    if (directoryPath.match(/^\/Volumes?\/[^\/]+/i)) {
      pathWithoutDrive = directoryPath.replace(/^\/Volumes?\/[^\/]+/i, '');
    } else if (directoryPath.match(/^[a-zA-Z]:/)) {
      pathWithoutDrive = directoryPath.replace(/^[a-zA-Z]:/, '');
    }

    // Remove separators before getting the last 4 characters.
    const dirName = pathWithoutDrive.replace(/\\|\//g, '');
    const last4 = dirName.slice(-4);
    const first4 = dirName.slice(0, 4);

    // Compute the CRC32 checksum as a 32-bit integer, then format as a hex string.
    const csum = crc32.buf(Buffer.from(pathWithoutDrive, 'utf-8'));
    const chksum = (csum >>> 0).toString(16).padStart(8, '0');

    return `${first4}..${last4}-${chksum}/`;
  }

  async loadCachedWaveforms(originalSubpath) {
    try {
      for (let i = 0; i < 5; i++) {
        const cacheFile = this.getWaveformCacheFilepath(originalSubpath, i);
        const waveformData = await this.fs.readFile(cacheFile);
        this.waveforms[i] = WaveformData.create(waveformData.buffer);
        this.renderWaveform(i);
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
      await this.fs.mkdir(this.path.dirname(cacheFile), { recursive: true });
      await this.fs.writeFile(cacheFile, Buffer.from(waveformData));
      console.log(`Saved waveform cache: ${cacheFile}`);
    } catch (error) {
      console.warn('Error saving waveform cache: ', error);
    }
  }

  renderWaveform(stemIndex) {
    const waveform = this.waveforms[stemIndex];
    if (!waveform) return;
    const channel = waveform.channel(0);
    const container = d3.select(this.shadowRoot.getElementById(`waveform-canvas-${stemIndex}`));

    // Clear any existing content
    container.selectAll('*').remove();

    const x = d3.scaleLinear();
    const y = d3.scaleLinear();

    const min = channel.min_array();
    const max = channel.max_array();

    // Set the desired width and height for the SVG container
    const verticalOffset = VDJWaveform.waveformHeight / 2; // Center the waveform vertically

    if (!this.waveformWidth) {
      this.updateWrapperAnimation();
      if (!this.waveformWidth) {
        return;
      }
    }

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
      .attr('stroke', this.getStemColor(stemIndex));
  }

  updateWrapperAnimation() {
    if (!this.duration || !this.bpm) {
      return;
    }

    const pixelBeatDistance = this.settings.getInt('virtualdj.waveform.pixelBeatDistance');
    const newWaveformWidth = Math.round(this.duration * this.bpm / 60 / 1000 * pixelBeatDistance);

    // If animation exists and width hasn't changed significantly, skip recreation
    if (this.animation && this.waveformWidth && Math.abs(this.waveformWidth - newWaveformWidth) < 1) {
      return;
    }

    // create the animation object using the Web Animations API
    const w = this.shadowRoot.getElementById('waveform-wrapper');
    if (!w) {
      return;
    }

    this.waveformWidth = newWaveformWidth;

    if (this.animation) {
      this.animation.cancel();
    }
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
    this.animation.currentTime = this.displayTime;
    if (this.isPlaying) {
      this.animation.play();
    }
    // Update widths of existing SVGs if they exist
    for (let i = 0; i < 5; i++) {
      this.renderWaveform(i);
    }
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
          contain: strict;
        }
        .waveform-wrapper {
          left: 25%;
          width: 100%;
          height: 100%;
          position: relative;
          will-change: transform;
          backface-visibility: hidden;
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
        .instruments {
          top: 24px;
        }
        .bass {
          top: 46px;
        }
        .hihat {
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
