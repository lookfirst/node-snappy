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

test(new Buffer('spppppppppassssssssssssssspuuuuuuuuuuur!!___"DSA'));
test(new Buffer([255, 200, 100, 3, 0, 256, 80]));
test(new Buffer('काक्नोम्यत्क्नोम्यत्चं शक्नोम्यत्तुमतुम् ।तुम् ।् । नोपहिनस्ति माम् ॥'));
test(new Buffer(JSON.stringify({
  status: 404,
  errorName: 'NoSuchBucket',
  errorMessage: 'The specified bucket does not exist.',
  uuid: '550e8400-e29b-41d4-a716-4466554400002'
})));
test(Fs.readFileSync(__dirname + '/urls.10K'));

process.on('exit', function() {
  Assert.equal(tests, 5);
});
