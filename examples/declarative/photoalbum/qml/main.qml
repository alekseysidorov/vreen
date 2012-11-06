import QtQuick 1.1
import com.vk.api 1.0

Rectangle {
    id: root

    width: 500
    height: 800

    TextEdit {
        id: login

        onLinkActivated: client.connectToHost()

        z: 10
        anchors.centerIn: parent
        text: qsTr("<a href=\"http://vk.com\">Click to login</a>")
        textFormat: TextEdit.RichText
    }

    DialogsModel {
        id: dialogs
        client: client
    }

    Client {
        id: client
        connection: conn
    }

    OAuthConnection {
        id: conn

        Component.onCompleted: {
            setConnectionOption(Connection.ShowAuthDialog, true);
            setConnectionOption(Connection.KeepAuthData, true);
        }

        clientId: 3220807
        displayType: OAuthConnection.Popup
    }

    PhotoAlbum {
        id: photoAlbum

        anchors.fill: parent
        scale: 0
    }

    SystemPalette {
        id: syspal
    }

    Connections {
        target: client

        onOnlineChanged: {
            if (client.online) {
                client.roster.sync();
                dialogs.getDialogs(0, 15, 160);
            }
        }
    }

    states: [
        State {
            name: "online"
            when: client.online
            PropertyChanges {
                target: login
                scale: 0
            }
            PropertyChanges {
                target: photoAlbum
                scale: 1
            }
        }
    ]
    transitions: [
        Transition {
            from: "*"
            to: "online"
            NumberAnimation { target: photoAlbum; property: "scale"; duration: 400; easing.type: Easing.InOutQuad }
            NumberAnimation { target: login; property: "scale"; duration: 400; easing.type: Easing.InOutQuad }
        }
    ]
}
