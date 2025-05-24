#include <iostream>

#include "JsEnv.h"
#include "util.h"

JsEnv *JsEnv::instance = nullptr;

JsEnv::JsEnv()
{
    plat = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(plat.get());
    v8::V8::Initialize();

    createParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = v8::Isolate::New(createParams);
}

JsEnv::~JsEnv()
{
    isolate->Dispose();

    v8::V8::Dispose();
    v8::V8::DisposePlatform();

    delete createParams.array_buffer_allocator;
}

bool JsEnv::run()
{
    std::string fileContent = getFileContent("builtins/bootstrap.js");
    if (fileContent.length() == 0) {
        return false;
    }

    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    // PRINT
    global->Set(isolate, "PRINT", v8::FunctionTemplate::New(isolate, jsPrint));

    v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global);
    v8::Context::Scope contextScope(context);

    v8::Local<v8::ObjectTemplate> loader = v8::ObjectTemplate::New(isolate);
    // loader.compile
    loader->Set(v8::String::NewFromUtf8(isolate, "compile").ToLocalChecked(), v8::FunctionTemplate::New(isolate, jsCompile));

    v8::Local<v8::Object> builtins = v8::Object::New(isolate);
    // builtins.loader
    builtins->Set(
        isolate->GetCurrentContext(),
        v8::String::NewFromUtf8(isolate, "loader").ToLocalChecked(),
        loader->NewInstance(isolate->GetCurrentContext()).ToLocalChecked()
    );

    v8::Local<v8::Object> globalObject = context->Global();
    // builtins
    globalObject->Set(context, v8::String::NewFromUtf8Literal(isolate, "builtins", v8::NewStringType::kNormal), builtins);

    const char *scriptString = fileContent.c_str();

    v8::Local<v8::String> scriptStringObject = v8::String::NewFromUtf8(isolate, scriptString).ToLocalChecked();
    v8::Local<v8::Script> scriptObject = v8::Script::Compile(context, scriptStringObject).ToLocalChecked();
    v8::Local<v8::Value> result = scriptObject->Run(context).ToLocalChecked();
    v8::String::Utf8Value utf8(isolate, result);

    std::cout << *utf8 << std::endl;

    return true;
}
