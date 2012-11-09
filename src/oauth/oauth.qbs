import qbs.base 1.0

Product {
    type: ["staticlibrary", "installed_content"]
    name: "vreenoauth"
    destination: "lib"

    //cpp.warningLevel: "all"

    Depends { name: "cpp" }
    Depends { name: "qt"; submodules: ['core', 'network', 'webkit'] }
    Depends { name: "vreen"; }

    files: [
        "*.cpp"
    ]

    Properties {
        condition: qbs.toolchain !== 'msvc'
        cpp.cxxFlags: base.concat([ "-std=c++11" ])
    }
    Properties {
        condition: qbs.targetOS === "mac"
        cpp.cxxFlags: base.concat([ "-stdlib=libc++" ])
    }

    Group {
        qbs.installDir: "include/vreen/auth"
        overrideTags: false
        fileTags: ["install"]
        files: ["*.h"]
    }

    ProductModule {
        Depends { name: "cpp" }
        Depends { name: "qt.webkit"}
    }
}

