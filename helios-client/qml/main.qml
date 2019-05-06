import QtQuick 2.12
import QtQuick.Window 2.12
import helios 1.0

Window {
    id: root

    visible: true
    title: qsTr("Helios")

    ApplicationWorkflow {
        anchors.fill: parent
        settingsCtl: settingsCtl
    }

    SettingsController {
        id: settingsCtl
    }

    Component.onCompleted: {
        width = settingsCtl.windowWidth;
        height = settingsCtl.windowHeight;
        x = Screen.width / 2 - width / 2;
        y = Screen.height / 2 - height / 2;

        settingsCtl.windowWidth = Qt.binding(function() { return width; });
        settingsCtl.windowHeight = Qt.binding(function() { return height; });
    }
}
