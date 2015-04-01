import qbs.base 1.0

Product {
    property string appViewerPath: "../common/"

    condition: Qt.core.versionMajor === 5

    type: ["application", "installed_content"]

    Depends { name: "Qt.core" }
    Depends { name: "Qt.quick" }
    Depends { name: "Qt.widgets" }
    Depends { name: "cpp" }

    cpp.includePaths: [appViewerPath]

    Group {
        prefix: appViewerPath
        files: [
            "declarativeview.h",
            "declarativeview.cpp",
            "main.cpp"
        ]
    }

    Group {
        name: "qml_files"
        prefix: "**/"
        files: ["*.qml"]
    }

    Group {
        qbs.install: true
        qbs.installDir: destinationDirectory
        fileTagsFilter: "application"
    }
}
