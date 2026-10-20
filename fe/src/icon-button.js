import { css, html } from "lit";
import { ifDefined } from "lit/directives/if-defined.js";
import { ref } from "lit/directives/ref.js";

export const iconButtonStyles = css`
  .icon-button {
    font-family: 'Material Icons';
    font-size: var(--mdc-icon-size, 24px);
    vertical-align: middle;
    cursor: pointer;
    user-select: none;
    line-height: 1;
    display: inline-block;
    text-transform: none;
    letter-spacing: normal;
    word-wrap: normal;
    white-space: nowrap;
    direction: ltr;
    -webkit-font-smoothing: antialiased;
    text-rendering: optimizeLegibility;
    -moz-osx-font-smoothing: grayscale;
    font-feature-settings: 'liga';
    color: inherit;
  }
  .icon-button.active {
    color: var(--ss-icon-button-emphasis-color);
  }
  .icon-button[disabled] {
    cursor: default;
    filter: grayscale(1);
    opacity: 0.5;
    pointer-events: none;
  }
`;

export function iconButton(iconOrOptions, onClick, options = {}) {
  // Support both positional and object-based arguments
  let opts = {};
  if (typeof iconOrOptions === 'object' && iconOrOptions !== null) {
    opts = { ...iconOrOptions };
  } else {
    opts = { icon: iconOrOptions, onClick, ...options };
  }

  const clickHandler = opts.onClick || opts.click || opts['@click'];
  const dropHandler = opts.onDrop || opts.drop || opts['@drop'];
  const dragOverHandler = opts.onDragOver || opts.dragover || opts['@dragover'];
  const dragLeaveHandler = opts.onDragLeave || opts.dragleave || opts['@dragleave'];
  const {
    icon = '',
    className = '',
    id,
    style,
    disabled = false,
    title,
  } = opts;

  const classAttr = ['material-icons', 'icon-button', className, opts.class].filter(Boolean).join(' ');

  const bindIconProp = (el) => {
    if (el && !el.__hasIconProp) {
      el.__hasIconProp = true;
      Object.defineProperty(el, 'icon', {
        get() {
          return this.textContent;
        },
        set(val) {
          this.textContent = val;
        },
        configurable: true,
      });
    }
  };

  return html`
    <span
      ${ref(bindIconProp)}
      id="${ifDefined(id)}"
      class="${classAttr}"
      style="${ifDefined(style)}"
      title="${ifDefined(title)}"
      ?disabled="${disabled}"
      @click="${clickHandler}"
      @drop="${dropHandler}"
      @dragover="${dragOverHandler}"
      @dragleave="${dragLeaveHandler}"
    >${icon}</span>
  `;
}
