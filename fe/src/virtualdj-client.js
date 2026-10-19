import { SettingsMixin } from "./mixin-settings.js";
 
class VirtualDjClient extends SettingsMixin(Object) {

  send(command, callback=undefined) {
    console.log(`Sending VDJ command: ${command}`)
    ipcRenderer.send(
        'vdj-send',
        this.settings.getString('virtualdj.ip'),
        this.settings.getInt('virtualdj.port'),
        command,
        callback
    );
  }

  async query(command) {
    console.log(`Sending VDJ query: ${command}`)
    return ipcRenderer.invoke(
        'vdj-query',
        this.settings.getString('virtualdj.ip'),
        this.settings.getInt('virtualdj.port'),
        command
    );
  }

};

if (!window.vdj) window.vdj = new VirtualDjClient();
