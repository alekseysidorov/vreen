import qbs.base 1.0

Product {
    type: ["staticlibrary"]
    name: "vreenoauth"
    destinationDirectory: project.vreen_lib_path

    //cpp.warningLevel: "all"

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ['core', 'network', 'webkitwidgets'] }
    Depends { name: "vreencore" }
    Depends { name: "vreen" }

    files: [
        "*.cpp"
    ]

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt.webkit"}
        Depends { name: "Qt.webkitwidgets"}

        cpp.includePaths: [
            product.buildDirectory + "/GeneratedFiles/include"
        ]
        cpp.defines: base.concat("VREEN_WITH_OAUTH")
    }

    Group {
        files: "*.h"
        fileTags: ["hpp", "devheader"]
        overrideTags: false
        qbs.installDir: "include/vreen/auth"
        qbs.install: true
    }
}

