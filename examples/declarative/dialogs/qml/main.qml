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
        id: dialogsModel
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

    ListView {
        id: dialogsView

        anchors.fill: parent
        scale: 0
        model: dialogsModel

        header: Text {
            width: parent.width
            height: 50
            text: qsTr("Last dialogs")
            font.bold: true
            font.pixelSize: login.font.pixelSize * 1.1
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }

        delegate: Rectangle {
            id: item

            property QtObject contact: incoming ? from : to;

            Component.onCompleted: {
                from.update();
                to.update();
            }

            width: parent.width
            height: 120
            color: index % 2 ? syspal.alternateBase : "transparent"

            Image {
                id: preview

                width: 75
                height: Math.min(sourceSize.height, 75)

                source: contact.photoSource
                fillMode: Image.PreserveAspectFit
                clip: true
                smooth: true

                anchors {
                    left: parent.left
                    leftMargin: 5
                    top: column.top
                }
            }

            Column {
                id: column

                spacing: 2

                anchors {
                    left: preview.right
                    top: parent.top
                    right: parent.right
                    bottom: parent.bottom
                    leftMargin: 10
                    rightMargin: 10
                    topMargin: 10
                }

                Text {
                    id: titleLabel
                    width: parent.width
                    font.bold: true
                    text: qsTr("%1 ➜ %2 %3").arg(from.name).arg(to.name).arg(chatId ? qsTr("(from chat)") : "")
                    elide: Text.ElideRight
                    wrapMode: Text.Wrap
                    maximumLineCount: 1
                }
                Text {
                    id: descriptionLabel
                    width: parent.width
                    text: body
                    elide: Text.ElideRight
                    wrapMode: Text.Wrap
                    maximumLineCount: 3
                }
            }

            Text {
                id: dateLabel

                color: syspal.dark
                font.pixelSize: login.font.pixelSize * 0.8

                anchors {
                    bottom: hr.top
                    bottomMargin: 3
                    left: column.left
                }
                text: {
                    var info = Qt.formatDateTime(date, qsTr("dddd in hh:mm"));
                    if (unread)
                        info += qsTr(", unread");
                    if (Object.keys(attachments).length > 0)
                        info += qsTr(", has attachments")
                    return info;
                }
            }

            Rectangle {
                id: hr
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
                color: syspal.window
            }
        }

        ScrollDecorator {
            flickableItem: parent
        }
    }

    SystemPalette {
        id: syspal
    }

    Connections {
        target: client

        onOnlineChanged: {
            if (client.online) {
                client.roster.sync();
                dialogsModel.getDialogs(0, 15, 160);
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
                target: dialogsView
                scale: 1
            }
        }
    ]
    transitions: [
        Transition {
            from: "*"
            to: "online"
            NumberAnimation { target: dialogsView; property: "scale"; duration: 400; easing.type: Easing.InOutQuad }
            NumberAnimation { target: login; property: "scale"; duration: 400; easing.type: Easing.InOutQuad }
        }
    ]
}
