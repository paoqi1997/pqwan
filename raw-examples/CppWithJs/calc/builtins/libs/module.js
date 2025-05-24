'use strict';

const { loader } = builtins;

const map = {};

class Module
{
    constructor(filename) {
        this.filename = filename;
        this.exports = {};
    }

    load() {
        const result = loader.compile(this.filename);
        // function(require, exports, module)
        result.call(this, Module.load, this.exports, this);
        return this.exports;
    }

    static load(filename, ...args) {
        if (map[filename]) {
            return map[filename];
        }

        const module = new Module(filename, ...args);

        const r = (map[filename] = module.load());

        PRINT(`R:${filename}: ${JSON.stringify(r)}`);

        return r;
    }
}

module.exports = Module;
