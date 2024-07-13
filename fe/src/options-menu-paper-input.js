import { css, html, LitElement } from "lit";

import "@material/web/textfield/filled-text-field.js";


class OptionsMenuPaperInput extends LitElement {
  render() {
    return html`
      <md-filled-text-field id="input" label="${this.label}" value="${this.value}" ?required=${this.required} no-asterisk @focus="${this.focusChanged}" @change="${this.focusChanged}" @input="${this.inputChanged}"></md-filled-text-field>
      <div id="selectBox" elevation="3" @click="${this.selected}">
        <slot></slot>
      </div>
    `;
  }

  static get properties() {
    return {
      label: { type: String },
      value: { type: String },
      required: { type: Boolean },
    }
  }

  constructor() {
    super();
    this.required = false;
  }

  focusChanged(e) {
    let input_element = this.shadowRoot.getElementById('input');
    if (input_element.value) {
      this.shadowRoot.getElementById('selectBox').classList.add('show');
      input_element.value = '';
    } else {
      // Use timeout; otherwise the dropdown closes before the click gets it selected.
      setTimeout(() => {
        this.shadowRoot.getElementById('selectBox').classList.remove('show');
        if (!input_element.value) input_element.value = this.value;
      }, 250);
    }
  }

  selected(e) {
    this.shadowRoot.getElementById('selectBox').classList.remove('show');
    let i = 0;
    let path = e.composedPath();
    for (; i < path.length; ++i) {
      if (path[i].nodeName == 'OPTIONS-MENU-ITEM') {
        break;
      }
    }
    this.value = path[i].innerText;
    this.shadowRoot.getElementById('input').value = this.value;
    this.unhideChildren();
  }

  unhideChildren() {
    let options = this.shadowRoot.querySelector('slot').assignedNodes();
    for (let i = 0; i < options.length; ++i) {
      if (options[i].nodeName == 'OPTIONS-MENU-ITEM') {
        options[i].classList.remove('hidden');
      }
    }
  }

  inputChanged(e) {
    setTimeout(() => {
      let options = this.shadowRoot.querySelector('slot').assignedNodes();
      let val = this.shadowRoot.querySelector('md-filled-text-field').value;
      for (let i = 0; i < options.length; ++i) {
        if (options[i].nodeName == 'OPTIONS-MENU-ITEM') {
          if (!val) {
            options[i].removeAttribute('hidden');
          } else {
            if (options[i].innerText.indexOf(val) == -1) {
              options[i].setAttribute('hidden', true);
            } else {
              options[i].removeAttribute('hidden');
            }
          }
        }
      }
    }, 200);
  }

  static get styles() {
    return [
      css`
        :host {
          position: relative;
        }
        #selectBox {
          background-color: var(--ss-options-menu-background-color);
          box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
          padding: 15px;
          position: absolute;
          top: 24px;
          right: 4px;
          border-radius: 2px;
          display: none;
          opacity: 0;
          transition: opacity 1s;
          z-index: 101;
          flex-direction: column;
          height: 172px;
          overflow-y: scroll;
        }
        #selectBox.show {
          display: flex;
          opacity: 1;
        }
        md-filled-text-field {
          width: 100%;
        }
      `
    ];
  }
}

window.customElements.define('options-menu-paper-input', OptionsMenuPaperInput);
