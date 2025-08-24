import { css, html, LitElement, unsafeCSS } from "lit";

import "./icon-button.js";
import { SettingsMixin } from "./mixin-settings.js";

class VDJWaveform extends SettingsMixin(LitElement) {
  render() {
    const progressPosition = this.getProgressPercentage();
    return html`
      <div class="waveform-container" @click="${this.handleClick}">
        <div class="deck-label">Deck ${this.deck}</div>
        ${this.trackLoaded ? html`
          <div class="waveform loaded" style="--waveform-mask: ${this.waveformUrl}"></div>
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
    this.waveformUrl = '';
    this.currentTime = 0;
    this.duration = 0;
    this.isPlaying = false;
    this.trackLoaded = false;
    this.updateInterval = null;
  }

  connectedCallback() {
    super.connectedCallback();
    ipcRenderer.on(`vdj-filepath-${this.deck}`, (e, data) => {
      console.log('vdj-filepath: ' + data);
      if (data) {
        if (data !== this.currentFilepath) {
          this.currentFilepath = data;
          // In a real implementation, you would convert the filepath to a waveform image URL
          // For demo purposes, we'll use a placeholder pattern
          this.waveformUrl = this.generateWaveformPattern();
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

  generateWaveformPattern() {
    // Generate a simple waveform pattern as SVG data URL for demo
    const width = 800;
    const height = 120;
    const bars = 200;

    let path = `M 0 ${height/2}`;
    for (let i = 0; i < bars; i++) {
      const x = (i / bars) * width;
      const amplitude = Math.random() * 0.8 + 0.2;
      const y1 = height/2 - (amplitude * height/2 * 0.8);
      const y2 = height/2 + (amplitude * height/2 * 0.8);
      path += ` L ${x} ${y1} L ${x} ${y2} L ${x} ${height/2}`;
    }

    const svg = `
      <svg width="${width}" height="${height}" xmlns="http://www.w3.org/2000/svg">
        <path d="${path}" fill="white" opacity="0.9"/>
      </svg>
    `;

    return `url("data:image/svg+xml;base64,${btoa(svg)}")`;
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

  handleClick(event) {
    if (!this.trackLoaded || this.duration === 0) return;

    const rect = this.getBoundingClientRect();
    const clickX = event.clientX - rect.left;
    const percentage = clickX / rect.width;
    const seekTime = percentage * this.duration;

    if (window.vdj && window.vdj.query) {
      window.vdj.query(`deck ${this.deck} set_time ${Math.floor(seekTime)}`);
    }
  }

  static get styles() {
    return [
      css`
        :host {
          display: block;
          width: 100%;
          height: 120px;
          position: relative;
          background: #000;
          border: 1px solid #333;
          border-radius: 4px;
          overflow: hidden;
        }
        .waveform-container {
          width: 100%;
          height: 100%;
          position: relative;
          background: linear-gradient(90deg, #1a1a2e, #16213e, #1a1a2e);
        }
        .waveform {
          width: 100%;
          height: 100%;
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
