// All the Node.js APIs are available in the preload process.
// It has the same sandbox as a Chrome extension.

window.ss = {}
const pkg = require('./package.json')
window.ss.version = pkg.version
window.ss.build = pkg.build