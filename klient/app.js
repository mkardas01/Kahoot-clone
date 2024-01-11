const express = require('express');
const http = require('http');
const net = require('net');
const path = require('path');
const WebSocket = require('ws');
const { EventEmitter } = require('events');
const SERVER_IP = "192.168.18.48";
const PORT_VAR = 41119;

class serverStatus extends EventEmitter {
    constructor() {
        super();
        this._value = null;
    }

    get value() {
        return this._value;
    }

    set value(newValue) {
        if (this._value !== newValue) {
            this._value = newValue;
            this.emit('change', newValue);
        }
    }
}

let serverStatusVar = new serverStatus();
serverStatus.value = null;

const app = express();

app.use('/', express.static(path.join(__dirname, 'public')));

const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// Utwórz połączenie socketowe do serwera C++
let server_socket;
// Ustaw nasłuchiwanie połączeń WebSocket

wss.on('connection', (ws) => {
    console.log('JavaScript client connected');

    // Generuj unikalny identyfikator dla klienta WebSocket

    serverStatusVar.on('change', () => {
        ws.send(`${JSON.stringify({ type: 'serverStatus', serverStatus: serverStatusVar.value })}\n\n`);
    });

    // Obsługuj wiadomości od klienta JavaScript
    ws.on('message', (message) => {
        console.log('Received from JavaScript client through WebSocket:', message);
        
        // Prześlij wiadomość do serwera C++
        server_socket.write(message);
    });

    // Obsługuj zakończenie połączenia z klientem JavaScript
    ws.on('close', () => {
        console.log('JavaScript client disconnected');
    });
});


function connectToCppServer() {
    server_socket = net.createConnection({ port: PORT_VAR, host: SERVER_IP }, () => {
        console.log('Connected to C++ server');
        serverStatusVar.value = true;
    });

    // Obsługuj dane od serwera C++
    server_socket.on('data', (data) => {
        const messageFromCplusplus = data.toString();
        console.log('Received from C++ server:', messageFromCplusplus);

        // Wysyłaj otrzymane dane do klienta JavaScript przez WebSocket
        wss.clients.forEach((client) => {
            if (client.readyState === WebSocket.OPEN) {
                client.send(messageFromCplusplus);
            }
        });
    });

    // Obsługuj zakończenie połączenia z serwerem C++
    server_socket.on('end', () => {
        console.log('Disconnected from C++ server');
        serverStatusVar.value = false;

        // Spróbuj ponownie połączyć się z serwerem C++ po krótkim czasie
        setTimeout(connectToCppServer, 1000);
    });

    // Obsłuż błędy podczas łączenia z serwerem C++
    server_socket.on('error', (err) => {
        console.error('Error connecting to C++ server:', err.message);
        // Spróbuj ponownie połączyć się z serwerem C++ po krótkim czasie
        setTimeout(connectToCppServer, 1000);
    });
}

connectToCppServer()

// Serwuj plik HTML
app.post('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public/index.html'));
});

// Uruchom serwer HTTP na porcie 3000
const PORT = 3000;
server.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
    
    var url = 'http://localhost:3000';
    var start = (process.platform == 'darwin'? 'open': process.platform == 'win32'? 'start': 'xdg-open');
    require('child_process').exec(start + ' ' + url);

});
