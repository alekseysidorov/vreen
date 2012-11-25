import qbs.base 1.0
import "../../qbs/VreenProduct.qbs" as VreenProduct

VreenProduct {
    type: ["staticlibrary", "installed_content"]
    name: "vreenoauth"
    destination: "lib"

    //cpp.warningLevel: "all"
    cpp.defines: []

    Depends { name: "cpp" }
    Depends { name: "qt"; submodules: ['core', 'network', 'webkit'] }
    Depends { name: "vreen"; }

    files: [
        "*.cpp"
    ]

    Group {
        qbs.installDir: "include/vreen/auth"
        overrideTags: false
        fileTags: ["install"]
        files: ["*.h"]
    }
    Properties {
        condition: qt.core.versionMajor === 5
        cpp.defines: outer.concat("QT_DISABLE_DEPRECATED_BEFORE=0")
    }

    ProductModule {
        Depends { name: "cpp" }
        Depends { name: "qt.webkit"}
    }
}

