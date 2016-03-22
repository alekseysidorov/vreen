import qbs.base

Project {
    qbsSearchPaths: "qbs"

    references: [
        "quick/dialogs/dialogs.qbs",
        "quick/audio/audio.qbs",
        "cpp/phonefetcher/phonefetcher.qbs"
    ]
}

