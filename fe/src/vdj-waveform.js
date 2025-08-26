import { css, html, LitElement, unsafeCSS } from "lit";

import crc32 from 'crc-32';
import WaveformData from 'waveform-data';
import * as d3 from 'd3';

import "./icon-button.js";
import { AlertsMixin } from "./mixin-alerts-pub.js";
import { SettingsMixin } from "./mixin-settings.js";

class VDJWaveform extends AlertsMixin(SettingsMixin(LitElement)) {
  render() {
    const waveformOffset = this.getWaveformOffsetPercentage() * 2000;
    return html`
      <div class="waveform-container" @click="${this.handleClick}">
        <div class="deck-label">Deck ${this.deck}</div>
        ${this.trackLoaded ? html`
          <div class="waveform-wrapper ${this.isPlaying ? 'playing' : 'paused'}"
               style="--waveform-offset: ${waveformOffset}px; --transition-duration: ${this.transitionDuration}s; --animation-duration: ${this.animationDuration}s;">
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
      waveformUrl: { type: String },
      currentTime: { type: Number },
      displayTime: { type: Number },
      duration: { type: Number },
      isPlaying: { type: Boolean },
      trackLoaded: { type: Boolean },
      bpm: { type: Number },
      absoluteBpm: { type: Number },
      transitionDuration: { type: Number },
      animationDuration: { type: Number }
    };
  }

  constructor() {
    super();
    this.fs = require('fs/promises');
    this.os = require('os');
    this.path = require('path');
    const util = require('util');
    this.exec = util.promisify(require('child_process').exec);
    this.waveformUrl = '';
    this.currentTime = 0;
    this.displayTime = 0;
    this.duration = 0;
    this.isPlaying = false;
    this.trackLoaded = false;
    this.updateInterval = null;
    this.displayTimeInterval = null;
    this.stemNames = ['vocal', 'hihat', 'bass', 'instruments', 'kick'];

    // BPM and smooth animation properties
    this.bpm = 0;
    this.absoluteBpm = 0;
    this.playbackRate = 1;
    this.transitionDuration = 0.1; // Duration for position updates
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

    this.startUpdating();

    // Listen for the processed waveform data from the main process
    ipcRenderer.on('waveform-data', (event, waveformData) => {
        const canvas = this.shadowRoot.getElementById('waveform-canvas-1');
        const ctx = canvas.getContext('2d');
        const waveform = WaveformData.create(waveformData);

        // Clear the canvas
        ctx.clearRect(0, 0, canvas.width, canvas.height);

        // Draw the waveform
        const channel = waveform.channel(0); // Get the first channel
        const scaleY = (val) => (val * canvas.height / 2) + canvas.height / 2;

        ctx.beginPath();
        ctx.moveTo(0, scaleY(channel.min_sample(0)));

        for (let i = 0; i < channel.length; i++) {
            // Draw the top half of the waveform
            ctx.lineTo(i, scaleY(channel.min_sample(i)));
            ctx.lineTo(i, scaleY(channel.max_sample(i)));
        }

        ctx.strokeStyle = '#007bff';
        ctx.lineWidth = 1;
        ctx.stroke();
    });
  }

  disconnectedCallback() {
    super.disconnectedCallback();
    this.stopUpdating();
    this.stopDisplayTimeUpdate();
  }

  firstUpdated() {
  }

  updateTimeSync(newTime) {
    this.currentTime = newTime;
    this.displayTime = newTime;
    this.lastSyncTime = newTime;
    this.lastSyncTimestamp = Date.now();

    // Force a quick transition to sync position
    this.transitionDuration = 0.1;
    this.requestUpdate();
  }

  updatePlaybackRate() {
    if (this.absoluteBpm > 0 && this.bpm > 0) {
      this.playbackRate = this.bpm / this.absoluteBpm;
      // Update animation duration based on playback rate
      this.animationDuration = 2 / this.playbackRate; // Adjust base duration by playback rate
      console.log(`Playback rate: ${this.playbackRate} (BPM: ${this.bpm}/${this.absoluteBpm})`);
      this.requestUpdate();
    }
  }

  handlePlayStateChange() {
    if (this.isPlaying) {
      this.startDisplayTimeUpdate();
      // Set longer transition for smooth movement
      this.transitionDuration = this.animationDuration;
    } else {
      this.stopDisplayTimeUpdate();
      // Quick transition when paused
      this.transitionDuration = 0.1;
    }
    this.requestUpdate();
  }

  startDisplayTimeUpdate() {
    this.stopDisplayTimeUpdate();
    this.displayTimeInterval = setInterval(() => {
      const now = Date.now();
      const elapsedMs = now - this.lastSyncTimestamp;
      const expectedProgress = elapsedMs * this.playbackRate;
      this.displayTime = Math.min(this.lastSyncTime + expectedProgress, this.duration);
      this.requestUpdate();
    }, 50); // Update display time every 50ms for smooth time display
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
    }, 60000);  // Sync frequency
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
      this.updateTimeSync(newTime);
      const data = await window.vdj.query(`deck ${this.deck} play`);
      console.log('playing: ' + data);
      const wasPlaying = this.isPlaying;
      this.isPlaying = data === 'yes';

      if (this.isPlaying !== wasPlaying) {
        this.handlePlayStateChange();
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

    this.waveformUrl1 = 'mask-image:url("file://' + this.getVdjStemWaveformFilepath(this.currentFilepath, 1) + '");background-color: ' + this.getStemColor(1);
    this.waveformUrl2 = 'mask-image:url("file://' + this.getVdjStemWaveformFilepath(this.currentFilepath, 2) + '");background-color: ' + this.getStemColor(2);
    this.waveformUrl3 = 'mask-image:url("file://' + this.getVdjStemWaveformFilepath(this.currentFilepath, 3) + '");background-color: ' + this.getStemColor(3);
    this.waveformUrl4 = 'mask-image:url("file://' + this.getVdjStemWaveformFilepath(this.currentFilepath, 4) + '");background-color: ' + this.getStemColor(4);
    this.waveformUrl5 = 'mask-image:url("file://' + this.getVdjStemWaveformFilepath(this.currentFilepath, 5) + '");background-color: ' + this.getStemColor(5);

    await this.processStems(this.currentFilepath);
  }

  formatTime(ms) {
    const seconds = Math.floor(ms / 1000);
    const minutes = Math.floor(seconds / 60);
    const remainingSeconds = seconds % 60;
    return `${minutes}:${remainingSeconds.toString().padStart(2, '0')}`;
  }

  getWaveformOffsetPercentage() {
    if (this.duration === 0) return 0;
    // Use displayTime for smooth animation
    const timeToUse = this.displayTime || this.currentTime;
    // Calculate how much to offset the waveform (negative value to move it left)
    return -Math.min((timeToUse / this.duration), 1);
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
          scale: 128
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
        await this.saveWaveformCache(originalSubpath, i, waveform.toJSON());

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
        const waveformData = JSON.parse(await this.fs.readFile(cacheFile, 'utf8'));
        const waveform = WaveformData.create(waveformData);
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

      // Save waveform data as JSON
      await this.fs.writeFile(cacheFile, JSON.stringify(waveformData, null, 2), 'utf8');
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
    const svgWidth = 2000;
    const svgHeight = 80; // Increased height for a larger waveform
    const verticalOffset = svgHeight / 2; // Center the waveform vertically

    x.domain([0, waveform.length]).rangeRound([0, svgWidth]);
    y.domain([d3.min(min), d3.max(max)]).rangeRound([verticalOffset, -verticalOffset]);

    const area = d3.area()
      .x((d, i) => x(i))
      .y0((d, i) => y(min[i]))
      .y1((d, i) => y(d));

    const graph = container.append('svg')
      .style('width', '2000px')
      .style('height', '40px')
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
    return this.path.join(cacheDir, songFilepath.replace(/\.[^.]+$/, `_stem${stemIndex}_waveform.json`));
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
          position: relative;
          background: linear-gradient(90deg, #1a1a2e, #16213e, #1a1a2e);
        }
        .waveform-wrapper {
          left: 50%;
          width: 100%;
          height: 100%;
          position: relative;
          transform: translateX(var(--waveform-offset, 0%));
        }
        .waveform-wrapper.paused {
          transition: transform var(--transition-duration, 0.1)s ease-out;
        }
        .waveform-wrapper.playing {
          transition: transform var(--transition-duration, 2)s linear;
        }
        .waveform {
          width: 100%;
          height: 40px;
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
      `,
    ];
  }
}

window.customElements.define('vdj-waveform', VDJWaveform);
