const PORT = 7777;

var dgram	= require('dgram'),
	packet	= require('coap-packet'),
	parse	= packet.parse,
	generate = packet.generate,
	payload	= new Buffer(''),
	message	= generate({ payload: payload });

var controller = dgram.createSocket("udp6");
var options = {
	uri: "localhost:8545",
	method: "POST",
	json: {
		jsonrpc: "2.0",
		method: "eth_accounts",
		params: "[]",
		id: 1
	}
};

controller.on("message", function(msg, rinfo) {
	msg = parse(msg).payload.toString();
	console.log("controller got : " + msg + " from " + rinfo.address + ":" + rinfo.port);

	var req = http.request(options, function(res) {
		console.log('STATUS: ' + res.statusCode);
		console.log('HEADERS: ' + JSON.stringify(res.headers));
		res.setEncoding('utf8');
		res.on('data', function (chunk) {
		console.log('BODY: ' + chunk);
	  });
	})

	req.end();

	// controller.send(msg, 0, msg.length, PORT, "::ffff:13.209.8.64", function(err, bytes) {
	// 	console.log("controller sent a message(" + msg + ") to main server");
	// });
});

controller.on("listening", function() {
	var address = controller.address();
	console.log("controller listening " + address.address + 
		":" + address.port);
});

controller.bind(PORT);
