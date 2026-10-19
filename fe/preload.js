// All the Node.js APIs are available in the preload process.
// It has the same sandbox as a Chrome extension.

window.ss = window.ss || {};
const pkg = require('./package.json');
window.ss.version = pkg.version;
window.ss.build = pkg.build;

window.nodeAPI = {
  fs: require('fs'),
  path: require('path'),
  os: require('os'),
  util: require('util'),
  child_process: require('child_process'),
  exec: require('util').promisify(require('child_process').exec),
  ipcRenderer: require('electron').ipcRenderer,
  webUtils: require('electron').webUtils,
  get axios() {
    delete this.axios;
    return (this.axios = require('axios'));
  },
  get express() {
    delete this.express;
    return (this.express = require('express'));
  },
  get SpotifyWebApi() {
    delete this.SpotifyWebApi;
    return (this.SpotifyWebApi = require('spotify-web-api-node'));
  },
  get request() {
    delete this.request;
    return (this.request = require('request'));
  },
};