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
      let event = new CustomEvent('midi-connected', { detail: mySynth });
      window.dispatchEvent(event);
    })
    .catch(err => this.addAlert('Unable to connect to Midi controller. ' + err));
  }

  _midiConnected(e) {
    this.midiConnected(e.detail);
  }

  getMidiInputs() {
    return [];
  }

  midiConnected(midiInput) {
    const typenames = {
      '8': 'noteoff',
      '9': 'noteon',
      'A': 'polyaftertouch',  // right name?
      'B': 'controlchange',
      'C': 'programchange',  // right name?
      'D': 'channelaftertouch',  // right name?
      'E': 'pitchwheel'  // right name?
    }
    let inputsByChannel = this.getMidiInputs().filter(x => !!x[0]).reduce((dict, x) => { (dict[x[0][1]] || (dict[x[0][1]] = [])).push(x); return dict; }, {});
    for (const chan in inputsByChannel) {
      let midiChan = midiInput.channels[parseInt(chan, 16) + 1];
      let inputsByType = Object.values(inputsByChannel[chan]).reduce((dict, x) => { (dict[x[0][0]] || (dict[x[0][0]] = [])).push(x); return dict; }, {});
      for (const type in inputsByType) {
        const inputsByNum = Object.values(inputsByType[type]).reduce((dict, x) => {
          const num = parseInt(x[0].substring(3, 5), 16);
          (dict[num] || (dict[num] = [])).push(x);
          return dict;
        }, {});
        midiChan.addListener(typenames[type.toUpperCase()], e => {
          if (e.message.dataBytes[0] in inputsByNum) {
            inputsByNum[e.message.dataBytes[0]].forEach(x => {
              if (x[0].substring(6,8) == 'xx' || e.rawValue == parseInt(x[0].substring(6, 8), 16)) x[1](e);
            });
          }
        });
      }
    }
  }
}

export const MidiMixin = dedupingMixin(midiMixin);
