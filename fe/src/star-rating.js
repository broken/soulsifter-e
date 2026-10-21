import { css, html } from "lit";
import { ifDefined } from "lit/directives/if-defined.js";

export const starRatingStyles = css`
  .star-rating {
    white-space: nowrap;
    display: inline-block;
    vertical-align: middle;
    color: var(--ss-star-rating-unselected-icon-color);
  }
  .star-rating.trashed {
    color: var(--ss-star-rating-trashed-icon-color);
  }
  .star-rating .material-icons {
    font-family: 'Material Icons';
    font-size: var(--mdc-icon-size, 13px);
    line-height: 1;
    letter-spacing: normal;
    text-transform: none;
    display: inline-block;
    white-space: nowrap;
    word-wrap: normal;
    direction: ltr;
    -webkit-font-smoothing: antialiased;
    text-rendering: optimizeLegibility;
    -moz-osx-font-smoothing: grayscale;
    font-feature-settings: 'liga';
    user-select: none;
    color: inherit;
    vertical-align: middle;
    padding: 0 1px;
  }
  .star-rating .material-icons[selected] {
    color: var(--ss-star-rating-selected-icon-color);
  }
  .star-rating.interactive .material-icons {
    cursor: pointer;
    transition: transform 0.1s ease, color 0.1s ease;
  }
  .star-rating.interactive:hover .material-icons {
    color: var(--ss-star-rating-unselected-icon-color);
  }
  .star-rating.interactive:hover .material-icons:not(:hover ~ .material-icons) {
    color: var(--ss-star-rating-selected-icon-color);
  }
  .star-rating.interactive .material-icons:hover {
    transform: scale(1.25);
  }
`;

function isStarSelected(star, value, max, count) {
  let ratio = max / count;
  let weightedStar = star * ratio;
  let full = weightedStar < value;
  let half = !full && weightedStar - Math.floor(ratio / 2) < value;
  return full ? 2 : half ? 1 : 0;
}

function starGlyph(selected) {
  return !selected ? 'star_border' : selected === 1 ? 'star_half' : 'star';
}

export function starRating(valueOrOptions, options = {}) {
  // Support both positional and object-based arguments
  let opts = {};
  if (typeof valueOrOptions === 'object' && valueOrOptions !== null) {
    opts = { ...valueOrOptions };
  } else if (typeof valueOrOptions === 'number' || typeof valueOrOptions === 'string') {
    opts = { value: valueOrOptions, ...options };
  } else {
    opts = { ...options };
  }

  const {
    value = 0,
    count = 5,
    max = 5,
    readOnly = false,
    className = '',
    id,
    style,
    title,
    once = false,
  } = opts;

  const numValue = Number(value) || 0;
  const classAttr = ['star-rating', !readOnly ? 'interactive' : '', className, opts.class].filter(Boolean).join(' ');

  // Fast-path to render a single template to increase performance.
  if (readOnly && count === 5) {
    const s0 = isStarSelected(0, numValue, max, 5);
    const s1 = isStarSelected(1, numValue, max, 5);
    const s2 = isStarSelected(2, numValue, max, 5);
    const s3 = isStarSelected(3, numValue, max, 5);
    const s4 = isStarSelected(4, numValue, max, 5);

    return html`<span
      id="${ifDefined(id)}"
      class="${classAttr}"
      style="${ifDefined(style)}"
      title="${ifDefined(title)}"
      ?once="${once}"
    ><span class="material-icons" ?selected=${s0 > 0}>${starGlyph(s0)}</span><span class="material-icons" ?selected=${s1 > 0}>${starGlyph(s1)}</span><span class="material-icons" ?selected=${s2 > 0}>${starGlyph(s2)}</span><span class="material-icons" ?selected=${s3 > 0}>${starGlyph(s3)}</span><span class="material-icons" ?selected=${s4 > 0}>${starGlyph(s4)}</span></span>`;
  }

  const clickOrSelectHandler = opts.onSelect || opts.onClick || opts.click || opts['@click'];
  const starItems = [];
  for (let s = 0; s < count; ++s) {
    const selected = isStarSelected(s, numValue, max, count);
    const icon = starGlyph(selected);
    const clickHandler = !readOnly
      ? (e) => {
          const nextValue = numValue === s + 1 ? 0 : s + 1;
          if (clickOrSelectHandler) {
            clickOrSelectHandler(nextValue, e, s);
          } else {
            const host = e.currentTarget.parentElement;
            if (host) {
              host.dispatchEvent(new CustomEvent('change', {
                detail: { value: nextValue },
                bubbles: true,
                composed: true,
              }));
            }
          }
        }
      : undefined;

    starItems.push(html`<span
      class="material-icons"
      ?selected=${selected > 0}
      @click=${clickHandler}
    >${icon}</span>`);
  }

  return html`
    <span
      id="${ifDefined(id)}"
      class="${classAttr}"
      style="${ifDefined(style)}"
      title="${ifDefined(title)}"
      ?once="${once}"
    >${starItems}</span>
  `;
}
