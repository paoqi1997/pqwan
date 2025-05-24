#include <fstream>
#include <iostream>
#include <sstream>

#include "util.h"

std::string getFileContent(const std::string& filePath)
{
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        std::cout << filePath << " is not open." << std::endl;
        return "";
    }

    std::stringstream inBuffer;
    inBuffer << inFile.rdbuf();

    return inBuffer.str();
}

v8::Local<v8::String> ToV8String(const std::string& s)
{
    return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), s.c_str()).ToLocalChecked();
}

void jsCompile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    v8::String::Utf8Value filePath(isolate, args[0].As<v8::String>());

    std::string fileContent = getFileContent(*filePath);

    v8::ScriptCompiler::Source source(v8::String::NewFromUtf8(isolate, fileContent.c_str()).ToLocalChecked());

    v8::Local<v8::String> params[] = {
        ToV8String("require"),
        ToV8String("exports"),
        ToV8String("module"),
    };

    v8::MaybeLocal<v8::Function> func = v8::ScriptCompiler::CompileFunction(context, &source, 3, params, 0, nullptr);

    if (func.IsEmpty()) {
        args.GetReturnValue().Set(v8::Undefined(isolate));
    } else {
        args.GetReturnValue().Set(func.ToLocalChecked());
    }
}

void jsPrint(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    int length = args.Length();

    for (int i = 0; i < length; ++i) {
        v8::String::Utf8Value utf8(args.GetIsolate(), args[i]);

        std::cout << *utf8;

        if (i != length - 1) {
            std::cout << " ";
        }
    }

    std::cout << std::endl;
}
