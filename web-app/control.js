const socket = io();

const startBtn = document.getElementById('startBtn');
const stopBtn = document.getElementById('stopBtn');
const speed = document.getElementById('speed');
const speedVal = document.getElementById('speedVal');
const color = document.getElementById('color');
const message = document.getElementById('message');
const sendMsg = document.getElementById('sendMsg');
const pattern = document.getElementById('pattern');
const status = document.getElementById('status');

socket.on('connect', () => { status.innerText = 'Status: connected'; });
socket.on('disconnect', () => { status.innerText = 'Status: disconnected'; });

startBtn.addEventListener('click', () => {
  socket.emit('command', 'START');
});
stopBtn.addEventListener('click', () => {
  socket.emit('command', 'STOP');
});

speed.addEventListener('input', () => {
  speedVal.innerText = speed.value;
  socket.emit('command', `SPEED:${speed.value}`);
});

color.addEventListener('input', () => {
  const hex = color.value;
  const r = parseInt(hex.substr(1,2), 16);
  const g = parseInt(hex.substr(3,2), 16);
  const b = parseInt(hex.substr(5,2), 16);
  socket.emit('command', `COLOR:${r},${g},${b}`);
});

sendMsg.addEventListener('click', () => {
  const msg = message.value.trim();
  if (msg.length > 0) {
    socket.emit('command', `MSG:${msg}`);
    message.value = '';
  }
});

pattern.addEventListener('change', () => {
  const p = pattern.value.toUpperCase();
  if (p === 'NONE') socket.emit('command', 'PATTERN:NONE');
  else socket.emit('command', `PATTERN:${p}`);
});
