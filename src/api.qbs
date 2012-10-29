import qbs.base 1.0

Product {
    name: "vreen"

    property string versionMajor: '0'
    property string versionMinor: '9'
    property string versionRelease: '90'
    property string version: versionMajor + '.' + versionMinor + '.' + versionRelease

    destination: {
        if (qbs.targetOS === 'windows')
            return "bin";
        else
            return "lib";
    }
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
    cpp.cxxFlags: {
        var flags = []
        if (qbs.toolchain !== "msvc") {
            flags.push("-std=c++0x")
        }
        return flags
    }
    cpp.visibility: "hidden"

    files: [
        "api/*.cpp",
    ]

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "network", "gui"] }
    Depends { name: "k8json"}

    Group {
        qbs.installDir: "include/vreen/" + version + "/vreen/private"
        fileTags: ["install"]
        overrideTags: false
        files: [
            "api/*_p.h",
            "api/draft/*_p.h",
            "api/draft/*.h"
        ]        
    }
    
    Group {
        qbs.installDir: "include/vreen"
        fileTags: ["install"]
        overrideTags: false
        files: [
            "api/*[^_][a-z].h",
        ]
    }
    ProductModule {
        Depends { name: "cpp" }
        cpp.includePaths: [
            product.buildDirectory + "/include",
            product.buildDirectory + "/include/vreen",
        ]
    }
}
