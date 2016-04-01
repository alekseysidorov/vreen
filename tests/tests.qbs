import qbs.base

Project {
    condition: project.with_tests

    references: [
        "auto/connection/connection.qbs"
    ]

    AutotestRunner { }
}

