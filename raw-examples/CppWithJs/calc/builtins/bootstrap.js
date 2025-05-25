'use strict';

const { loader } = builtins;

function loadNativeModules() {
    builtins.libs = {};

    const modules = [
        {
            path: 'builtins/libs/module.js',
            name: 'module'
        },
    ];

    for (let i = 0; i < modules.length; ++i) {
        const module = {
            exports: {},
        };

        // function(require, exports, module) {
        //   module.js 文件的内容
        // }
        loader.compile(modules[i].path).call(null, loader.compile, module.exports, module);

        // module.exports = Module;
        builtins.libs[modules[i].name] = module.exports;
    }
}

function runMain() {
    builtins.libs.module.load('index.js');
    return 'finished';
}

loadNativeModules();
runMain();
