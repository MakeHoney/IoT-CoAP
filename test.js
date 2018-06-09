const PORT = 7777;

var http = require('http');
var request = require('request');
var Web3 = require('web3');

var web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:8545"));
var abi = [
	{
		"constant": true,
		"inputs": [
			{
				"name": "_key",
				"type": "uint256"
			}
		],
		"name": "getValue1",
		"outputs": [
			{
				"name": "",
				"type": "string"
			}
		],
		"payable": false,
		"stateMutability": "view",
		"type": "function"
	},
	{
		"constant": true,
		"inputs": [
			{
				"name": "_key",
				"type": "uint256"
			}
		],
		"name": "getValue2",
		"outputs": [
			{
				"name": "",
				"type": "string"
			}
		],
		"payable": false,
		"stateMutability": "view",
		"type": "function"
	},
	{
		"constant": true,
		"inputs": [
			{
				"name": "_value1",
				"type": "string"
			},
			{
				"name": "_value2",
				"type": "string"
			}
		],
		"name": "setValue",
		"outputs": [
			{
				"name": "",
				"type": "uint256"
			}
		],
		"payable": false,
		"stateMutability": "view",
		"type": "function"
	}
];
var contractInstanceAddr = "0x48c03a0550373ce3ac5d6f488cb00066b9a70787";
var t = web3.eth.contract(abi).at(contractInstanceAddr);
var blockInfo = {
	hash: "",
	number: ""
}

web3.personal.unlockAccount(web3.eth.coinbase, "bhun");

var transactionHash = t.setValue.sendTransaction("test", "wow", {from:web3.eth.coinbase});

var interval = setInterval(function() {
	var receipt = web3.eth.getTransactionReceipt(transactionHash);
	if(receipt != null) {
		blockInfo.hash = receipt.blockHash;
		blockInfo.number = receipt.blockNumber;
		console.log(blockInfo.hash);
		console.log(blockInfo.number);
		if(blockInfo.hash.length > 0) {
			/* save block info in database */
			clearInterval(interval);
		}
	}
}, 50);

var i = 0;
while(t.getValue1.call(i, {from:web3.eth.coinbase}).length) {
	console.log(t.getValue1.call(i, {from:web3.eth.coinbase}))
	i++;
}





// var url = 'http://localhost:8545/'
// var unlockAccount = {
// 	jsonprc : "2.0",
// 	method : "personal_unlockAccount",
// 	params : ["0xc03e7ca636d300ba12c09c99d4f1534b21895803",
// 						"ACC3"
// 	],
// 	id : 1
// }
//
// var deployContract = {
// 	jsonprc : "2.0",
// 	method : "eth_sendTransaction",
// 	params : [{
// 			/* coinbase */
// 			from : "0xc03e7ca636d300ba12c09c99d4f1534b21895803",
// 			data : '0x608060405234801561001057600080fd5b50610522806100206000396000f300608060405260043610610057576000357c0100000000000000000000000000000000000000000000000000000000900463ffffffff168063159c82561461005c578063491460cf14610102578063ec86cfad146101a8575b600080fd5b34801561006857600080fd5b506100876004803603810190808035906020019092919050505061026b565b6040518080602001828103825283818151815260200191508051906020019080838360005b838110156100c75780820151818401526020810190506100ac565b50505050905090810190601f1680156100f45780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b34801561010e57600080fd5b5061012d60048036038101908080359060200190929190505050610323565b6040518080602001828103825283818151815260200191508051906020019080838360005b8381101561016d578082015181840152602081019050610152565b50505050905090810190601f16801561019a5780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b3480156101b457600080fd5b50610255600480360381019080803590602001908201803590602001908080601f0160208091040260200160405190810160405280939291908181526020018383808284378201915050505050509192919290803590602001908201803590602001908080601f01602080910402602001604051908101604052809392919081815260200183838082843782019150505050505091929192905050506103db565b6040518082815260200191505060405180910390f35b6060600160008381526020019081526020016000206000018054600181600116156101000203166002900480601f0160208091040260200160405190810160405280929190818152602001828054600181600116156101000203166002900480156103175780601f106102ec57610100808354040283529160200191610317565b820191906000526020600020905b8154815290600101906020018083116102fa57829003601f168201915b50505050509050919050565b6060600160008381526020019081526020016000206001018054600181600116156101000203166002900480601f0160208091040260200160405190810160405280929190818152602001828054600181600116156101000203166002900480156103cf5780601f106103a4576101008083540402835291602001916103cf565b820191906000526020600020905b8154815290600101906020018083116103b257829003601f168201915b50505050509050919050565b60008260016000805481526020019081526020016000206000019080519060200190610408929190610451565b508160016000805481526020019081526020016000206001019080519060200190610434929190610451565b506000808154809291906001019190505550600054905092915050565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f1061049257805160ff19168380011785556104c0565b828001600101855582156104c0579182015b828111156104bf5782518255916020019190600101906104a4565b5b5090506104cd91906104d1565b5090565b6104f391905b808211156104ef5760008160009055506001016104d7565b5090565b905600a165627a7a72305820ffee401fbb564c667e4882b7ec33554c4cdb492f5dc59426e5e64cfff2fc6ebf0029',
// 	    gas: Web3.utils.toHex('4700000')
// 		}
// 	],
// 	id : 1
// }
//
// var getTransactionReceipt = {
// 	jsonprc : "2.0",
// 	method : "eth_getTransactionReceipt",
// 	params : [""],
// 	id : 1
// }
//
// var ethCall = {
// 	jsonprc : "2.0",
// 	method : "eth_call",
// 	params : ["asdf", "rrr", {
// 		/* coinbase */
// 		from : "0xc03e7ca636d300ba12c09c99d4f1534b21895803",
// 		to : "0x48c03a0550373ce3ac5d6f488cb00066b9a70787",
// 		data : "0xec86cfad",
// 		gas: Web3.utils.toHex('4700000')
// 	}],
// 	id : 1
// }
//
// function makeJSON(postData) {
// 	return options = {
// 		url: url,
// 		method: 'POST',
// 		headers: {
// 			"Content-Type": "application/json"
// 		},
// 		body: postData,
// 		json: true
// 	}
// }



//
//
// request(makeJSON(unlockAccount), function (err, res, body) {
//
// 	request(makeJSON(deployContract), function (err, res, body) {
//
// 		if (err) {
// 			console.error('error posting json: ', err)
// 			throw err
// 		}
// 		var headers = res.headers
// 		var statusCode = res.statusCode
// 		console.log('headers: ', headers)
// 		console.log('statusCode: ', statusCode)
// 		console.log('body: ', body)
// 		console.log('---------------------')
// 		console.log(t.methods.getValue1(0))
//
// 		// console.log(enc("asdf"))
// 		var transactionReceipt = makeJSON(getTransactionReceipt);
// 		transactionReceipt.body.params[0] = body.result;
//
// 		/* delay until contract mined! */
// 		/* you can get blockNumber, blockHash etc... */
//
//
// 		request(makeJSON(ethCall), function (err, res, body) {
// 			if (err) {
// 				console.error('error posting json: ', err)
// 				throw err
// 			}
// 			console.log("body : ", body);
// 		})
//
// 		// setTimeout(function() {
// 		// 	request(transactionReceipt, function (err, res, body) {
// 		// 		console.log(body.result.contractAddress);
// 		// 	})
// 		// }, 10000);
//
//
//
// 	})
// })
