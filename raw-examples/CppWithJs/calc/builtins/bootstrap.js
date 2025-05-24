'use strict';

const { loader } = builtins;

function loadInnerModules() {
    builtins.libs = {};

    const modules = [
        {
            module: 'builtins/libs/module.js',
            name: 'module'
        },
    ];

    for (let i = 0; i < modules.length; ++i) {
        const module = {
            exports: {},
        };

        // function(require, exports, module)
        loader.compile(modules[i].module).call(null, loader.compile, module.exports, module);

        builtins.libs[modules[i].name] = module.exports;
    }
}

function runMain() {
    builtins.libs.module.load('index.js');
    return 'finished';
}

loadInnerModules();
runMain();
