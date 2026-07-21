import { css, html, LitElement } from "lit";
import { SettingsMixin } from "./mixin-settings.js";

class HotkeysInfo extends SettingsMixin(LitElement) {
  render() {
    const playPause = this.settings.getString('hotkey.media.playPause') || 'Ø';
    const navBack = this.settings.getString('hotkey.nav.back') || 'Ø';
    const navForward = this.settings.getString('hotkey.nav.forward') || 'Ø';
    const hotkeyUp = this.settings.getString('hotkey.songList.up') || 'Ø';
    const hotkeyDown = this.settings.getString('hotkey.songList.down') || 'Ø';
    const hotkeySelect = this.settings.getString('hotkey.songList.select') || 'Ø';
    const zoomIn = this.settings.getString('hotkey.waveform.zoomIn') || 'Ø';
    const zoomOut = this.settings.getString('hotkey.waveform.zoomOut') || 'Ø';

    return html`
      <header>
        <span>Keyboard shortcuts</span>
        <hr>
      </header>
      <section>
        <div class="right flex">
          <span class="emphasis">?</span> : <br>
          <span class="emphasis">${hotkeyUp}</span> : <br>
          <span class="emphasis">${hotkeyDown}</span> : <br>
          <span class="emphasis">${hotkeySelect}</span> : <br>
          <span class="emphasis">Escape</span> : <br>
        </div>
        <div class="flex">
          Show keyboard shortcuts<br>
          Move up in song list<br>
          Move down in song list<br>
          Select song in song list<br>
          Clear song selection & Close dialog<br>
        </div>
        <div class="right flex">
          <span class="emphasis">${playPause}</span> : <br>
          <span class="emphasis">${navBack}</span> : <br>
          <span class="emphasis">${navForward}</span> : <br>
          <span class="emphasis">${zoomIn}</span> : <br>
          <span class="emphasis">${zoomOut}</span> : <br>
        </div>
        <div class="flex">
          Play / Pause audio<br>
          Go back in song trail<br>
          Go forward in song trail<br>
          Waveform zoom in<br>
          Waveform zoom out<br>
        </div>
      </section>
    `;
  }

  static get styles() {
    return [
      css`
        :host {
          display: flex;
          flex-direction: column;
          width: 623px;
          white-space: nowrap;
        }
        header {
          color: var(--ss-search-info-text-heading-color);
          margin-top: 14px;
        }
        section {
          display: flex;
          flex-direction: row;
        }
        .emphasis {
          color: var(--ss-search-info-text-emphasis-color);
        }
        .right {
          text-align: right;
        }
        .flex {
          flex: 1;
          flex-basis: 0.000000001px;
        }
      `,
    ];
  }
}

window.customElements.define('hotkeys-info', HotkeysInfo);
