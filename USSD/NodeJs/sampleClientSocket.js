var WebSocket = require('websocket');

var webSocketClient = WebSocket.client;

var socketClient = new webSocketClient();

socketClient.on('connectFailed', (error)=>{
	console.log('Connect Error: ' + error.toString());
});

socketClient.on('connect', function(connection){
	console.log('Websocket Client Connected');
	connection.on('error', (error)=>{
		console.log('Connection Error: ' + error.toString());
	});

	connection.on('close',() =>{
		console.log('echo-protocol Connecton Closed');
	});

	connection.on('message', (message)=>{
		if(message.type === 'utf8'){
			console.log("Received: '" + message.utf8Data + "'");
		}
	});

	function sendNumber() {
		if(connection.connected) {
			var number = Math.round(Math.random() * 0xFFFFFF);
			connection.sendUTF(number.toString());
			setTimeout(sendNumber, 1000);
		}
	}

	sendNumber();
});

socketClient.connect('ws://localhost:8080/', 'echo-protocol');
