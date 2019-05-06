import QtQuick 2.12
import QtQuick.Window 2.12
import helios 1.0

Window {
    id: root

    visible: true
    title: qsTr("Helios")

    ApplicationWorkflow {
        anchors.fill: parent
    }

    SettingsController {
        id: settings
    }

    Component.onCompleted: {
        width = settings.windowWidth;
        height = settings.windowHeight;
        x = Screen.width / 2 - width / 2;
        y = Screen.height / 2 - height / 2;

        settings.windowWidth = Qt.binding(function() { return width; });
        settings.windowHeight = Qt.binding(function() { return height; });
    }
}
