const PORT = 7777;

var http = require('http');
var request = require('request')

var url = 'http://localhost:8545/'
var postData = {
	jsonrpc: '2.0',
	method: 'eth_accounts',
	params: [],
	id: 1
}

var options = {
  url: url,
  method: 'POST',
  headers: {
		"Content-Type": "application/json"
	},
  body: postData,
  json: true
}

request(options, function (err, res, body) {
	if (err) {
		console.error('error posting json: ', err)
		throw err
	}
	var headers = res.headers
	var statusCode = res.statusCode
	console.log('headers: ', headers)
	console.log('statusCode: ', statusCode)
	console.log('body: ', body)
})