// import QtQuick 1.0 // to target Maemo 5
import QtQuick 1.1
import com.vk.api 1.0

Rectangle {
    width: 500
    height: 800

    TextEdit {
        anchors.centerIn: parent

        readOnly: true
        onLinkActivated: {
            console.log("connecting")
            client.connectToHost()
        }
        text: qsTr("<a href=\"http://vk.com\">Login...</a>")
    }
}
