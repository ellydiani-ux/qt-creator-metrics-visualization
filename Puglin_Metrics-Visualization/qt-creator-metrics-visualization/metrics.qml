import QtQuick 1.0

Rectangle {
    id: rectangle1
    width: 600
    height: 400

    Column {
        id: menu
        y: -7
        width: parent.width / 5
        anchors.left: parent.left
        anchors.leftMargin: 0
        spacing: 10
        Rectangle {
            id: methods
            width: 120
            height: 22
            radius: 6
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
            border.color: "#2f2f2f"
            MouseArea {
                id: mouseareaMethods
                hoverEnabled: true
                anchors.fill: parent
                onEntered: methods.scale = 1.05
                onExited: methods.scale = 1
                onClicked: {
                    for (var i = 0; i < graph.children.length; ++i)
                        graph.children[i].destroy();
                    var projectClasses = metricsCollector.projectClassesToVariantList();
                    var classesFunctions = metricsCollector.classFunctionsToVariantMap();
                    for (var i = 0; i < projectClasses.length; ++i)
                    {
                        var rect = Qt.createQmlObject("import QtQuick 1.0; Rectangle { color: 'red'; width: 20; }", graph);
                        rect.x = i*30;
                        rect.height = classesFunctions[projectClasses[i]]*10;
                        rect.y = graph.height / 2 - rect.height
                    }
                }
            }

            Text {
                id: text1
                color: "#ffffff"
                text: qsTr("QMC")
                smooth: true
                font.pixelSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.bold: false
                horizontalAlignment: Text.AlignHCenter
            }
            anchors.horizontalCenterOffset: 0
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

        Rectangle {
            id: attributes
            width: 120
            height: 22
            radius: 6
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
            border.color: "#2f2f2f"
            Text {
                id: text3
                color: "#ffffff"
                text: qsTr("QAC")
                smooth: true
                font.pixelSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.bold: false
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea {
                id: mouseareaAttributes
                hoverEnabled: true
                anchors.fill: parent
                onEntered: attributes.scale = 1.05
                onExited: attributes.scale = 1
            }
            anchors.horizontalCenterOffset: 0
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
        anchors.verticalCenter: parent.verticalCenter


        Rectangle {
            id: mac
            width: 120
            height: 22
            radius: 6
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
            border.color: "#2f2f2f"
            Text {
                id: text4
                color: "#ffffff"
                text: qsTr("MAC")
                smooth: true
                font.pixelSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.bold: false
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea {
                id: mouseareaMAC
                hoverEnabled: true
                anchors.fill: parent
                onEntered: mac.scale = 1.05
                onExited: mac.scale = 1
            }
            anchors.horizontalCenterOffset: 0
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

        Rectangle {
            id: mtm
            width: 120
            height: 22
            radius: 6
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
            border.color: "#2f2f2f"
            Text {
                id: text5
                color: "#ffffff"
                text: qsTr("MTM")
                smooth: true
                font.pixelSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.bold: false
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea {
                id: mouseareatMTM
                hoverEnabled: true
                anchors.fill: parent
                onEntered: mtm.scale = 1.05
                onExited: mtm.scale = 1
            }
            anchors.horizontalCenterOffset: 0
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

      }

    Rectangle {
        id: graph
        x: 5
        y: 6
        width: 4*(parent.width / 5)
        height: parent.height
        color: "#ffffff"
        anchors.rightMargin: 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
    }


}
