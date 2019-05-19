import QtQuick 2.12
import QtQuick.Dialogs 1.2
import helios 1.0

Rectangle {
    id: root

    property bool darkMode: false
    property string username
    property string authToken

    signal logoutButtonActivated

    color: darkMode ? "#000000" : "#ffffff"

    HLabel {
        id: welcomeLabel
        text: "Welcome, " + username
        darkMode: root.darkMode
        anchors {
            left: parent.left
            leftMargin: 5
            top: parent.top
            topMargin: 5
        }
    }

    HButton {
        darkMode: root.darkMode
        label: "Log out"
        anchors {
            left: welcomeLabel.right
            leftMargin: 5
            top: parent.top
            topMargin: 5
        }
        onClicked: {
            logoutButtonActivated();
        }
    }

    MessageDialog {
        id: errorDialog
        title: "Error"
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        selectExisting: true
        selectFolder: false
        selectMultiple: false
        onAccepted: {
            rfsCtl.upload(fileUrl);
        }
    }

    FileDialog {
        id: downloadDirPickDlg

        property string fileToDownload

        title: "Chose a destination directory"
        folder: shortcuts.home
        selectExisting: true
        selectFolder: true
        selectMultiple: false
        onAccepted: {
            rfsCtl.download(fileToDownload, fileUrl);
        }
    }

    RemoteFileSystemController {
        id: rfsCtl

        authenticationToken: authToken

        onCwdChanged: {
            fileListing.files = files;
            pathInput.setText(cwd);
        }

        onFileAddedInCwd: {
            fileListing.insertFile(file);
        }

        onFileRemovedFromCwd: {
            fileListing.removeFile(fileName);
        }

        onTransferAdded: {
            transferList.addTranfer(transfer);
        }

        onTransferUpdated: {
            transferList.updateTransfer(transfer);
        }

        onTransferRemoved: {
            transferList.removeTransfer(transfer);
        }

        onError: {
            errorDialog.text = message;
            errorDialog.visible = true;
        }
    }

    KeyStorageController {
        id: ksCtl

        onKeysChanged: {
            var list = ksCtl.keys(KeyStorageController.KEY_SIZE_256);
            if (list.length === 0) {
                keyListing.selectedIndex = -1;
            }
            keyListing.model = list;
        }
    }

    FileListing {
        id: fileListing
    }

    TransfersList {
        id: transferList
    }

    HTextInput {
        id: pathInput

        darkMode: root.darkMode

        anchors {
            left: parent.left
            leftMargin: 5
            top: welcomeLabel.bottom
            topMargin: 20
            right: parent.right
            rightMargin: 5
        }

        onAccepted: {
            rfsCtl.cwd = text;
        }
    }

    Row {
        id: fileControls

        anchors {
            left: parent.left
            leftMargin: 5
            top: pathInput.bottom
            topMargin: 5
            right: parent.right
            rightMargin: 5
        }

        height: 40
        spacing: 5

        HTextInput {
            id: fileNameInput
            darkMode: root.darkMode
            hint: "File name"
            anchors.verticalCenter: parent.verticalCenter
        }

        HButton {
            darkMode: root.darkMode
            anchors.verticalCenter: parent.verticalCenter
            label: "New Dir"

            onClicked: {
                rfsCtl.createDirectory(fileNameInput.text);
            }
        }

        HButton {
            darkMode: root.darkMode
            anchors.verticalCenter: parent.verticalCenter
            label: "Back"

            onClicked: {
                rfsCtl.goBack();
            }
        }

        HButton {
            darkMode: root.darkMode
            anchors.verticalCenter: parent.verticalCenter
            label: "Delete"

            onClicked: {
                rfsCtl.remove(fileNameInput.text);
            }
        }

        HTextInput {
            id: destinationDirInput
            darkMode: root.darkMode
            hint: "Destination"
            anchors.verticalCenter: parent.verticalCenter
        }

        HButton {
            darkMode: root.darkMode
            anchors.verticalCenter: parent.verticalCenter
            label: "Move"

            onClicked: {
                rfsCtl.move(fileNameInput.text, destinationDirInput.text);
            }
        }

        HButton {
            darkMode: root.darkMode
            anchors.verticalCenter: parent.verticalCenter
            label: "Rename"

            onClicked: {
                rfsCtl.rename(fileNameInput.text, destinationDirInput.text);
            }
        }

        HButton {
            darkMode: root.darkMode
            anchors.verticalCenter: parent.verticalCenter
            label: "Upload"

            onClicked: {
                fileDialog.visible = true;
            }
        }
    }

    Rectangle {
        id: fileListingContainer

        anchors {
            left: parent.left
            top: fileControls.bottom
            leftMargin: 5
            topMargin: 5
            right: transfersPane.left
            rightMargin: 5
            bottom: keyListingContainer.top
            bottomMargin: 5
        }

        clip: true
        border.width: 1
        border.color: root.darkMode ? "#ffffff" : "#000000"

        ListView {
            anchors.fill: parent
            model: fileListing
            delegate: Rectangle {
                width: 300
                height: 30
                color: "transparent"
                HLabel {
                    darkMode: root.darkMode
                    text: model.fileData.name + " " + (model.fileData.isDirectory ? "[dir]" : model.fileData.size)
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (model.fileData.isDirectory) {
                            rfsCtl.openDirectory(model.fileData.name);
                        } else {
                            downloadDirPickDlg.fileToDownload = model.fileData.name;
                            downloadDirPickDlg.visible = true;
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: keyListingContainer

        anchors {
            left: parent.left
            leftMargin: 5
            right: transfersPane.left
            rightMargin: 5
            bottom: parent.bottom
            bottomMargin: 5
        }

        height: 150

        clip: true
        border.width: 1
        border.color: root.darkMode ? "#ffffff" : "#000000"

        ListView {
            id: keyListing

            property int selectedIndex: -1
            property string selectedKey

            anchors.fill: parent
            model: ksCtl.keys(KeyStorageController.KEY_SIZE_256)
            delegate: Rectangle {
                width: 300
                height: 25
                color: keyListing.selectedIndex === index ? "#777777" : "transparent"
                HLabel {
                    darkMode: root.darkMode
                    text: modelData
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        keyListing.selectedIndex = index;
                        keyListing.selectedKey = modelData;
                    }
                }
            }
        }

        HTextInput {
            id: keyNameInput
            darkMode: root.darkMode
            hint: "Key name"
            anchors {
                right: newKeyButton.left
                rightMargin: 5
                bottom: parent.bottom
                bottomMargin: 5
            }
        }

        HButton {
            id: newKeyButton
            anchors {
                bottom: parent.bottom
                bottomMargin: 5
                right: parent.right
                rightMargin: 5
            }

            onClicked: {
                if (!ksCtl.createKey(keyNameInput.text, KeyStorageController.KEY_SIZE_256)) {
                    errorDialog.text = "A key with this name already exists";
                    errorDialog.visible = true;
                }
            }

            darkMode: root.darkMode
            label: "New key"
        }

        HButton {
            id: removeKeyButton
            anchors {
                bottom: newKeyButton.top
                bottomMargin: 5
                right: parent.right
                rightMargin: 5
            }
            darkMode: root.darkMode
            label: "Remove key"

            onClicked: {
                if (keyListing.selectedIndex !== -1) {
                    if (!ksCtl.removeKey(keyListing.selectedKey)) {
                        errorDialog.text = "There is no key with the name " + keyListing.selectedKey;
                        errorDialog.visible = true;
                    }
                }
            }
        }
    }

    Rectangle {
        id: transfersPane

        anchors {
            right: parent.right
            top: fileControls.bottom
            bottom: parent.bottom
            leftMargin: 5
            topMargin: 5
            bottomMargin: 5
        }

        width: 300

        ListView {
            anchors.fill: parent
            model: transferList
            delegate: Rectangle {
                width: transfersPane.width
                height: 100
                Column {
                    anchors.fill: parent
                    spacing: 3

                    HLabel {
                        darkMode: root.darkMode
                        text: model.transferData.remotePath
                    }

                    HLabel {
                        darkMode: root.darkMode
                        text: String(model.transferData.transferredBytes) + " (" +
                              String(Math.round(model.transferData.transferredBytes / model.transferData.fileSize * 100)) + "%)"
                    }

                    HButton {
                        darkMode: root.darkMode
                        label: "Cancel"
                        onClicked: {
                            rfsCtl.cancelTransfer(model.transferData);
                        }
                    }
                }
            }
        }
    }
}
