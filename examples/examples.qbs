import qbs.base

Project {
    condition: project.with_examples
    qbsSearchPaths: "qbs"

    references: [
        "quick/dialogs/dialogs.qbs",
        "quick/audio/audio.qbs",
        "cpp/phonefetcher/phonefetcher.qbs"
    ]
}

