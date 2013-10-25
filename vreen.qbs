import qbs.base 1.0

Project {   
    property string vreen_bin_path
    property string vreen_qml_path
    property string vreen_lib_path
    property string vreen_libexec_path

    moduleSearchPaths: [ "qbs/modules", "modules" ]

    references: [
        "src/3rdparty/k8json/k8json.qbs",
        "src/api.qbs",
        "src/qml/qml.qbs",
        "src/oauth/oauth.qbs",
        "src/directauth/directauth.qbs",
        "examples/examples.qbs"
    ]
}
