import { css, html, LitElement } from "lit";

import "@material/mwc-icon";

import "./options-menu.js";
import "./options-menu-item.js";
import { AlertsMixin } from "./mixin-alerts.js";


class PlaylistListItem extends AlertsMixin(LitElement) {
  render() {
    let icon = !!this.playlist.query ? html`<mwc-icon class="mdc-ripple-surface mdc-ripple-surface--primary">find_replace</mwc-icon>` : html``;
    return html`
      <div class="itemContainer" @drop="${this.handleDrop}" @dragover="${this.handleDragOver}" @dragleave="${this.handleDragLeave}">
        <div class="item" ?selected="${this.selected}" @click="${this.toggleSelect}">
          <div class="group">
            <span>${this.playlist.name}</span>
            ${icon}
          </div>
          <options-menu id="opt">
            <options-menu-item @click="${this.editAction}">Edit playlist</options-menu-item>
            <options-menu-item @click="${this.copyAction}">Copy to clipboard</options-menu-item>
            <options-menu-item @click="${this.deleteAction}">Delete playlist</options-menu-item>
            <options-menu-item @click="${this.youtubeSyncAction}" ?hidden="${!this.playlist.youtubeId}">Sync to YoutTube</options-menu-item>
          </options-menu>
        </div>
      </div>
    `;
  }

  static get properties() {
    return {
      playlist: { type: Object },
      selected: { type: Boolean },
    }
  }

  constructor() {
    super();
    this.selected = false;
  }

  toggleSelect(e) {
    this.select = !this.selected;
    let grp = e.metaKey || e.shiftKey || e.ctrlKey;
    let event = new CustomEvent('toggle-select', {
        detail: { playlist: this.playlist, multi: grp },
        bubbles: true,
        composed: true });
    this.dispatchEvent(event);
  }

  editAction(e) {
    let event = new CustomEvent('playlist-edit', { detail: this.playlist });
    window.dispatchEvent(event);
    e.stopPropagation();
  }

  copyAction(e) {
    let str = this.playlist.name + "\n\n";
    let i = 0;
    ss.PlaylistEntry.findByPlaylistId(this.playlist.id).sort((a, b) => a.position - b.position).forEach(
        (e) => str += ++i + ". " + e.song.artist + " - "  + e.song.title + "\n");
    ipcRenderer.send('copytoclipboard', str);
    this.shadowRoot.getElementById('opt').style.display = 'none';
    setTimeout(() => this.shadowRoot.getElementById('opt').style.removeProperty('display'), 1);
    e.stopPropagation();
  }

  deleteAction(e) {
    let ytPlaylistId = this.playlist.youtubeId;
    ss.PlaylistEntry.findByPlaylistId(this.playlist.id).forEach((e) => e.erase());
    this.playlist.styles = [];
    this.playlist.update();
    this.playlist.erase();
    let event = new CustomEvent('update-playlists', { });
    window.dispatchEvent(event);
    e.stopPropagation();
    if (!!ytPlaylistId) window.yt.deletePlaylist(ytPlaylistId);
  }

  youtubeSyncAction(e) {
    window.yt.updatePlaylistEntries(this.playlist.id);
  }

  handleDragLeave(e) {
    e.preventDefault();
  }

  handleDragOver(e) {
    e.preventDefault();
  }

  handleDrop(e) {
    e.preventDefault();
    // only makes sense to drag & drop songs for non-smart playlists
    if (!!this.playlist.query) {
      this.addAlert('Cannot manually add songs to smart playlists.', 4);
      return;
    }
    // this must be in multiple steps for it to save correctly
    let entries = ss.PlaylistEntry.findByPlaylistId(this.playlist.id);
    let entry = new ss.PlaylistEntry();
    entry.playlist = this.playlist;
    entry.song = window.ssDraggedObj;
    entry.position = entries.length;
    entry.save();
    // update youtube if necessary
    if (!!this.playlist.youtubeId) window.yt.addPlaylistEntry(entry.id);
  }

  static get styles() {
    return [
      css`
        .itemContainer {
          border-radius: 2px;
          margin: 7px 20px;
          height: 30px;
          display: flex;
          flex-direction: column;
        }
        .item {
          height: 30px;
          padding-left: 40px;
          flex: 1 1 auto;
          display: flex;
          flex-direction: row;
          align-items: center;
        }
        .item[selected] {
          background-color: var(--ss-playlist-list-item-selected-background-color);
        }
        .group {
          display: flex;
          flex-direction: row;
          flex: 1 1 auto;
        }
        mwc-icon {
          flex: 1 1 auto;
          --mdc-icon-size: 16px;
        }
        options-menu {
          display: none;
        }
        .item:hover > options-menu {
          display: block;
        }
      `
    ];
  }
}

window.customElements.define('playlist-list-item', PlaylistListItem);
