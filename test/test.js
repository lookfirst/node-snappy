var Snappy = require('..');
var Assert = require('assert');
var Fs     = require('fs');

tests = 0;

test = function(buffer) {
  Snappy.compress(buffer, function(err, compressed){
    Assert.ifError(err);

    Snappy.decompress(compressed, function(err, decompressed){
      Assert.ifError(err);
      Assert.equal(decompressed.toString(), buffer.toString());
      tests++
    });
  });
}

test(new Buffer('spasspur'));
test(new Buffer([255, 200, 100, 3, 0, 256, 80]));
test(new Buffer('काचं शक्नोम्यत्तुम् । नोपहिनस्ति माम् ॥'));
test(new Buffer(JSON.stringify({
  status: 500,
  errorName: 'NoSuchBucket',
  errorMessage: 'The specified bucket does not exist.',
  uuid: '2231231283129938219383912'
})));
test(Fs.readFileSync(__dirname + '/urls.10K'));

process.on('exit', function() {
  Assert.equal(tests, 5);
});
