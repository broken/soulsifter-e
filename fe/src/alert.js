class AlertLevel {
  static DEBUG = new AlertLevel("debug");
  static INFO = new AlertLevel("info");
  static WARNING = new AlertLevel("warning");
  static ERROR = new AlertLevel("error");
  static FATAL = new AlertLevel("fatal");
  constructor(name) {
    this.name = name;
  }
}

export class Alert {
  constructor(id, msg, progress = undefined, callback = undefined, level = AlertLevel.INFO) {
    this.id = id;
    this.msg = msg;
    this.progress = progress;
    this.level = level;
    this.callback = callback;
  }
}
