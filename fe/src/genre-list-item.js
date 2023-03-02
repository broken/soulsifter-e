import { css, html, LitElement } from "@polymer/lit-element";

import "./icon-button.js";
import "./options-menu.js";
import "./options-menu-item.js";


class GenreListItem extends LitElement {
  render() {
    let icon = this.opened ? 'expand_less' : 'expand_more';
    let subgenres = this.getSubGenres().map(g =>
        html`<genre-list-item .genre="${g}" @expand-parents="${this.expand}" ?hidden="${!this.opened}" ?singleselect="${this.singleSelect}" ?optionsmenu="${this.optionsMenu}"></genre-list-item>`);
    let button = subgenres.length ? html`<icon-button icon=${icon} @click="${this.toggleExpansion}"></icon-button>` : html`<icon-button></icon-button>`;
    let menu = !this.optionsMenu ? html`` : html`<options-menu><options-menu-item @click="${this.editAction}">Edit genre</options-menu-item></options-menu>`;
    return html`
      <div class="itemContainer">
        <div class="item" ?selected="${this.selected}">
          ${button}
          <span @click="${this.toggleSelect}">${this.genre.name}</span>
          ${menu}
        </div>
        ${subgenres}
      </div>
    `;
  }

  static get properties() {
    return {
      genre: { type: Object },
      selected: { type: Boolean },
      opened: { type: Boolean },
      singleSelect: { type: Boolean },
      optionsMenu: { type: Boolean },
    }
  }

  constructor() {
    super();
    this.opened = false;
    this.selected = false;
    this.singleSelect = false;
    this.optionsMenu = false;
  }

  getSubGenres() {
    return this.genre.children.sort((a, b) => a.name.localeCompare(b.name));
  }

  toggleExpansion(e) {
    this.opened = !this.opened;
  }

  toggleSelect(e) {
    this.select(!this.selected);
  }

  select(selected) {
    if ((selected && !this.selected) ||
        (!selected && this.selected)) {
      this.selected = !this.selected;
      let event = new CustomEvent('toggle-select', {
          detail: { genre: this.genre },
          bubbles: true,
          composed: true });
      this.dispatchEvent(event);
    }
    if (!this.singleSelect && this.genre.children && this.genre.children.length > 0) {
      for (var i = 0, childNodes = this.shadowRoot.querySelectorAll('genre-list-item');
           i < childNodes.length; ++i) {
        childNodes[i].select(this.selected);
      }
    }
  }

  selectIffInArray(genres) {
    if (!genres) return;
    // select this and request parents to expand if found in array
    var selected = false;
    for (var i = 0; i < genres.length; ++i) {
      if (genres[i].id === this.genre.id) {
        selected = true;
        let event = new CustomEvent('expand-parents', {
            detail: { genre: this.genre },
            bubbles: true,
            composed: true });
        this.dispatchEvent(event);
      }
    }
    this.opened = false;
    this.selected = selected;
    // have all child elements check if they're in the array
    for (var i = 0, childNodes = this.shadowRoot.querySelectorAll('genre-list-item');
         i < childNodes.length; ++i) {
      childNodes[i].selectIffInArray(genres);
    }
  }

  expand() {
    this.opened = true;
  }

  editAction(e) {
    let event = new CustomEvent('genre-edit', { detail: this.genre });
    window.dispatchEvent(event);
    e.stopPropagation();
  }

  static get styles() {
    return [
      css`
        .itemContainer {
          margin: 7px 0 0 18px;
          display: flex;
          flex-direction: column;
        }
        .item {
          height: 24px;
          flex: 1 1 auto;
          display: flex;
          flex-direction: row;
          align-items: center;  
        }
        .item[selected] {
          background-color: var(--ss-genre-list-item-select-color);
        }
        .item span {
          flex: 1 1 auto;
        }
        icon-button {
          width: 24px;
        }
        options-menu {
          display: none;
        }
        .item:hover > options-menu {
          display: block;
        }



        paper-icon-button {
          transform: scale(.6);
        }


        ss-options-menu {
          --iron-icon-height: 16px;
          --iron-icon-width: 16px;
        }
        #item ss-options-menu::shadow #btn {
          display: none;
        }
        #item:hover ss-options-menu::shadow #btn {
          display: block;
        }
      `
    ];
  }
}

window.customElements.define('genre-list-item', GenreListItem);
