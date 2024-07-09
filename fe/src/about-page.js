import { css, html, LitElement } from "lit";


class AboutPage extends LitElement {
  render() {
    const version = window.ss.version
    const build = window.ss.build
    return html`
      <section>
        <img src="../resources/soulsifter.png">
        <div class="text-container">
          <div class="text">
            <span class="emphasis">Soul Sifter</span><br>
            version ${version}<br>
            build ${build}
          </div>
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
          white-space: nowrap;
        }
        section {
          display: flex;
          flex-direction: row;
          gap: 22px;
          justify-content: center;
        }
        img {
          width: 162px;
        }
        .emphasis {
          color: var(--ss-about-page-text-emphasis-color);
          font-size: 1.1em;
        }
        .text {
          position: absolute;
          top: 50%;
          left: 50%;
          transform: translate(-50%, -50%);
        }
        .text-container {
          width: 98px;
          position: relative;
        }
      `,
    ];
  }
}

window.customElements.define('about-page', AboutPage);
