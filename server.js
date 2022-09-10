const fs = require('fs');
const { SerialPort, ReadlineParser } = require('serialport');

const parser = new ReadlineParser();
const port = new SerialPort({ path: '/COM4', baudRate: 9600 });
port.pipe(parser);
// Read the port data
port.open(function (err) {
  let index = 0;
  const coords = JSON.parse(fs.readFileSync('./data.json', 'utf-8'));

  setTimeout(() => {
    passNextCoord();
  }, 7000);

  parser.on('data', function (data) {
    console.log('receiving' + data);
    if (data.trim() === 'done') {
      passNextCoord();
    }
  });

  function passNextCoord() {
    const nextCoords = `${coords[index].x},${coords[index].y}`;
    console.log('writing' + nextCoords);
    port.write(nextCoords + '\n');

    if (index !== coords.length - 1) {
      index++;
    }
  }
});
