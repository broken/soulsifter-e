import { WebMidi } from "webmidi";

import { Utilities } from "webmidi";


class MidiNote {
  constructor(note) {
    if (!/^[0-9A-Fa-f]{2} [0-9A-Fa-f]{2} [0-9A-Fa-fx]{2}$/.test(note)) {
      throw new Error(`Invalid MIDI note format. Expected 'XX YY ZZ', but got "${note}"`);
    }
    const typenames = {
      '6': 'noteon',
      '7': 'noteon',
      '8': 'noteon',
      '9': 'noteon',
      'A': 'polyaftertouch',  // right name?
      'B': 'controlchange',
      'C': 'programchange',  // right name?
      'D': 'channelaftertouch',  // right name?
      'E': 'pitchwheel'  // right name?
    };
    this.channel = parseInt(note[1], 16) + 1;
    this.type = typenames[note[0].toUpperCase()];
    this.byte1 = parseInt(note.substring(3, 5), 16);
    this.byte2any = note.substring(6,8) == 'xx'
    if (this.byte2any) this.byte2 = -1;
    else this.byte2 = parseInt(note.substring(6, 8), 16);
    if (!this.byte2any && this.byte2 == 0 && this.type == 'noteon') this.type = 'noteoff';
  }
}

class MidiManager {
  constructor() {
    if (MidiManager.instance) {
      return MidiManager.instance;
    }
    this._midiInput = null;
    // This structured map will hold all registered callbacks
    this._channelCallbacks = []
    MidiManager.instance = this;
    this._msb = {};
  }

  async connect(controllerName) {
    try {
      await WebMidi.enable();
      if (WebMidi.inputs.length < 1) {
        console.log('No device detected.');
      } else {
        WebMidi.inputs.forEach((device, index) => {
          console.log(`${index}: ${device.name}`);
        });
      }
      this._midiInput = WebMidi.getInputByName(controllerName);
      if (this._midiInput) {
        console.log(`Connected to MIDI controller: ${controllerName}`);
        this._setupListeners();
      } else {
        console.error(`MIDI controller '${controllerName}' not found.`);
      }
    } catch (err) {
      console.error('Unable to connect to Midi controller.', err);
    }
  }

  async disconnect() {
    this._midiInput.removeListener();
    this._midiInput = null;
    this._channelCallbacks = [];
  }

  _setupListeners() {
    if (this._midiInput) {
      Object.entries(this._channelCallbacks).forEach(([channel, chanCb]) => {
        Object.keys(chanCb).forEach(type => {
          this._setupListenerOnChannelForType(channel, type);
        });
      });
    }
  }

  _setupListenerOnChannelForType(channel, type) {
    if (this._midiInput) {
      console.log(`Setting up listener for channel ${channel} and type ${type}`);
      let midiChan = this._midiInput.channels[channel];
      let noteCallbacks = this._channelCallbacks[channel][type];
      midiChan.addListener(type, e => {
        if (e.message.dataBytes[0] in noteCallbacks) {
          noteCallbacks[e.message.dataBytes[0]].forEach(x => {
            let note = x[0];
            let callback = x[1];
            if (note.byte2any || e.rawValue == note.byte2) callback(e);
          });
        }
      });
    }
  }

  registerInput(note, callback) {
    try {
      if (!note || !callback) return;
      const notes = note.split(',').map(n => n.trim());
      for (note of notes) {
        console.log(`${note} registering`);
        if (note.includes('/')) {
          const [msb, lsb] = note.split('/').map(n => new MidiNote(n.trim()));
          const uuid = crypto.randomUUID();
          const msbCallback = e => this._msb[uuid] = e.rawValue;
          this._registerInput(msb, msbCallback);
          const lsbCallback = e => {
            const value = Utilities.fromMsbLsbToFloat(this._msb[uuid], e.rawValue);
            e.rawValue = value;
            callback(e);
          };
          this._registerInput(lsb, lsbCallback);
        } else {
          const midiNote = new MidiNote(note);
          this._registerInput(midiNote, callback);
        }
      }
    } catch (err) {
      console.error(err);
      return;
    }
  }

  _registerInput(midiNote, callback) {
    let chanCb = this._channelCallbacks[midiNote.channel];
    if (!chanCb) {
      chanCb = {};
      this._channelCallbacks[midiNote.channel] = chanCb;
    }
    let typeCb = chanCb[midiNote.type];
    if (!typeCb) {
      typeCb = [];
      chanCb[midiNote.type] = typeCb;
    }
    let cb = typeCb[midiNote.byte1];
    if (!cb) {
      cb = [];
      typeCb[midiNote.byte1] = cb;
      this._setupListenerOnChannelForType(midiNote.channel, midiNote.type);
    }
    cb.push([midiNote, callback]);
  }

  // Method for components to unregister their inputs
  // unregisterInput(note, callback) {
  //   const midiNote = new MidiNote(note);
  //   if (!!this._channelCallbacks[midiNote.channel] &&
  //       !!this._channelCallbacks[midiNote.channel][midiNote.type] &&
  //       !!this._channelCallbacks[midiNote.channel][midiNote.type][midiNote.byte1]) {
  //     let callbacks = this._channelCallbacks[midiNote.channel][midiNote.type][midiNote.byte1];
  //     for (data in callbacks) {
  //       if (data[0] == note && data[1] == callback) {
  //         delete callbacks[data];
  //         break;
  //       }
  //     }
  //   }
  // }

  removeListeners() {
    if (this._midiInput) {
      for (const channel in this._channelCallbacks) {
        console.log(`Removing midi listeners for channel ${channel}`);
        let midiChan = this._midiInput.channels[channel];
        midiChan.removeListener();
      }
      this._channelCallbacks = []
    }
  }
}

export const midiManager = new MidiManager();
