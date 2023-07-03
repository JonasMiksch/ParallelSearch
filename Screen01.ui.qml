

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.5
import QtQuick.Controls 6.5
import UntitledProject

Rectangle {
    id: rectangle
    width: Constants.width
    height: Constants.height

    color: Constants.backgroundColor

    Button {
        id: button
        width: 100
        height: 40
        text: qsTr("Lineare Suche")
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -84
        anchors.horizontalCenterOffset: -253
        checkable: true
        anchors.horizontalCenter: parent.horizontalCenter

        Connections {
            target: button
            onClicked: animation.start()
        }
    }

    Text {
        id: label
        width: 156
        height: 40
        text: qsTr("Insert Search Term")
        anchors.top: button.bottom
        font.bold: true
        anchors.horizontalCenterOffset: -225
        font.family: Constants.font.family
        anchors.topMargin: -165
        anchors.horizontalCenter: parent.horizontalCenter

        SequentialAnimation {
            id: animation

            ColorAnimation {
                id: colorAnimation1
                target: rectangle
                property: "color"
                to: "#2294c6"
                from: Constants.backgroundColor
            }

            ColorAnimation {
                id: colorAnimation2
                target: rectangle
                property: "color"
                to: Constants.backgroundColor
                from: "#2294c6"
            }
        }
    }

    Button {
        id: button1
        x: 657
        y: 657
        text: qsTr("Binear Search")
    }

    Button {
        id: button2
        x: 657
        y: 555
        text: qsTr("Build Tree")
    }

    Label {
        id: label1
        x: 922
        y: 504
        width: 28
        height: 23
        text: qsTr("Label")
    }

    Label {
        id: label2
        x: 657
        y: 504
        width: 28
        height: 23
        text: qsTr("Label")
    }

    Label {
        id: label3
        x: 1129
        y: 504
        width: 28
        height: 23
        text: qsTr("Label")
    }

    TextInput {
        id: textInput
        x: 657
        y: 372
        width: 195
        height: 20
        text: qsTr("Text Input")
        font.pixelSize: 12
    }

    Label {
        id: label4
        x: 922
        y: 613
        width: 28
        height: 23
        text: qsTr("Label")
    }

    Label {
        id: label5
        x: 657
        y: 613
        width: 28
        height: 23
        text: qsTr("Label")
    }

    Label {
        id: label6
        x: 1129
        y: 613
        width: 28
        height: 23
        text: qsTr("Label")
    }

    Label {
        id: label7
        x: 922
        y: 725
        width: 28
        height: 23
        text: qsTr("Label")
    }

    Label {
        id: label8
        x: 657
        y: 725
        width: 28
        height: 23
        text: qsTr("Label")
    }

    Label {
        id: label9
        x: 1129
        y: 725
        width: 28
        height: 23
        text: qsTr("Label")
    }
    states: [
        State {
            name: "clicked"
            when: button.checked

            PropertyChanges {
                target: label
                text: qsTr("Button Checked")
            }
        }
    ]
}
