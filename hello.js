

var addon = require("./build/Release/hello");

console.log(addon);
console.log(addon.startKinect());
//console.log(addon.runKinect());
var interval = setInterval(addon.runKinect,100);
setTimeout(function () {
	addon.printFrame();
	clearInterval(interval);
	addon.stopKinect();
},5000);

