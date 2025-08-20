class VirtualDjClient extends Object {

  constructor() {
    super();
    this.settings = new ss.SoulSifterSettings();
  }

  send(command) {
    ipcRenderer.send(
        'vdj-send',
        this.settings.getString('virtualdj.ip'),
        this.settings.getInt('virtualdj.port'),
        command
    );
  }

};

if (!window.vdj) window.vdj = new VirtualDjClient();
