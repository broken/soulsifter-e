import { css, html, LitElement, unsafeCSS } from "lit";

import crc32 from 'crc-32';
import WaveSurfer from 'wavesurfer.js';

import "./icon-button.js";
import { AlertsMixin } from "./mixin-alerts-pub.js";
import { SettingsMixin } from "./mixin-settings.js";

class VDJWaveform extends AlertsMixin(SettingsMixin(LitElement)) {
  render() {
    const progressPosition = this.getProgressPercentage();
    return html`
      <div class="outside">
        <div id="wavesurfer-1" style="width: 200px"></div>
        <div id="wavesurfer-2" style="width: 200px"></div>
        <div id="wavesurfer-3" style="width: 200px"></div>
        <div id="wavesurfer-4" style="width: 200px"></div>
        <div id="wavesurfer-5" style="width: 200px"></div>
      </div>
      <div class="waveform-container" @click="${this.handleClick}">
        <div class="deck-label">Deck ${this.deck}</div>
        ${this.trackLoaded ? html`
          <div class="waveform loaded" style="${this.waveformUrl1} || ''"></div>
          <div class="waveform loaded" style="${this.waveformUrl2} || ''"></div>
          <div class="waveform loaded" style="${this.waveformUrl3} || ''"></div>
          <div class="waveform loaded" style="${this.waveformUrl4} || ''"></div>
          <div class="waveform loaded" style="${this.waveformUrl5} || ''"></div>
          <div class="progress-indicator" style="--progress-position: ${progressPosition}%"></div>
          <div class="time-display">
            ${this.formatTime(this.currentTime)} / ${this.formatTime(this.duration)}
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
      duration: { type: Number },
      isPlaying: { type: Boolean },
      trackLoaded: { type: Boolean }
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
    this.duration = 0;
    this.isPlaying = false;
    this.trackLoaded = false;
    this.updateInterval = null;
    this.stemNames = ['vocal', 'hihat', 'bass', 'instruments', 'kick'];
  }

  connectedCallback() {
    super.connectedCallback();
    if (this.settings.getBool('virtualdj.active'))
      this.style.setProperty('--vdj-waveform-display', 'flex');
    else
      this.style.setProperty('--vdj-waveform-display', 'none');
    ipcRenderer.on(`vdj-filepath-${this.deck}`, (e, data) => {
      console.log('vdj-filepath: ' + data);
      if (data) {
        if (data !== this.currentFilepath) {
          this.currentFilepath = data;
          // In a real implementation, you would convert the filepath to a waveform image URL
          // For demo purposes, we'll use a placeholder pattern
          this.generateWaveformPattern();
          window.vdj.query(`deck ${this.deck} get_time total `, `vdj-duration-${this.deck}`);
          this.trackLoaded = true;
        }
      } else {
        this.trackLoaded = false;
      }
    });
    ipcRenderer.on(`vdj-time-${this.deck}`, (e, data) => {
      console.log('time: ' + data);
      this.currentTime = parseInt(data) || 0;
    });
    ipcRenderer.on(`vdj-duration-${this.deck}`, (e, data) => {
      console.log('duration: ' + data);
      this.duration = parseInt(data) || 0;
    });
    ipcRenderer.on(`vdj-isplaying-${this.deck}`, (e, data) => {
      console.log('playing: ' + data);
      if (data === 'yes') {
        this.isPlaying = true;
      } else {
        this.isPlaying = false;
      }
    });
    this.startUpdating();
  }

  disconnectedCallback() {
    super.disconnectedCallback();
    this.stopUpdating();
  }

    firstUpdated() {
      this.stemWavesurfers = [];
      for (let i = 1; i <= 5; i++) {
        const wavesurfer = WaveSurfer.create({
          container: this.shadowRoot.getElementById(`wavesurfer-${i}`),
          waveColor: this.getStemColor(i),
          progressColor: this.getStemProgressColor(i),
          cursorColor: '#ffffff',
          barWidth: 2,
          barRadius: 3,
          responsive: true,
          height: 128,
          normalize: true,
          backend: 'WebAudio',
          // For stem separation, you'd need to implement channel splitting
          // This is a simplified version that loads the full file
          splitChannels: false
        });
        wavesurfer.on('error', (msg) => {
          this.addAlert('Wavesurfer audio error: ' + msg);
        });
        this.stemWavesurfers.push(wavesurfer);
      }
    }

