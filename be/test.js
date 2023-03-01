var addon = require('bindings')('soulsifter');

console.log(addon.MusicService.cleanDirName('this')); // 'world'
