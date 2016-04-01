import qbs.base

Project {   
    property string vreen_bin_path: "bin"
    property string vreen_qml_path: "bin"
    property string vreen_lib_path: "lib"
    property string vreen_libexec_path: "lib"

    property string vreen_version_major:  2
    property string vreen_version_minor: 0
    property string vreen_version_path: 0

    property bool with_examples: true
    property bool with_tests: true

    Properties {
        condition: qbs.targetOS.contains("windows")
        vreen_libexec_path: "bin"
    }

    references: [
        "src/api.qbs",
        "src/qml/qml.qbs",
        "src/oauth/oauth.qbs",
        "src/directauth/directauth.qbs",
        "examples/examples.qbs",
        "tests/tests.qbs"
    ]
}
