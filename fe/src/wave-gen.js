import { css, html, LitElement } from "lit";

import WaveSurfer from 'wavesurfer.js';

import { AlertsMixin } from "./mixin-alerts.js";
import { SettingsMixin } from "./mixin-settings.js";
import { WaveGenQueueMixin } from "./mixin-wave-gen-queue.js";
import { WaveformUtilMixin } from "./mixin-waveform-util.js";


class WaveGen extends AlertsMixin(SettingsMixin(WaveGenQueueMixin(WaveformUtilMixin(LitElement)))) {
  render() {
    return html`
      <div id="wavesurfer"></div>
    `;
  }

  static get properties() {
    return {
      songFilepath: { type: String },
    }
  }

  constructor() {
    super();
    this.fs = require('fs/promises');
    this.path = require('path');
    this.isWavesurferProcessing = false;
  }

  firstUpdated() {
    this.wavesurfer = WaveSurfer.create({
      container: this.shadowRoot.getElementById('wavesurfer'),
      height: this.waveformHeight,
      width: this.waveformWidth,
      waveColor: '#FF0000',
    });
    this.wavesurfer.on('error', function (msg) {
      this.addAlert('Wavesurfer audio error: ' + msg, 8);
    });
  }

  waveGenQueueChanged(q) {
    this.waveGenQueue = q;
    if (!this.isWavesurferProcessing) {
      this.isWavesurferProcessing = true;
      this.startProcessing(this.removeSongFromWaveGenQueue());
    }
  }

  async startProcessing(filepath) {
    try {
      while (!!filepath) {
        let fullFilepath = this.settings.getString('dir.music') + filepath;
        let waveformFilepath = this.getFullWaveformFilepath(filepath);
        await this.fs.access(fullFilepath, this.fs.F_OK | this.fs.R_OK);
        await this.wavesurfer.load(fullFilepath);
        let img = await this.wavesurfer.exportImage('image/webp', 1, 'blob');
        let wfDirPath = this.path.dirname(waveformFilepath);
        try  {
          await this.fs.access(wfDirPath, this.fs.F_OK);
        } catch(err) {
          await this.fs.mkdir(wfDirPath, { recursive: true });
        }
        await this.fs.writeFile(waveformFilepath, Buffer.from(await img[0].arrayBuffer()));
        console.log('File written successfully to ' + waveformFilepath);
        this.notifyWaveGenCompleted(filepath);
        filepath = this.removeSongFromWaveGenQueue();
      }
    } catch(err) {
      this.addAlert('Wavesurfer failed creating waveform for ' + filepath + ' : ' + err, 8);
    }
    this.isWavesurferProcessing = false;
  }

  static get styles() {
    return [
      css`
        :host {
          position: absolute;
          top: -2000px;
          left: -2000px;
        }
      `,
    ];
  }
}

window.customElements.define('wave-gen', WaveGen);
