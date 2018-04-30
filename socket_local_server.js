const PORT = 7777;

var dgram = require('dgram');

var localServer = dgram.createSocket("udp4");

localServer.on("message", function(msg, rinfo) {
	console.log("localServer got : " + msg + " from " + rinfo.address + ":" + rinfo.port);

	localServer.send(msg, 0, msg.length, 7777, "13.209.8.64", function(err, bytes) {
		console.log("localServer sent message(" + msg + ") to main server");
	});
});

localServer.on("listening", function() {
	var address = localServer.address();
	console.log("localServer listening " + address.address + 
		":" + address.port);
});

localServer.bind(PORT);