const appdmg = require('appdmg');

const appBundlePath = process.argv[2];
const bridgePkgPath = process.argv[3];
const dmgPath = process.argv[4];

appdmg({
    target: dmgPath,
    basepath: __dirname,
    specification: {
        title: 'DiffScope',
        background: 'diffscope-background.png',
        'icon-size': 128,
        contents: [
            {x: 468, y: 292, type: 'link', path: '/Applications'},
            {x: 132, y: 292, type: 'file', path: appBundlePath},
            {x: 132, y: 476, type: 'file', path: bridgePkgPath},
            {x: 94, y: 700, type: 'position', path: '.background'}
        ]
    }
});

