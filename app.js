var coap        = require('coap'),
    server      = coap.createServer({ type: 'udp6' })

server.on('request', function(req, res) {
  let body = [];
  req.on('data', function(chunk) {
    body.push(chunk);
  }).on('end', function() {
    body = Buffer.concat(body).toString();
    res.end(body + 'dd');
  })
  // res.end('Hello ' + req.url.split('/')[1] + '\n')
})

// the default CoAP port is 5683
server.listen(function() {
  var req = coap.request('coap://[bbbb::1415:9200:17:9df6]:5683/red')
  console.log('server listening...');
  req.on('response', function(res) {
    console.log('server listening...2');
    res.pipe(process.stdout)
    // res.on('end', function() {
    //   process.exit(0)
    // })
  })

  req.end()
})

// const coap = require('coap'), 
//       req  = coap.request('coap://localhost/shine') 

// req.on('response', function(res) { 
//       res.pipe(process.stdout) 
// })
