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

std::string JsEnv::runJsFile(const std::string& filePath)
{
    std::string fileContent = getFileContent(filePath);
    if (fileContent.length() == 0) {
        return "";
    }

    return runScriptString(fileContent.c_str());
}

std::string JsEnv::runScriptString(const char* scriptString)
{
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Local<v8::Context> context = v8::Context::New(isolate);
    v8::Context::Scope contextScope(context);

    v8::Local<v8::String> scriptStringObject = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), scriptString).ToLocalChecked();
    v8::Local<v8::Script> scriptObject = v8::Script::Compile(context, scriptStringObject).ToLocalChecked();
    v8::Local<v8::Value> result = scriptObject->Run(context).ToLocalChecked();
    v8::String::Utf8Value utf8(isolate, result);

    return *utf8;
}
