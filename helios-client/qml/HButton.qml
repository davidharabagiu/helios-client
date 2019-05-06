import QtQuick 2.12

Rectangle {
    id: root

    property bool darkMode: false
    property string label: "My button"

    readonly property color normalColor: darkMode ? "#101010" : "#cecece"
    readonly property color hoverColor: darkMode ? "#202020" : "#d4d4d4"
    readonly property color clickedColor: darkMode ? "#303030" : "#dadada"

    signal clicked()

    width: 80
    height: 25
    color: mouseArea.containsPress ? clickedColor : (mouseArea.containsMouse ? hoverColor : normalColor)
    border.width: 1
    border.color: darkMode ? "#989898" : "#878787"

    Text {
        font.pixelSize: 12
        font.family: "Monaco"
        color: darkMode ? "#dddddd" : "#000000"
        text: label
        anchors.centerIn: parent
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            root.clicked();
        }
    }
}
