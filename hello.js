

var addon = require("./build/Release/hello");

console.log(addon);
console.log(addon.startKinect());


addon.setFrameCallback(function ( arg ) {
	console.log(arg);
});


var interval = setInterval(addon.runKinect,100);
setTimeout(function () {
	console.log("Stopping...");
	//addon.printFrame();
	clearInterval(interval);
	addon.stopKinect();
	console.log("Stopped.");
},5000);