  startUpdating() {
    this.updateWaveform();
    this.updateInterval = setInterval(() => {
      this.updatePlaybackInfo();
    }, 60000);  // Update 10 times per second for smooth animation
  }

  stopUpdating() {
    if (this.updateInterval) {
      clearInterval(this.updateInterval);
      this.updateInterval = null;
    }
  }

  updateWaveform() {
    try {
      window.vdj.query(`deck ${this.deck} get_filepath`, `vdj-filepath-${this.deck}`);
    } catch (error) {
      console.warn('Failed to get get filepath:', error);
    }
  }

  async updatePlaybackInfo() {
    try {
      window.vdj.query(`deck ${this.deck} get_time elapsed`, `vdj-time-${this.deck}`);
      window.vdj.query(`deck ${this.deck} play`, `vdj-isplaying-${this.deck}`);
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

  getProgressPercentage() {
    if (this.duration === 0) return 0;
    return Math.min((this.currentTime / this.duration) * 100, 100);
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
      const exists = await ipcRenderer.invoke('existsfilepath', this.getVdjStemWaveformFilepath(originalSubpath, 5));
      if (exists) {
        return;
      }
      let fullFilepath = this.settings.getString('dir.vdjStems') + this.getDirChecksumAndSuffix(this.path.dirname(filepath)) + this.path.basename(filepath) + '.vdjstems';
      await this.fs.access(fullFilepath, this.fs.F_OK | this.fs.R_OK);
      const tmpPath = await this.fs.mkdtemp(this.path.join(this.os.tmpdir(), 'ss-stems-'));
      await this.exec(`yes y | ffmpeg -i "${fullFilepath}" -map 0:a:0 -c copy ${tmpPath}/1.m4a -map 0:a:1 -c copy ${tmpPath}/2.m4a -map 0:a:2 -c copy ${tmpPath}/3.m4a -map 0:a:3 -c copy ${tmpPath}/4.m4a -map 0:a:4 -c copy ${tmpPath}/5.m4a`);
      for (let i = 1; i <= 5; i++) {
        let waveformFilepath = this.getVdjStemWaveformFilepath(originalSubpath, i);
        let wavesurfer = this.stemWavesurfers[i - 1];
        await wavesurfer.load(`/tmp/${i}.m4a`);
        let img = await wavesurfer.exportImage('image/webp', 1, 'blob');  // svg?
        let wfDirPath = this.path.dirname(waveformFilepath);
        try  {
          await this.fs.access(wfDirPath, this.fs.F_OK);
        } catch(err) {
          await this.fs.mkdir(wfDirPath, { recursive: true });
        }
        await this.fs.writeFile(waveformFilepath, Buffer.from(await img[0].arrayBuffer()));
        console.log('File written successfully to ' + waveformFilepath);
      }
    } catch(err) {
      this.addAlert('Wavesurfer failed creating waveform for ' + filepath + ' : ' + err, 8);
    }
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
        .waveform {
          width: 200px;
          height: 48px;
          background: linear-gradient(45deg, #00d4ff, #ff0080, #00ff88);
          mask-image: var(--waveform-mask);
          mask-size: cover;
          mask-position: center;
          mask-repeat: no-repeat;
          opacity: 0.8;
          transition: opacity 0.3s ease;
        }
        .waveform.loaded {
          opacity: 1;
        }
        .progress-indicator {
          position: absolute;
          top: 0;
          left: 0;
          width: 2px;
          height: 100%;
          background: #fff;
          box-shadow: 0 0 10px rgba(255, 255, 255, 0.8);
          left: var(--progress-position, 0%);
          transition: transform 0.1s linear;
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
