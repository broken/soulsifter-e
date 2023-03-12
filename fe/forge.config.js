const fs = require('fs');
const fsx = require('fs-extra');
const path = require('path');
const appPath = __dirname;

module.exports = {
  packagerConfig: {
    icon: 'DVDRipper',
  },
  rebuildConfig: {},
  makers: [
    {
      name: '@electron-forge/maker-squirrel',
      config: {},
    },
    {
      name: '@electron-forge/maker-zip',
      platforms: ['darwin'],
    },
    {
      name: '@electron-forge/maker-deb',
      config: {},
    },
    {
      name: '@electron-forge/maker-rpm',
      config: {},
    },
  ],
  hooks: {
    postPackage: async (forgeConfig, options) => {
      console.info('Packages built at:', options.outputPaths);
    },
    packageAfterCopy: async (forgeConfig, buildPath, electronVersion, platform, arch) => {
      const symlinkPath = path.join(buildPath, 'node_modules', 'soulsifter');
      return new Promise((resolve, reject) => {
        fs.unlink(symlinkPath, (err) => {
          if (err) {
            reject(`Failed to remove ss symlink: ${err}`);
          } else {
            const bePath = path.join(appPath, '..', 'be');
            fsx.copy(bePath, symlinkPath, (err) => {
              if (err) {
                reject(`Failed to copy backend directory: ${err}`);
              } else {
                resolve();
              }
            });
          }
        });
      });
    },
  },
};
