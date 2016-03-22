import qbs.base

Product {
    name: "vreen"

    property string versionMajor: vreen_version_major
    property string versionMinor: vreen_version_minor
    property string versionRelease: vreen_version_path
    property string version: versionMajor + '.' + versionMinor + '.' + versionRelease

    destinationDirectory: vreen_libexec_path
    type: ["dynamiclibrary", "installed_content"]

    //cpp.warningLevel: "all"
    cpp.includePaths: [
        "3rdparty",
        "api"
    ]
    cpp.defines: [
        "VK_LIBRARY",
    ]
    cpp.positionIndependentCode: true
    cpp.visibility: ["hidden"]

    files: [
        "api/*.cpp",
    ]

    qbsSearchPaths: "../"

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "network", "gui"] }
    Depends { name: "vreen.core" }

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core", "network", "gui"] }

        cpp.includePaths: [
            product.buildDirectory + "/GeneratedFiles/include",
            product.buildDirectory + "/GeneratedFiles/include/vreen",
            product.buildDirectory + "/GeneratedFiles/include/vreen/" + product.version
        ]
        cpp.rpaths: product.buildDirectory + "/" + project.vreen_lib_path
        cpp.cxxLanguageVersion: "c++11"
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
        qbs.installDir: "include/vreen/" + product.version + "/vreen/private"
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
        fileTagsFilter: ['dynamiclibrary', 'dynamiclibrary_symlink']
        qbs.install: true
        qbs.installDir: project.vreen_lib_path
    }

    Group {
        name: "Other files"

        prefix: "../"
        files: [
            "AUTHORS",
            "COPYING",
            "ChangeLog",
            "LICENSE.md",
            "README.md",
        ]
    }
}
