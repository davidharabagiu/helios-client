import QtQuick 2.12

Item {
    id: root

    property bool darkMode: false
    property string label: "My check box"
    property bool checked: false

    height: 20

    Rectangle {
        id: checkedRectangle

        width: height
        height: root.height
        color: darkMode ? "#101010" : "#cecece"
        border.color: darkMode ? "#989898" : "#878787"
        border.width: 1

        Rectangle {
            width: checkedRectangle.width - 10
            height: checkedRectangle.height - 10
            color: "#0f81fc"
            visible: checked
            anchors.centerIn: parent
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            onClicked: {
                checked = !checked;
            }
        }
    }

    Text {
        id: labelText

        font.pixelSize: 12
        font.family: "Monaco"
        color: darkMode ? "#dddddd" : "#000000"
        text: label
        anchors {
            left: checkedRectangle.right
            leftMargin: 5
            verticalCenter: checkedRectangle.verticalCenter
        }
    }
}
