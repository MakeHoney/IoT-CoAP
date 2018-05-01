const PORT = 7777;

var dgram = require('dgram');
var fs = require('fs');

var server = dgram.createSocket("udp6");

server.on("message", function(msg, rinfo) {
	console.log("server got : " + msg + " from " +
		rinfo.address + ":" + rinfo.port);

	fs.appendFile('log.txt', msg + "\n", function(err) {
		if(err) throw err;
	});
});

server.on("listening", function() {
	var address = server.address();
	console.log("server listening " + address.address + 
		":" + address.port);
});

server.bind(PORT);