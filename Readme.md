# snappy [![Build Status](https://secure.travis-ci.org/Skomski/node-snappy.png?branch=master)](http://travis-ci.org/Skomski/node-snappy)

Node.js bindings for Google's fast compressor/decompressor: <http://code.google.com/p/snappy/>

## Install

```
npm install snappy
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
    * `cb` - Function with two arguments `(err, uncompressedBuffer)`

## License

Licensed under the MIT license.
