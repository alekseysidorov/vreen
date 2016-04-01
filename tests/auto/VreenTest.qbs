import qbs

CppApplication {
    Depends { name: "Qt.test" }

    Depends { name: "vreen" }
    Depends { name: "vreenoauth" }

    cpp.includePaths: [
        ".",
        "..",
        "../.."
    ]
    consoleApplication: true
    type: [ "application", "test", "autotest" ]

    files: [
        "tst_" + name + ".cpp"
    ]
}
