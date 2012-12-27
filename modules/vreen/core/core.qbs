import qbs.base 1.0

Module {
    property string libDestination: {
        if (qbs.targetOS === 'windows')
            return "bin";
        else
            return "lib";
    }
    property string qmlDestination: "bin"

    cpp.cxxFlags: base
    cpp.defines: base

    Depends { name: "cpp" }
    Depends { name: "qt.core"}

    Properties {
        condition: qbs.toolchain != 'msvc'
        cpp.cxxFlags: outer.concat([ "-std=c++11" ])
    }
    Properties {
        condition: qbs.targetOS == "mac"
        cpp.cxxFlags: outer.concat([ "-stdlib=libc++" ])
    }

    setupRunEnvironment: {
        putenv('QML2_IMPORT_PATH', product.buildDirectory + "/" + qmlDestination);
    }
}