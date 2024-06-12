module.exports = {
    testEnvironment: 'node',
    moduleNameMapper: {
        '\\.(css)$': 'identity-obj-proxy',
    },
    setupFilesAfterEnv: ['<rootDir>/jest.setup.ts'],
};
