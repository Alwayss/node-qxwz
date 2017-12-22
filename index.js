const qxwz = require('./build/release/qxwz');

const QXWZ = {};
QXWZ.start = qxwz.start;
QXWZ.sendGGA = qxwz.sendGGA;
QXWZ.stop = stop;

module.exports = QXWZ;