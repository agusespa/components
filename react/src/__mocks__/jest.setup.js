const { JSDOM } = require('jsdom');

const jsdomConfig = {
  url: 'http://localhost',
};

const jsdom = new JSDOM('<!doctype html><html><body></body></html>', jsdomConfig);
const { window } = jsdom;

global.window = window;
global.document = window.document;
global.navigator = {
  userAgent: 'node.js',
};

