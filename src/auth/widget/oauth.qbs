import qbs.base 1.0

Product {
    property bool

    type: ["staticlibrary"]
    name: "vreenoauth"
    destinationDirectory: project.vreen_lib_path

    Depends { name: "cpp" }
    Depends { name: "Qt.core" }

    Properties {
        condition: !project.with_webkit
        cpp.defines: base.concat("VREEN_WITH_WEBENGINE")
    }

    Depends { name: "Qt.webengine"; condition: !project.with_webkit }
    Depends { name: "Qt.webenginewidgets"; condition: !project.with_webkit  }
    Depends { name: "Qt.webkitwidgets"; condition: project.with_webkit  }


    Depends { name: "vreencore" }
    Depends { name: "vreen" }

    files: [
        "*.cpp"
    ]

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt.core" }

        Depends { name: "Qt.webengine"; condition: !project.with_webkit }
        Depends { name: "Qt.webenginewidgets"; condition: !project.with_webkit  }
        Depends { name: "Qt.webkitwidgets"; condition: project.with_webkit  }

        cpp.includePaths: [
            product.buildDirectory + "/GeneratedFiles/include"
        ]
        cpp.defines: base.concat("VREEN_WITH_OAUTH")
    }

    Group {
        name: "Devel headers"
        files: "*.h"
        fileTags: ["hpp", "devheader"]
        overrideTags: false
        qbs.installDir: "include/vreen/auth"
        qbs.installSourceBase: "."
        qbs.install: true
    }
}

