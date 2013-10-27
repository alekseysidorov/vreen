import qbs.base 1.0

Product {
    name: "vreen"

    property string versionMajor: project.vreen_version_major
    property string versionMinor: project.vreen_version_minor
    property string versionRelease: project.vreen_version_path
    //property string version: versionMajor + '.' + versionMinor + '.' + versionRelease

    destinationDirectory: project.vreen_lib_destination
    type: ["dynamiclibrary", "installed_content"]

    //cpp.warningLevel: "all"
    cpp.includePaths: [
        "3rdparty",
        "api"
    ]
    cpp.defines: [
        "VK_LIBRARY",
        "K8JSON_INCLUDE_GENERATOR",
        "K8JSON_INCLUDE_COMPLEX_GENERATOR"
    ]
    cpp.positionIndependentCode: true
    cpp.visibility: ["hidden"]

    files: [
        "api/*.cpp",
    ]

    Properties {
        condition: Qt.core.versionMajor === 5
        cpp.defines: outer.concat("QT_DISABLE_DEPRECATED_BEFORE=0")
    }

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "network", "gui"] }
    Depends { name: "k8json" }
    Depends { name: "vreen.core" }

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core", "network", "gui"] }
        Depends { name: "vreen.core" }

        cpp.includePaths: [
            product.buildDirectory + "/GeneratedFiles/include",
            product.buildDirectory + "/GeneratedFiles/include/vreen",
            product.buildDirectory + "/GeneratedFiles/include/vreen/" +  vreen.core.version
        ]
        cpp.rpaths: product.buildDirectory + "/" + vreen.core.libDestination
    }

    Group {
        name: "Private Headers"

        files: [
            "api/*_p.h",
            "api/draft/*_p.h",
            "api/draft/*.h"
        ]
        fileTags: ["devheader"]
        overrideTags: false
        qbs.installDir: "include/vreen/" + vreen.core.version + "/vreen/private"
        qbs.install: true
    }

    Group {
        name: "Public Headers"

        files: "api/*.h"
        excludeFiles: "api/*_p.h"
        fileTags: ["devheader"]
        overrideTags: false
        qbs.installDir: "include/vreen"
        qbs.install: true
    }

    Group {
        fileTagsFilter: 'dynamiclibrary'
        qbs.install: true
        qbs.installDir: vreen.core.libDestination
    }
}
