import { css, html, LitElement } from "@polymer/lit-element";

import "@polymer/paper-radio-button/paper-radio-button.js";
import "@polymer/paper-toggle-button/paper-toggle-button.js";

import { SearchOptionsMixin } from "./mixin-search-options.js";


class SearchOptions extends SearchOptionsMixin(LitElement) {
  render() {
    return html`
      <section>
        <div class="option">
          <paper-toggle-button id="bpmRestrict" @checked-changed="${this.checkedChanged}"></paper-toggle-button>
          <span>BPM restrict</span>
        </div>
        <div class="option">
          <paper-toggle-button id="keyRestrict" @checked-changed="${this.checkedChanged}"></paper-toggle-button>
          <span>Key restrict</span>
        </div>
        <div class="option">
          <paper-toggle-button id="energyRestrict" @checked-changed="${this.checkedChanged}"></paper-toggle-button>
          <span>Energy restrict</span>
        </div>
        <div class="option">
          <paper-toggle-button id="trashedRestrict" @checked-changed="${this.checkedChanged}"></paper-toggle-button>
          <span>Trashed restrict</span>
        </div>
        <div class="option">
          <paper-toggle-button id="repeatRestrict" @checked-changed="${this.checkedChanged}"></paper-toggle-button>
          <span>Repeat restrict</span>
        </div>
        <div class="option">
          <paper-toggle-button id="mvRestrict" @checked-changed="${this.checkedChanged}"></paper-toggle-button>
          <span>Music Video mode</span>
        </div>
        <div class="option">
          <paper-toggle-button id="dynamicGenres" @checked-changed="${this.checkedChanged}"></paper-toggle-button>
          <span>Dynamic genre mode</span>
        </div>
        <br>
        <div class="option">
          <span>Order by:</span>
          <paper-radio-button id="orderByDateAdded" @change="${this.orderByDateAddedChanged}" checked></paper-radio-button>
          <span>Date added</span>
          <paper-radio-button id="orderByReleaseDate" @change="${this.orderByReleaseDateChanged}"></paper-radio-button>
          <span>Release Date</span>
          <paper-radio-button id="orderByRandom" @change="${this.orderByRandomChanged}"></paper-radio-button>
          <span>Random</span>
        </div>
      <section>
    `;
  }

  constructor() {
    super();
    this.orderBy = 0;
  }

  checkedChanged(e) {
    this.searchOptions = {
        bpmRestrict: this.shadowRoot.getElementById('bpmRestrict').checked,
        keyRestrict: this.shadowRoot.getElementById('keyRestrict').checked,
        energyRestrict: this.shadowRoot.getElementById('energyRestrict').checked,
        trashedRestrict: this.shadowRoot.getElementById('trashedRestrict').checked,
        repeatRestrict: this.shadowRoot.getElementById('repeatRestrict').checked,
        mvRestrict: this.shadowRoot.getElementById('mvRestrict').checked,
        dynamicGenres: this.shadowRoot.getElementById('dynamicGenres').checked,
        orderBy: this.orderBy,
    };
    this.changeSearchOptions(this.searchOptions);
  }

  orderByChanged(o) {
    this.orderBy = o;
    if (o != 0) this.shadowRoot.getElementById('orderByDateAdded').checked = false;
    if (o != 1) this.shadowRoot.getElementById('orderByReleaseDate').checked = false;
    if (o != 2) this.shadowRoot.getElementById('orderByRandom').checked = false;
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

  searchOptionsChanged(x) {
    this.searchOptions = x;
    this.shadowRoot.getElementById('bpmRestrict').checked = this.searchOptions.bpmRestrict;
    this.shadowRoot.getElementById('keyRestrict').checked = this.searchOptions.keyRestrict;
    this.shadowRoot.getElementById('energyRestrict').checked = this.searchOptions.energyRestrict;
    this.shadowRoot.getElementById('trashedRestrict').checked = this.searchOptions.trashedRestrict;
    this.shadowRoot.getElementById('repeatRestrict').checked =  this.searchOptions.repeatRestrict;
    this.shadowRoot.getElementById('mvRestrict').checked =  this.searchOptions.mvRestrict;
    this.shadowRoot.getElementById('dynamicGenres').checked =  this.searchOptions.dynamicGenres;
    this.orderByChanged(this.searchOptions.orderBy);
  }

  static get styles() {
    return [
      css`
        :host {
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
        paper-toggle-button {
          padding-top: 3px;
          margin-left: 15px;
        }
        span {
          padding-left: 10px;
          margin-right: 15px;
        }
      `
    ];
  }
}

window.customElements.define('search-options', SearchOptions);
