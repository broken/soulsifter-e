import { css, html, LitElement } from "lit";

import WaveSurfer from 'wavesurfer.js';

import { AlertsMixin } from "./mixin-alerts-pub.js";
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
    this.fs = window.require('fs');
    this.path = window.require('path');
    this.isWavesurferProcessing = false;
  }

  firstUpdated() {
    this.wavesurfer = WaveSurfer.create({
      container: this.shadowRoot.getElementById('wavesurfer'),
      height: this.waveformHeight,
      width: this.waveformWidth,
      waveColor: '#FFFFFF',
    });
    this.wavesurfer.on('error', (msg) => {
      this.addAlert('Wavesurfer audio error: ' + msg);
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
    while (!!filepath) {
      try {
        let fullFilepath = this.settings.getString('dir.music') + filepath;
        let waveformFilepath = this.getFullWaveformFilepath(filepath);

        let exists = false;
        const startTime = Date.now();
        while (Date.now() - startTime < 60000) {
          try {
            await this.fs.promises.access(fullFilepath, this.fs.constants.F_OK | this.fs.constants.R_OK);
            exists = true;
            break;
          } catch (err) {
            await new Promise(resolve => setTimeout(resolve, 1000));
          }
        }
        if (!exists) {
          await this.fs.promises.access(fullFilepath, this.fs.constants.F_OK | this.fs.constants.R_OK);
        }

        await this.wavesurfer.load(fullFilepath);
        let img = await this.wavesurfer.exportImage('image/webp', 1, 'blob');
        let wfDirPath = this.path.dirname(waveformFilepath);
        try  {
          await this.fs.promises.access(wfDirPath, this.fs.constants.F_OK);
        } catch(err) {
          await this.fs.promises.mkdir(wfDirPath, { recursive: true });
        }
        await this.fs.promises.writeFile(waveformFilepath, Buffer.from(await img[0].arrayBuffer()));
        console.log('File written successfully to ' + waveformFilepath);
        this.notifyWaveGenCompleted(filepath);
      } catch(err) {
        this.addAlert('Wavesurfer failed creating waveform for ' + filepath + ' : ' + err, 1);
      }
      filepath = this.removeSongFromWaveGenQueue();
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
