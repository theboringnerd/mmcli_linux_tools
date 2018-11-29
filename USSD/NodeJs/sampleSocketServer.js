var WebSocket = require('websocket');
var webSocketServer = WebSocket.server;


var http = require('http');


var httpServer = http.createServer(function(request, response) {
	console.log((new Date()) + ' Received request for ' + request.url);
	response.writeHead(404);
	response.end();
});

httpServer.listen(8080, () => {
	console.log((new Date()) + ' Server is listening on port 8080');
});


var wsServer = new webSocketServer({
	httpServer: httpServer,
	autoAcceptConnections: false
});

wsServer.on('request', (request) =>{
	if(!originIsAllowed(request.origin)) {
		
	}

	console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
	var connection = request.accept('echo-protocol', request.origin);
	console.log((new Date()) + ' Connected accepted.');
	connection.on('message', (message) => {
		if(message.type === 'utf8'){
			console.log('Received message: ' + message.utf8Data);
			connection.sendUTF(message.utf8Data);
		}

		else if(message.type === 'binary') {
			console.log('Received binary message of ' + message.binaryData.lenth + ' bytes');
			connection.sendBytes(message.binaryData);
		}
	});

	connection.on('close', function(reasonCode, description) {
		console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');
	});
});

function originIsAllowed(origin) {
	return true;
}
