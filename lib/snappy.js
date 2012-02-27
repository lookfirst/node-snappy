var snappy;

try {
  snappy = require('../build/default/binding');
} catch(e) {
  snappy = require('../build/Release/binding');
}

exports.compress = function(buffer, cb) {
  snappy.compress(buffer, cb);
};

exports.decompress = function(buffer, cb) {
  snappy.uncompress(buffer, cb);
};
