import qbs.base 1.0
import "../../qbs/VreenProduct.qbs" as VreenProduct

VreenProduct {
    name: "vreenplugin"
    type: ["dynamiclibrary", "installed_content"]

    property string uri: "com.vk.api"
    property bool system: false

    function __destination() {
        return system ? "todo/" : "bin/" + uri.replace(/\./g, "/");
    }

    destination: __destination()

    cpp.includePaths: [
        "src"
    ]
    cpp.defines: []

    files: [
        "src/audiomodel.cpp",       "src/buddymodel.h",
        "src/clientimpl.cpp",       "src/commentsmodel.h",
        "src/newsfeedmodel.cpp",    "src/wallmodel.cpp",
        "src/audiomodel.h",         "src/chatmodel.cpp",
        "src/clientimpl.h",         "src/dialogsmodel.cpp",
        "src/newsfeedmodel.h",      "src/wallmodel.h",
        "src/buddymodel.cpp",       "src/chatmodel.h",
        "src/commentsmodel.cpp",    "src/dialogsmodel.h",
        "src/vkitqmlplugin.h",      
    ]

    Depends { name: "cpp" }    
    Depends { name: "Qt.core" }    
    Depends { name: "Qt.network" }        
    Depends { name: "Qt.declarative"; condition: qt.core.versionMajor === 4 }
    Depends { name: "Qt.quick"; condition: qt.core.versionMajor === 5 }    
    Depends { name: "vreen"}
    Depends { name: "vreenoauth" }

    Properties {
        condition: true
        cpp.defines: outer.concat("VREEN_WITH_OAUTH")
    }
    Properties {
        condition: qt.core.versionMajor === 5
        cpp.defines: outer.concat("QT_DISABLE_DEPRECATED_BEFORE=0")
    }

    Group {
        qbs.installDir: __destination()
        fileTags: ["install", "qml"]
        files: [
            "qmldir/PhotoModel.qml",    "qmldir/qmldir"
        ]
    }
    Group {
        condition: qt.core.versionMajor === 4
        files:  "vreenplugin_qt4.cpp"
    }
    Group {
        condition: qt.core.versionMajor === 5
        files: "vreenplugin_qt5.cpp"
    }
}
