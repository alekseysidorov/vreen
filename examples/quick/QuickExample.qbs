import qbs.base 1.0

Product {
    property string appViewerPath: localPath + "/common/"

    condition: qt.core.versionMajor === 5

    type: ["application", "installed_content"]

    Depends { name: "qt.core" }
    Depends { name: "qt.quick"; condition: qt.core.versionMajor === 5 }
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
