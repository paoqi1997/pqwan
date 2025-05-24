#ifndef JS_ENV_H
#define JS_ENV_H

#include <memory>

#include <libplatform/libplatform.h>
#include <v8.h>

class JsEnv
{
public:
    static JsEnv* getInstance() {
        if (instance == nullptr) {
            instance = new JsEnv();
        }
        return instance;
    }
    JsEnv(const JsEnv&) = delete;
    JsEnv& operator = (const JsEnv&) = delete;
    bool run();
private:
    JsEnv();
    ~JsEnv();
    static JsEnv *instance;
    std::unique_ptr<v8::Platform> plat;
    v8::Isolate::CreateParams createParams;
    v8::Isolate *isolate;
};

#endif // JS_ENV_H
