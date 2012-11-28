import qbs.base 1.0
import "../qbs/VreenProduct.qbs" as VreenProduct

VreenProduct {
    name: "vreen"

    property string versionMajor:  0
    property string versionMinor: 9
    property string versionRelease: 5
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

    Properties {
        condition: qt.core.versionMajor === 5
        cpp.defines: outer.concat("QT_DISABLE_DEPRECATED_BEFORE=0")
    }

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
        Depends { name: "Qt"; submodules: ["core", "network", "gui"] }

        cpp.includePaths: [
            product.buildDirectory + "/include",
            product.buildDirectory + "/include/vreen",
            product.buildDirectory + "/include/vreen/" +  "0.9.5" //hack
        ]
    }
}
