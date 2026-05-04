import QtQuick 1.0

Rectangle {
    id: rect
    width: 120
    height: 22
    radius: 6
    smooth: true
    anchors.horizontalCenter: parent.horizontalCenter
    border.color: "#2f2f2f"

    property alias text: label.text

    signal buttonClicked();

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        onEntered: rect.scale = 1.05
        onExited: rect.scale = 1
        onClicked: buttonClicked()
    }
    Text {
        id: label
        color: "#ffffff"
        smooth: true
        font.pixelSize: 12
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        font.bold: false
        horizontalAlignment: Text.AlignHCenter
    }
    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#757575"
        }
        GradientStop {
            position: 1
            color: "#4b4b4b"
        }
    }
}
