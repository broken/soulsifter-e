// Modules to control application life and create native browser window
const { app, dialog, BrowserWindow, ipcMain, nativeImage } = require('electron')
const path = require('path');

const isDev = process.env.IS_DEV === 'true';

const createWindow = () => {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    width: 1680,
    height: 498,
    x: 0,
    y: 0,
    icon: path.join(__dirname, 'DVDRipper.icns'),
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: true,
      contextIsolation: false
    }
  })

  // and load the index.html of the app.
  mainWindow.loadFile('build/index.html')

  // Open the DevTools.
  mainWindow.webContents.openDevTools()
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  createWindow()

  app.on('activate', () => {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

ipcMain.on('ondragstart', (event, filepath, icon) => {
  nativeImage.createThumbnailFromPath(icon, {width: 50, height: 50})
  .then((img) => {
    event.sender.startDrag({
      file: filepath,
      icon: img,
    });
  })
  .catch((err) => {
    console.log(err);
  });
})

ipcMain.handle('opendialog', async (event, title, defaultPath, props) => {
  const result = await dialog.showOpenDialog({
    title: title,
    defaultPath: defaultPath,
    properties: props
  });
  return result;
})

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit()
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.
