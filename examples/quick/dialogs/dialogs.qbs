import qbs.base 1.0
import "../QuickExample.qbs" as QuickExample

QuickExample {
    destination: "examples/dialogs"

    Group {
        fileTags: "install"
        qbs.installPrefix: destination
        files: "qml"
    }
}
