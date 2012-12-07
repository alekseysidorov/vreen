import qbs.base 1.0

Product {
    property string appViewerPath: localPath + "/common/"
    condition: qt.core.versionMajor === 5

    type: [qbs.targetOS == 'mac' ? "applicationbundle" : "application", "installed_content"]

    Depends { name: "qt"; submodules: ["core", "quick"] }
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
