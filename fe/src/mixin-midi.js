import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

import { WebMidi } from "webmidi";

let midiMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
    };
  }

  constructor() {
    super();
    this._midiListener = (e) => this._midiConnected(e);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('midi-connected', this._midiListener);
  }

  disconnectedCallback() {
    window.removeEventListener('midi-connected', this._midiListener);
    super.disconnectedCallback();
  }

  connectToMidiController(e) {
    WebMidi
    .enable()
    .then(() => {
      if (WebMidi.inputs.length < 1) {
        console.log('No device detected.');
      } else {
        WebMidi.inputs.forEach((device, index) => {
          console.log(`${index}: ${device.name}`);
        });
      }
      const mySynth = WebMidi.getInputByName(this.settings.getString('audio.midiControllerName'));
      let myChan = mySynth.channels[this.settings.getInt('audio.volumeMidiChannel')];
      let event = new CustomEvent('midi-connected', { detail: myChan });
      window.dispatchEvent(event);
    })
    .catch(err => this.addAlert('Unable to connect to Midi controller. ' + err));
  }

  _midiConnected(e) {
    this.midiConnected(e.detail);
  }

  midiConnected(chan) {
  }
}

export const MidiMixin = dedupingMixin(midiMixin);
