import qbs.base 1.0

Product {
    property string appViewerPath: "../common/"

    type: ["application", "installed_content"]

    Depends { name: "Qt.core" }
    Depends { name: "Qt.quick" }
    Depends { name: "Qt.widgets" }
    Depends { name: "cpp" }

    cpp.includePaths: [appViewerPath]

    Group {
        name: "declarativeView"
        prefix: appViewerPath
        files: [
            "declarativeview.h",
            "declarativeview.cpp",
            "main.cpp"
        ]
    }

    Group {
        name: "qml files"
        prefix: "qml/**"
        files: "*"
    }

    Group {
        name: "Install files"
        files: "qml"
        qbs.install: true
        qbs.installDir: destinationDirectory
    }

    Group {
        qbs.install: true
        qbs.installDir: destinationDirectory
        fileTagsFilter: "application"
    }
}
