import qbs.base 1.0

CppApplication {
    type: "application"
    destinationDirectory: "examples/phonefetcher"

    Depends { name: "vreen"}
    Depends { name: "vreenoauth" }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: destinationDirectory
    }

    files: [
        "main.cpp",
        "phonefetcher.cpp",
        "phonefetcher.h",
    ]
}
