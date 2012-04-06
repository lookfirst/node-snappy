var Benchmark = require('benchmark');
var Fs = require('fs')
var Snappy = require('..')
var Zlib   = require('zlib')

var suite = new Benchmark.Suite;

var buffer = Fs.readFileSync(__dirname + '/../test/urls.10K');

var FastDeflate = Zlib.createDeflate({
  level: 1,
  memLevel: 8
});

var deflateLength = 0;

FastDeflate.on('data', function(chunk) {
  deflateLength += chunk.length;
});

FastDeflate.on('end', function() {
  console.log('zlib: ' + deflateLength + ' bytes');
});

FastDeflate.write(buffer);
FastDeflate.end();

Snappy.compress(buffer, function(err, result) {
  console.log('snappy: ' + result.length + ' bytes');
});

suite.add('zlib', function(deferred) {
  var result = new Buffer(253263);
  var position = 0;
  var FastDeflate = Zlib.createDeflate({
    level: 1,
    memLevel: 8,
    chunkSize: 253263
  });
  FastDeflate.on('data', function(chunk) {
    chunk.copy(result, position);
    position += chunk.length;
  });
  FastDeflate.on('end', function() {
    deferred.resolve();
  });
  FastDeflate.write(buffer);
  FastDeflate.end();
}, { defer: true, minSamples: 2000, maxTime: 30 });

suite.add('snappy', function(deferred) {
  Snappy.compress(buffer, function(err, result) {
    if (err) throw err;
    deferred.resolve();
  });
},{ defer: true, minSamples: 2000, maxTime: 30 });

suite.on('cycle', function(event, bench) {
  console.log(String(bench));
})
.on('complete', function() {
  console.log('Fastest is ' + this.filter('fastest').pluck('name'));
})
.run({ async: false });
