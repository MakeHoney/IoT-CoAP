const PORT = 7777;

var dgram = require('dgram');
var crawler = dgram.createSocket("udp4");
var message = new Buffer("");

crawler.send(message, 0, message.length, 7777, "")