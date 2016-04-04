import qbs.base 1.0

Product {
    condition: false

    property string clientId
    property string clientSecret
    property string clientName

    type: ["staticlibrary", "installed_content"]
    name: "vreendirectauth"
    destinationDirectory: vreen_lib_path

    cpp.defines: [
        "VREEN_DIRECTAUTH_CLIENT_ID=\"" + clientId + "\"",
        "VREEN_DIRECTAUTH_CLIENT_SECRET=\"" + clientSecret + "\"",
        "VREEN_DIRECTAUTH_CLIENT_NAME=\"" + clientName + "\""
    ]

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ['core', 'network'] }
    Depends { name: "vreen" }

    files: [
        "*.cpp"
    ]

    Group {
        files: "*.h"
        fileTags: ["hpp", "devheader"]
        overrideTags: false
    }
}

