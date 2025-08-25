import { css, html, LitElement, unsafeCSS } from "lit";

import crc32 from 'crc-32';
import WaveformData from 'waveform-data';


import "./icon-button.js";
import { AlertsMixin } from "./mixin-alerts-pub.js";
import { SettingsMixin } from "./mixin-settings.js";

class VDJWaveform extends AlertsMixin(SettingsMixin(LitElement)) {
  render() {
    const waveformOffset = this.getWaveformOffsetPercentage();
    return html`
      <div class="waveform-container" @click="${this.handleClick}">
        <div class="deck-label">Deck ${this.deck}</div>
        ${this.trackLoaded ? html`
          <div class="waveform-wrapper" style="--waveform-offset: ${waveformOffset}%">
            <canvas id="waveform-canvas-1" class="waveform loaded"></canvas>
            <canvas id="waveform-canvas-2" class="waveform loaded"></canvas>
            <canvas id="waveform-canvas-3" class="waveform loaded"></canvas>
            <canvas id="waveform-canvas-4" class="waveform loaded"></canvas>
            <canvas id="waveform-canvas-5" class="waveform loaded"></canvas>
          </div>
          <div class="progress-indicator"></div>
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
  }

    firstUpdated() {
    }

  startUpdating() {
    this.updateWaveform();
    this.updateInterval = setInterval(() => {
      this.updatePlaybackInfo();
    }, 500);  // Update 10 times per second for smooth animation
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

  getWaveformOffsetPercentage() {
    if (this.duration === 0) return 0;
    // Calculate how much to offset the waveform (negative value to move it left)
    return -Math.min((this.currentTime / this.duration) * 100, 100);
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
      // if (exists) {
      //   return;
      // }
      let fullFilepath = this.settings.getString('dir.vdjStems') + this.getDirChecksumAndSuffix(this.path.dirname(filepath)) + this.path.basename(filepath) + '.vdjstems';
      // await this.fs.access(fullFilepath, this.fs.F_OK | this.fs.R_OK);
      const tmpPath = await this.fs.mkdtemp(this.path.join(this.os.tmpdir(), 'ss-stems-'));
      await this.exec(`yes y | ffmpeg -i "${fullFilepath}" -map 0:a:0 -c copy ${tmpPath}/1.m4a -map 0:a:1 -c copy ${tmpPath}/2.m4a -map 0:a:2 -c copy ${tmpPath}/3.m4a -map 0:a:3 -c copy ${tmpPath}/4.m4a -map 0:a:4 -c copy ${tmpPath}/5.m4a`);
      for (let i = 1; i <= 5; i++) {
        let waveformFilepath = this.getVdjStemWaveformFilepath(originalSubpath, i);
        // const audioBuffer = this.fs.readFileSync(`${tmpPath}/${i}.m4a`);
        // ipcRenderer.send('process-audio', `${tmpPath}/${i}.m4a`);
        // try {

          const audioContext = new AudioContext();

          const response = await fetch(`file://${tmpPath}/${i}.m4a`)
          const buffer = await response.arrayBuffer();
          const options = {
            audio_context: audioContext,
            array_buffer: buffer,
            scale: 128
          };
          // buffer = await audioCtx.decodeAudioData(await response.arrayBuffer());

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

          const waveformData =  waveform.toJSON();


          const canvas = this.shadowRoot.getElementById(`waveform-canvas-${i}`);
          const ctx = canvas.getContext('2d');
          // const waveform = WaveformData.create(waveformData);

          // Clear the canvas
          ctx.clearRect(0, 0, canvas.width, canvas.height);

          // Draw the waveform
          const channel = waveform.channel(0); // Get the first channel
          const scaleY = (amplitude, height) => {
            const range = 256;
            const offset = 128;

            return height - ((amplitude + offset) * height) / range;
          }

          ctx.beginPath();

          // Loop forwards, drawing the upper half of the waveform
          for (let x = 0; x < waveform.length; x++) {
            const val = channel.max_sample(x);

            ctx.lineTo(x + 0.5, scaleY(val, canvas.height) + 0.5);
          }

          // Loop backwards, drawing the lower half of the waveform
          for (let x = waveform.length - 1; x >= 0; x--) {
            const val = channel.min_sample(x);

            ctx.lineTo(x + 0.5, scaleY(val, canvas.height) + 0.5);
          }

          ctx.strokeStyle = '#007bff';
          // ctx.lineWidth = 1;
          ctx.closePath();
          ctx.stroke();
          ctx.fill();

          // Send the processed waveform data back to the renderer
        //   event.sender.send('waveform-data', waveform.toJSON());
        // } catch (error) {
        //   console.error('Error processing audio file:', error);
        //   mainWindow.webContents.send('addalert', {'a': `Failed wavefjorm creation. ${error}`});
        // }
        // const waveform = await new Promise((resolve, reject) => {
        //   WaveformData.createFromAudio(audioBuffer, {
        //     samplingRate: 100, // Adjust sampling rate for detail vs. file size
        //   }, (err, waveform) => {
        //     if (err) {
        //       return reject(err);
        //     }
        //     resolve(waveform);
        //   });
        // });
        // const canvas = document.getElementById(`waveform-canvas-${i}`);
        // const ctx = canvas.getContext('2d');
        // ctx.clearRect(0, 0, canvas.width, canvas.height);
        // const channel = waveform.channel(0); // Get the first channel
        // const scaleY = (val) => (val * canvas.height / 2) + canvas.height / 2;
        // ctx.beginPath();
        // ctx.moveTo(0, scaleY(channel.min_sample(0)));

        // for (let i = 0; i < channel.length; i++) {
        //     // Draw the top half of the waveform
        //     ctx.lineTo(i, scaleY(channel.min_sample(i)));
        //     ctx.lineTo(i, scaleY(channel.max_sample(i)));
        // }

        // ctx.strokeStyle = '#007bff';
        // ctx.lineWidth = 1;
        // ctx.stroke();

        // let wfDirPath = this.path.dirname(waveformFilepath);
        // try  {
        //   await this.fs.access(wfDirPath, this.fs.F_OK);
        // } catch(err) {
        //   await this.fs.mkdir(wfDirPath, { recursive: true });
        // }
        // await this.fs.writeFile(waveformFilepath, Buffer.from(await img[0].arrayBuffer()));
        // console.log('File written successfully to ' + waveformFilepath);
      }
    } catch(err) {
      this.addAlert('WaveformData failed creating waveform for ' + filepath + ' : ' + err, 8);
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
        .waveform-wrapper {
          width: 10000px;
          height: 100%;
          position: relative;
          transform: translateX(var(--waveform-offset, 0%));
          transition: transform 0.1s linear;
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
