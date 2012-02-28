This is a fork of kesla's node-snappy. Simple async binding and no more :)

# node-snappy [![Build Status](https://secure.travis-ci.org/Skomski/node-snappy.png?branch=master)](http://travis-ci.org/Skomski/node-snappy)

Node.js bindings for Google's fast compressor/decompressor: <http://code.google.com/p/snappy/>

## Install

```
npm install snappy
```

## Simple Benchmark

```
Testfile:
  https://raw.github.com/Skomski/node-snappy/master/test/urls.10K
  size: 702087 bytes

Compression:
  snappy: 357267 bytes
  zlib(level:1): 253263 bytes

Speed:
  zlib x 68.30 ops/sec ±1.52% (56 runs sampled)
  snappy x 361 ops/sec ±0.94% (61 runs sampled)
```

## Usage

```javascript

var Snappy = require('snappy');
var buffer = new Buffer([255, 200, 100, 3, 0, 256, 80]);

Snappy.compress(buffer, function(err, compressed){
  if (err) throw err;

  Snappy.decompress(compressed, function(err, decompressed){
    if (err) throw err;
  });
});
```

## Methods

### compress(buffer, cb)
  * Required:
    * `buffer` - Decompressed buffer
    * `cb` - Function with two arguments `(err, compressedBuffer)`

### decompress(buffer, cb)
  * Required:
    * `buffer` - Compressed buffer
    * `cb` - Function with two arguments `(err, decompressedBuffer)`

## License

Licensed under the MIT license.
