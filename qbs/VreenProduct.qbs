import qbs.base 1.0

Product {

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
}
