import qbs.base 1.0

Product {
    property string appViewerPath: "common/"

    condition: Qt.core.versionMajor === 5

    type: ["application", "installed_content"]

    Depends { name: "Qt.core" }
    Depends { name: "Qt.quick"; condition: Qt.core.versionMajor === 5 }
    Depends { name: "cpp" }

    cpp.includePaths: [appViewerPath]

    Group {
        prefix: appViewerPath
        files: [
            "*.cpp",
            "*.h"
        ]
    }
}
