import { css, html, LitElement } from "lit";

import "@polymer/paper-radio-button/paper-radio-button.js";
import "@material/web/switch/switch.js";

import { SearchOptionsMixin } from "./mixin-search-options.js";


class SearchOptions extends SearchOptionsMixin(LitElement) {
  render() {
    return html`
      <section>
        <div class="option">
          <md-switch id="bpmRestrict" @change="${this.checkedChanged}"></md-switch>
          <label for="bpmRestrict">BPM restrict</label>
        </div>
        <div class="option">
          <md-switch id="keyRestrict" @change="${this.checkedChanged}"></md-switch>
          <label for="keyRestrict">Key restrict</label>
        </div>
        <div class="option">
          <md-switch id="energyRestrict" @change="${this.checkedChanged}"></md-switch>
          <label for="energyRestrict">Energy restrict</label>
        </div>
        <div class="option">
          <md-switch id="trashedRestrict" @change="${this.checkedChanged}"></md-switch>
          <label for="trashedRestrict">Trashed restrict</label>
        </div>
        <div class="option">
          <md-switch id="repeatRestrict" @change="${this.checkedChanged}"></md-switch>
          <label for="repeatRestrict">Repeat restrict</label>
        </div>
        <div class="option">
          <md-switch id="mvRestrict" @change="${this.checkedChanged}"></md-switch>
          <label for="mvRestrict">Music Video mode</label>
        </div>
        <div class="option">
          <md-switch id="dynamicGenres" @change="${this.checkedChanged}"></md-switch>
          <label for="dynamicGenres">Dynamic genre mode</label>
        </div>
        <div class="option">
          <md-switch id="useStems" @change="${this.checkedChanged}"></md-switch>
          <label for="useStems">Use stems when available</label>
        </div>
        <br>
        <div class="option">
          <label>Order by:</label>
          <paper-radio-button id="orderByDateAdded" @change="${this.orderByDateAddedChanged}" checked></paper-radio-button>
          <label>Date added</label>
          <paper-radio-button id="orderByReleaseDate" @change="${this.orderByReleaseDateChanged}"></paper-radio-button>
          <label>Release Date</label>
          <paper-radio-button id="orderByRandom" @change="${this.orderByRandomChanged}"></paper-radio-button>
          <label>Random</label>
          <paper-radio-button id="orderByBpm" @change="${this.orderByBpmChanged}"></paper-radio-button>
          <label>Bpm</label>
          <paper-radio-button id="orderByAlbum" @change="${this.orderByAlbumChanged}"></paper-radio-button>
          <label>Album</label>
        </div>
      </section>
    `;
  }

  constructor() {
    super();
    this.orderBy = 0;
  }

  checkedChanged(e) {
    this.searchOptions = {
        bpmRestrict: this.shadowRoot.getElementById('bpmRestrict').selected,
        keyRestrict: this.shadowRoot.getElementById('keyRestrict').selected,
        energyRestrict: this.shadowRoot.getElementById('energyRestrict').selected,
        trashedRestrict: this.shadowRoot.getElementById('trashedRestrict').selected,
        repeatRestrict: this.shadowRoot.getElementById('repeatRestrict').selected,
        mvRestrict: this.shadowRoot.getElementById('mvRestrict').selected,
        dynamicGenres: this.shadowRoot.getElementById('dynamicGenres').selected,
        useStems: this.shadowRoot.getElementById('useStems').selected,
        orderBy: this.orderBy,
    };
    this.changeSearchOptions(this.searchOptions);
  }

  orderByChanged(o) {
    this.orderBy = o;
    if (o != 0) this.shadowRoot.getElementById('orderByDateAdded').checked = false;
    if (o != 1) this.shadowRoot.getElementById('orderByReleaseDate').checked = false;
    if (o != 2) this.shadowRoot.getElementById('orderByRandom').checked = false;
    if (o != 3) this.shadowRoot.getElementById('orderByBpm').checked = false;
    if (o != 4) this.shadowRoot.getElementById('orderByAlbum').checked = false;
  }
  orderByDateAddedChanged(e) {
    this.orderByChanged(0);
    this.checkedChanged();
  }
  orderByReleaseDateChanged(e) {
    this.orderByChanged(1);
    this.checkedChanged();
  }
  orderByRandomChanged(e) {
    this.orderByChanged(2);
    this.checkedChanged();
  }
  orderByBpmChanged(e) {
    this.orderByChanged(3);
    this.checkedChanged();
  }
  orderByAlbumChanged(e) {
    this.orderByChanged(4);
    this.checkedChanged();
  }

  searchOptionsChanged(x) {
    this.searchOptions = x;
    this.shadowRoot.getElementById('bpmRestrict').selected = this.searchOptions.bpmRestrict;
    this.shadowRoot.getElementById('keyRestrict').selected = this.searchOptions.keyRestrict;
    this.shadowRoot.getElementById('energyRestrict').selected = this.searchOptions.energyRestrict;
    this.shadowRoot.getElementById('trashedRestrict').selected = this.searchOptions.trashedRestrict;
    this.shadowRoot.getElementById('repeatRestrict').selected =  this.searchOptions.repeatRestrict;
    this.shadowRoot.getElementById('mvRestrict').selected =  this.searchOptions.mvRestrict;
    this.shadowRoot.getElementById('dynamicGenres').selected =  this.searchOptions.dynamicGenres;
    this.shadowRoot.getElementById('useStems').selected =  this.searchOptions.useStems;
    this.orderByChanged(this.searchOptions.orderBy);
  }

  static get styles() {
    return [
      css`
        :host {
          display: flex;
          flex-direction: column;
          width: 666px;
          white-space: nowrap;
        }
        section {
          display: flex;
          flex-direction: column;
        }
        .option {
          display: flex;
          flex-direction: row;
        }
        md-switch {
          padding-top: 3px;
          margin-left: 15px;
        }
        label {
          padding-top: 10px;
          padding-left: 10px;
          margin-right: 15px;
        }
      `
    ];
  }
}

window.customElements.define('search-options', SearchOptions);
