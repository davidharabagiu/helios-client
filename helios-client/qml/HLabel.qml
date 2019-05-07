import QtQuick 2.12

Text {
    id: root

    property bool darkMode: false

    font.pixelSize: 12
    font.family: "Monaco"
    color: darkMode ? "#dddddd" : "#000000"
    text: "My label"
}
