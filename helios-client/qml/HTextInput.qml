import QtQuick 2.12

Rectangle {
    id: root

    property bool darkMode: false
    property string text: input.text
    property string hint
    property bool password: false
    property bool highlightFocus: false

    width: 140
    height: 25
    color: darkMode ? (input.focus && highlightFocus ? "#013670" : "#202020") :
                      (input.focus && highlightFocus ? "#a6d0fc" : "#bebebe")
    border.width: 1
    border.color: darkMode ? "#989898" : "#878787"
    radius: 3
    clip: true

    Text {
        id: hintText

        font.pixelSize: 12
        font.family: "Monaco"
        font.italic: true
        color: darkMode ? "#aaaaaa" : "#888888"
        text: hint
        visible: !input.focus && input.text.length === 0
        anchors {
            left: parent.left
            leftMargin: 3
            right: parent.right
            rightMargin: 3
            verticalCenter: parent.verticalCenter
        }
    }

    TextInput {
        id: input
        font.pixelSize: 12
        font.family: "Monaco"
        color: darkMode ? "#dddddd" : "#000000"
        echoMode: password ? TextInput.Password : TextInput.Normal
        anchors {
            left: parent.left
            leftMargin: 3
            right: parent.right
            rightMargin: 3
            verticalCenter: parent.verticalCenter
        }
    }
}
