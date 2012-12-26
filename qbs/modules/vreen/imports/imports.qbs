import qbs.base 1.0

Module {
    setupRunEnvironment: {
        putenv('QML2_IMPORT_PATH', destination);
    }
}
