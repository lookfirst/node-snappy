// Copyright 2012 Karl Skomski MIT

#include <snappy.h>
#include <node_buffer.h>

namespace v8 {

  struct request {
    const char* input_data;
    size_t input_length;
    char* output_data;
    size_t output_length;
    Persistent<Function> callback;
  };

  void compress_work(uv_work_t *job) {
    request *req = static_cast<request*>(job->data);

    size_t max_length = snappy::MaxCompressedLength(req->input_length);

    req->output_data = new char[max_length];

    snappy::RawCompress(
        req->input_data,
        req->input_length,
        req->output_data,
        &req->output_length);
  }

  void compress_done(uv_work_t *job) {
    HandleScope scope;

    request *req = static_cast<request*>(job->data);

    Handle<Object> buffer = node::Buffer::New(
      req->output_data, req->output_length)->handle_;

    Handle<Value> argv[2] = { Local<Value>::New(Null()), buffer };
    TryCatch try_catch;
    req->callback->Call(Context::GetCurrent()->Global(), 2, argv);
    if (try_catch.HasCaught()) node::FatalException(try_catch);

    req->callback.Dispose();
    delete[] req->output_data;
    delete req;
    delete job;
  }

  Handle<Value> compress(const Arguments& args) {
    HandleScope scope;

    request *req = new request;

    Local<Object> input = args[0]->ToObject();
    req->input_length = node::Buffer::Length(input);
    req->input_data = node::Buffer::Data(input);
    req->callback = Persistent<Function>::New(Local<Function>::Cast(args[1]));

    uv_work_t *job = new uv_work_t;
    job->data = req;
    uv_queue_work(uv_default_loop(), job, compress_work, compress_done);

    return scope.Close(Undefined());
  }

  void uncompress_work(uv_work_t *job) {
    request *req = static_cast<request*>(job->data);

    if (!snappy::GetUncompressedLength(
          req->input_data,
          req->input_length,
          &req->output_length)) {
      req->output_length = -1;
      return;
    }

    req->output_data = new char[req->output_length];

    if (!snappy::RawUncompress(
          req->input_data,
          req->input_length,
          req->output_data)) {
      req->output_length = -1;
    }
  }

  void uncompress_done(uv_work_t *job) {
    HandleScope scope;

    request* req = static_cast<request*>(job->data);

    Handle<Value> argv[2];

    if (req->output_length != -1) {
      argv[0] = Local<Value>::New(Null());
      argv[1] = node::Buffer::New(
          req->output_data,
          req->output_length)->handle_;
    } else {
      argv[0] = Exception::Error(
        String::New("Buffer could not be decompressed"));
      argv[1] = Local<Value>::New(Null());
    }

    TryCatch try_catch;
    req->callback->Call(Context::GetCurrent()->Global(), 2, argv);
    if (try_catch.HasCaught()) node::FatalException(try_catch);

    req->callback.Dispose();
    delete[] req->output_data;
    delete req;
    delete job;
  }

  Handle<Value> uncompress(const Arguments& args) {
    HandleScope scope;

    request *req = new request;

    Local<Object> input = args[0]->ToObject();

    req->input_length = node::Buffer::Length(input);
    req->input_data = node::Buffer::Data(input);
    req->callback = Persistent<Function>::New(Local<Function>::Cast(args[1]));

    uv_work_t *uncompressJob = new uv_work_t;
    uncompressJob->data = req;
    uv_queue_work(
      uv_default_loop(),
      uncompressJob,
      uncompress_work,
      uncompress_done);

    return scope.Close(Undefined());
  }

  void Init(Handle<Object> target) {
    target->Set(String::NewSymbol("compress"),
      FunctionTemplate::New(compress)->GetFunction());
    target->Set(String::NewSymbol("uncompress"),
      FunctionTemplate::New(uncompress)->GetFunction());
  }

  NODE_MODULE(binding, Init)
}
