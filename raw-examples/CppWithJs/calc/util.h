#ifndef UTIL_H
#define UTIL_H

#include <string>

#include <v8.h>

std::string getFileContent(const std::string& filePath);

v8::Local<v8::String> ToV8String(const std::string& s);
void jsCompile(const v8::FunctionCallbackInfo<v8::Value>& args);
void jsPrint(const v8::FunctionCallbackInfo<v8::Value>& args);

#endif // UTIL_H
