import { css, html, LitElement } from "@polymer/lit-element";


class SearchInfo extends LitElement {
  render() {
    return html`
      <header>
        <span>Search atoms</span>
        <hr>
      </header>
      <section>
        <div class="right flex">
          <span class="emphasis">id</span> : <br>
          <span class="emphasis">a</span> or <span class="emphasis">artist</span> : <br>
          <span class="emphasis">t</span> or <span class="emphasis">title</span> : <br>
          <span class="emphasis">remixer</span> : <br>
          <span class="emphasis">r</span> or <span class="emphasis">rating</span> : <br>
          <span class="emphasis">comment</span> : <br>
          <span class="emphasis">c</span> or <span class="emphasis">curator</span> : <br>
          <span class="emphasis">e</span> or <span class="emphasis">energy</span> : <br>
          <span class="emphasis">bpm</span> : <br>
        </div>
        <div class="flex">
          Internal song ID<br>
          Song artist<br>
          Song title<br>
          Song remixer<br>
          Minimum song rating<br>
          Song comment<br>
          Discovery curator<br>
          Song energy<br>
          Song BPM (single or span)<br>
        </div>
        <div class="right flex">
          <span class="emphasis">aid</span> : <br>
          <span class="emphasis">n</span> or <span class="emphasis">album</span> : <br>
          <span class="emphasis">m</span> or <span class="emphasis">mixed</span> : <br>
          <span class="emphasis">l</span> or <span class="emphasis">label</span> : <br>
          <span class="emphasis">y</span> or <span class="emphasis">year</span> : <br>
          <span class="emphasis">trashed</span> : <br>
          <span class="emphasis">lowq</span> : <br>
          <span class="emphasis">l</span> or <span class="emphasis">limit</span> : <br>
          <span class="emphasis">q</span> or <span class="emphasis">query</span> : <br>
          <span class="emphasis">o</span> or <span class="emphasis">order</span> : <br>
        </div>
        <div class="flex">
          Internal album ID<br>
          Album name<br>
          Is album mixed<br>
          Label released on<br>
          Year released<br>
          Has song been deleted<br>
          Is song low quality<br>
          Result count limit<br>
          Custom query predicate<br>
          Order By (rand, released, added)<br>
        </div>
      </section>
      <header>
        <span>Special characters</span>
        <hr>
      </header>
      <section>
        <div class="right flex">
          <span class="emphasis">-</span> : <br>
          <span class="emphasis">"</span> : <br>
          <span class="emphasis">&lt;</span>, <span class="emphasis">&gt;</span>, <span class="emphasis">=</span> : <br>
        </div>
        <div class="flex">
          Negates a query atom<br>
          Used to group terms<br>
          Used for number ranges<br>
        </div>
        <div class="right flex">
          <span class="emphasis">0</span> or <span class="emphasis">1</span> : <br>
          <span class="emphasis">\\</span> : <br>
        </div>
        <div class="flex">
          Boolean values<br>
          Escape character<br>
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
          width: 575px;
          white-space: nowrap;
        }
        header {
          color: var(--ss-search-info-heading-color);
          margin-top: 14px;
        }
        section {
          display: flex;
          flex-direction: row;
        }
        .emphasis {
          color: var(--ss-search-info-emphasis-color);
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

window.customElements.define('search-info', SearchInfo);
