const Camera = require('./camera.js');

const camera = new Camera(320, 240);

camera.add('ox');
camera.add('yx');
camera.add('oy');
camera.call();

console.log(
`ox: ${camera.ox()}
yx: ${camera.yx()}
oy: ${camera.oy()}`
)