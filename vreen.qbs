import qbs.base

Project {   
    property string vreen_bin_path: "bin"
    property string vreen_qml_path: "bin"
    property string vreen_lib_path: "lib"
    property string vreen_libexec_path: "lib"

    property string vreen_version_major:  1
    property string vreen_version_minor: 9
    property string vreen_version_path: 0

    Properties {
        condition: qbs.targetOS.contains("windows")
        vreen_libexec_path: "bin"
    }

    references: [
        "src/api.qbs",
        "src/qml/qml.qbs",
        "src/oauth/oauth.qbs",
        "src/directauth/directauth.qbs",
        "examples/examples.qbs"
    ]
}
