import qbs.base 1.0
import "../QuickExample.qbs" as QuickExample

QuickExample {
    destinationDirectory: "examples/dialogs"

    Group {
        fileTags: "install"
        qbs.installDir: destinationDirectory
        files: "qml"
    }
}
