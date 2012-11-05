import qbs.base 1.0

Product {
    name: "vreen"

    property string versionMajor: project.versionMajor
    property string versionMinor: project.versionMinor
    property string versionRelease: project.versionRelease
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
    cpp.visibility: "hidden"

    files: [
        "api/*.cpp",
    ]

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "network", "gui"] }
    Depends { name: "k8json"}

    Properties {
        condition: qbs.toolchain !== 'msvc'
        cpp.cxxFlags: base.concat([ "-std=c++11" ])
    }
    Properties {
        condition: qbs.targetOS === "mac"
        cpp.cxxFlags: base.concat([ "-stdlib=libc++" ])
    }


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
            product.buildDirectory + "/include/vreen/" +  project.version
        ]
    }
}
