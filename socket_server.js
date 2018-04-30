const PORT = 7777;

var dgram = require('dgram');

var server = dgram.createSocket("udp4");

server.on("message", function(msg, rinfo) {
	console.log("server got : " + msg + "from " +
		rinfo.address + " : " + rinfo.port);
});

server.on("listening", function() {
	var address = server.address();
	console.log("server listening " + address.address + 
		" : " + address.port);
});

server.bind(PORT);



// const port = 7777;

// var net = require('net');
// var server = net.createServer(function(socket) {
// 	console.log('client connected');

// 	socket.on('close', function() {
// 		console.log('client disconnected');
// 	});

// 	socket.on('data', function(data) {
// 		data = data.toString();
// 		console.log('message from client : ' + data);
// 		socket.write('echo from server : ' + data + '\n\0');
// 		/* 서버단 local ip */
// 		console.log('local = %s:%s', socket.localAddress, socket.localPort);
// 		/* 클라이언트 단 공유기(router) ip */
// 		console.log('remote = %s:%s', socket.remoteAddress, socket.remotePort);
// 	});
// });

// server.listen(port, '::', function() {
//     console.log('server listening...');
// });