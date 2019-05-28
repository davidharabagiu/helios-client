import QtQuick 2.12
import QtQuick.Dialogs 1.2
import helios 1.0

Rectangle {
    id: root

    property bool darkMode: false
    property var session

    signal backButtonActivated()
    signal acceptButtonActivated(var notification)

    color: darkMode ? "#000000" : "#ffffff"

    Row {
        id: controls

        spacing: 5

        anchors {
            left: parent.left
            right: parent.right
        }

        height: 30

        HButton {
            id: backButton

            darkMode: root.darkMode
            label: "Back"
            onClicked: {
                backButtonActivated();
            }

            height: controls.height
        }

        HButton {
            id: dismissButton

            darkMode: root.darkMode
            label: "Dismiss"
            onClicked: {
                if (notificationList.selectedIndex !== -1) {
                    notCtl.dismissNotification(notificationList.selectedItem.id);
                }
            }

            height: controls.height
        }

        HButton {
            id: dismissAllButton

            darkMode: root.darkMode
            label: "Dismiss All"
            onClicked: {
                notCtl.dismissAllNotifications();
            }

            height: controls.height
        }

        HButton {
            id: acceptButton

            darkMode: root.darkMode
            label: "Accept"
            onClicked: {
                acceptButtonActivated(notificationList.selectedItem);
            }

            height: controls.height
        }
    }

    MessageDialog {
        id: errorDialog
        title: "Error"
    }

    NotificationsController {
        id: notCtl

        onNotificationsChanged: {
            if (notCtl.notifications.length <= notificationList.selectedIndex) {
                notificationList.selectedIndex = -1;
                notificationList.selectedItem = null;
            }
        }

        session: root.session
    }

    Rectangle {
        id: notificationsListContainer

        anchors {
            top: controls.bottom
            topMargin: 5
            left: parent.left
            leftMargin: 5
            right: parent.right
            rightMargin: 5
            bottom: parent.bottom
            bottomMargin: 5
        }

        height: 150

        clip: true

        ListView {
            id: notificationList

            property int selectedIndex: -1
            property var selectedItem

            anchors.fill: parent
            model: notCtl.notifications
            delegate: Rectangle {
                width: notificationList.width
                height: 25
                color: notificationList.selectedIndex === index ? "#777777" : "transparent"

                HLabel {
                    darkMode: root.darkMode
                    text: modelData.text
                    anchors.verticalCenter: parent.verticalCenter
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        notificationList.selectedIndex = index;
                        notificationList.selectedItem = modelData;
                    }
                }
            }
        }
    }
}
