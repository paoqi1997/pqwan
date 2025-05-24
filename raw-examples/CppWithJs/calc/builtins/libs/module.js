'use strict';

const { loader } = builtins;

const map = {};

class Module
{
    constructor(filename) {
        this.filename = filename;
        this.exports = {};
    }

    selfLoad() {
        const result = loader.compile(this.filename);

        // function(require, exports, module) {
        //   index.js 文件的内容
        // }
        result.call(this, Module.load, this.exports, this);

        return this.exports;
    }

    static load(filename, ...args) {
        if (map[filename]) {
            return map[filename];
        }

        const module = new Module(filename, ...args);

        const exports = (map[filename] = module.selfLoad());

        PRINT(`exports:${filename}: ${JSON.stringify(exports)}`);

        return exports;
    }
}

module.exports = Module;
