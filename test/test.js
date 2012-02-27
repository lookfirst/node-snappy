var Snappy = require('../lib/snappy');
var Assert = require('assert');

test = function(buffer) {
  console.log('test: ' + buffer);

  Snappy.compress(buffer, function(err, compressed){
    Assert.ifError(err);

    Snappy.decompress(compressed, function(err, decompressed){
      Assert.ifError(err);
      Assert.equal(decompressed.toString(), buffer.toString());
    });
  });
}

test(new Buffer('spasspur'));
test(new Buffer([255, 200, 100, 3, 0, 256, 80]));
