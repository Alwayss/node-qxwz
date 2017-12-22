const qxwz = require('./build/Release/qxwz');

const QXWZ = {};
QXWZ.start = qxwz.start;
QXWZ.sendGGA = qxwz.sendGGA;
QXWZ.stop = qxwz.stop;

module.exports = QXWZ;