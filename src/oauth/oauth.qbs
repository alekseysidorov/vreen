import qbs.base 1.0

Product {
    type: ["staticlibrary", "installed_content"]
    name: "vreenoauth"
    destination: "lib"

    //cpp.warningLevel: "all"

    Depends { name: "cpp" }
    Depends { name: "qt"; submodules: ['core', 'network', 'webkit'] }
    Depends { name: "vreen"; }

    files: [
        "oauthconnection.cpp",
        "oauthconnection.h"
    ]

    Group {
        qbs.installDir: "include/vreen/auth"
        fileTags: ["install"]
        files: ["oauthconnection.h"]
    }
}

