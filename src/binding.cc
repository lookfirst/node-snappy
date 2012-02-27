#include <snappy.h>
#include <node_buffer.h>

using namespace v8;

Handle<Value> compress(const Arguments& args) {
  HandleScope scope;

  Local<Object> input = args[0]->ToObject();
  size_t length = node::Buffer::Length(input);
  char *data = node::Buffer::Data(input);
  std::string dst;

  snappy::Compress(data, length, &dst);

  Local<Function> cb = Local<Function>::Cast(args[1]);
  Handle<Object> buffer = node::Buffer::New(String::New(dst.c_str(), dst.length()));
  Handle<Value> argv[2] = { Local<Value>::New(Null()), buffer };
  cb->Call(Context::GetCurrent()->Global(), 2, argv);

  return scope.Close(Undefined());
}

Handle<Value> uncompress(const Arguments& args) {
  HandleScope scope;

  Local<Object> input = args[0]->ToObject();
  size_t length = node::Buffer::Length(input);
  char *data = node::Buffer::Data(input);
  std::string dst;

  Local<Function> cb = Local<Function>::Cast(args[1]);
  Handle<Value> argv[2];

  if (snappy::Uncompress(data, length, &dst)) {
    argv[0] = Local<Value>::New(Null());
    argv[1] = node::Buffer::New(String::New(dst.c_str(), dst.length()));
  } else {
    argv[0] = Exception::Error(
      String::New("Buffer could not be decompressed")
    );
    argv[1] = Local<Value>::New(Null());
  }

  cb->Call(Context::GetCurrent()->Global(), 2, argv);

  return scope.Close(Undefined());
}

void Init(Handle<Object> target) {
  target->Set(String::NewSymbol("compress"),
    FunctionTemplate::New(compress)->GetFunction());
  target->Set(String::NewSymbol("uncompress"),
    FunctionTemplate::New(uncompress)->GetFunction());
}

NODE_MODULE(binding, Init)
