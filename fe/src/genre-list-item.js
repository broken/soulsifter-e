import { css, html, LitElement } from "lit";

import { iconButton, iconButtonStyles } from "./icon-button.js";
import "./options-menu.js";
import "./options-menu-item.js";


class GenreListItem extends LitElement {
  render() {
    let icon = this.opened ? 'expand_less' : 'expand_more';
    let subgenres = this.getSubGenres().map(g =>
        html`<genre-list-item .genre="${g}" @expand-parents="${this.expand}" ?hidden="${!this.opened}" ?singleselect="${this.singleSelect}" ?optionsmenu="${this.optionsMenu}"></genre-list-item>`);
    let button = subgenres.length ? iconButton({ icon, onClick: this.toggleExpansion }) : iconButton({});
    let menu = !this.optionsMenu ? html`` : html`<options-menu @mouseenter="${this.hideTooltip}"><options-menu-item @click="${this.editAction}">Edit genre</options-menu-item></options-menu>`;
    let description = this.genre.description || 'n/a';
    return html`
      <div class="itemContainer">
        <div class="item" ?selected="${this.selected}"
             @mouseenter="${this.onMouseEnter}"
             @mouseleave="${this.onMouseLeave}">
          ${button}
          <span class="name" @click="${this.toggleSelect}">${this.genre.name}</span>
          ${menu}
          <div class="tooltip" popover="manual"><pre>${description}</pre></div>
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
    this.onMouseEnter = this.onMouseEnter.bind(this);
    this.onMouseLeave = this.onMouseLeave.bind(this);
    this.hideTooltip = this.hideTooltip.bind(this);
  }

  disconnectedCallback() {
    this.hideTooltip();
    super.disconnectedCallback();
  }

  onMouseEnter() {
    this.hideTooltip();
    this._tooltipTimer = setTimeout(() => {
      const tooltip = this.shadowRoot?.querySelector('.tooltip');
      if (tooltip && !tooltip.matches(':popover-open')) {
        try {
          tooltip.showPopover();
        } catch (e) {}
      }
    }, 700);
  }

  hideTooltip() {
    if (this._tooltipTimer) {
      clearTimeout(this._tooltipTimer);
      this._tooltipTimer = null;
    }
    const tooltip = this.shadowRoot?.querySelector('.tooltip');
    if (tooltip && tooltip.matches(':popover-open')) {
      try {
        tooltip.hidePopover();
      } catch (e) {}
    }
  }

  onMouseLeave() {
    this.hideTooltip();
  }

  getSubGenres() {
    return this.genre.children.sort((a, b) => a.name.localeCompare(b.name));
  }

  toggleExpansion(e) {
    this.hideTooltip();
    this.opened = !this.opened;
  }

  toggleSelect(e) {
    this.hideTooltip();
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
      iconButtonStyles,
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
          position: relative;
          anchor-name: --genre-item;
        }
        .item:hover {
          z-index: 10;
        }
        .item[selected] {
          background-color: var(--ss-genre-list-item-selected-background-color);
        }
        .item .name {
          flex: 1 1 auto;
        }
        icon-button, .icon-button {
          width: 24px;
          min-width: 24px;
          flex-shrink: 0;
        }
        options-menu {
          display: none;
        }
        .item:hover > options-menu {
          display: block;
        }
        .tooltip {
          margin: 0;
          inset: unset;
          position: fixed;
          position-anchor: --genre-item;
          top: anchor(bottom);
          left: anchor(start);
          position-try-fallbacks: flip-block;
          background-color: var(--ss-options-menu-background-color);
          color: var(--clr, #eee);
          border: 1px solid rgba(255, 255, 255, 0.1);
          box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
          padding: 6px 10px;
          border-radius: 4px;
          font-size: 12px;
          line-height: 1.4;
          max-width: 280px;
          z-index: 1000;
          pointer-events: none;
          opacity: 0;
          transition: opacity 0.2s ease, display 0.2s ease allow-discrete, overlay 0.2s ease allow-discrete;
        }
        .tooltip:popover-open {
          opacity: 1;
        }
        @starting-style {
          .tooltip:popover-open {
            opacity: 0;
          }
        }
        .tooltip pre {
          margin: 0;
          white-space: pre-wrap;
          word-break: break-word;
          font-family: inherit;
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
