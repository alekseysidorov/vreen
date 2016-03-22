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
        name: "Qml files"
        prefix: "qml/**"
        files: "*"

        qbs.install: true
        qbs.installDir: destinationDirectory + "/qml"
        qbs.installSourceBase: "qml"
    }

    Group {
        qbs.install: true
        qbs.installDir: destinationDirectory
        fileTagsFilter: "application"
    }
}
