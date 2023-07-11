console.log("app starting :)");
const { app, BrowserWindow } = require('electron');

app.commandLine.appendSwitch('v');

function createWindow() {
    console.log("creating window :)");
    const win = new BrowserWindow({
        width: 512, height: 512
    });
    console.log("loading page :)");
    win.loadFile('src/index.html');
    console.log("page loaded :)");
}

app.whenReady().then(createWindow);
console.log("everything executed :)");
