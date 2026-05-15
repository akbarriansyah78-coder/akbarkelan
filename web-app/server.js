// Simple backend server that forwards socket commands to Arduino over serial

const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
require('dotenv').config();

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const SERIAL_PORT = process.env.SERIAL_PORT || '/dev/ttyACM0';
const BAUD = parseInt(process.env.BAUD || '115200');

let serial;
try {
  serial = new SerialPort(SERIAL_PORT, { baudRate: BAUD });
  const parser = serial.pipe(new Readline({ delimiter: '\r\n' }));
  parser.on('data', line => console.log('ARDUINO>', line));
  console.log('Opened serial', SERIAL_PORT, 'at', BAUD);
} catch (e) {
  console.warn('Could not open serial port', SERIAL_PORT, e.message);
}

app.use(express.static(__dirname));

io.on('connection', (socket) => {
  console.log('Client connected');
  socket.on('command', (cmd) => {
    console.log('CMD from client:', cmd);
    if (serial && serial.isOpen) {
      serial.write(cmd + '\n');
    }
  });
});

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => console.log(`Server running at http://localhost:${PORT}`));
