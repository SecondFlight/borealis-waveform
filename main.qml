import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import Borealis.Modules.Audio.Static 1.0

Window {
    property real leftVal: 0
    property real rightVal: 1

    id: mainWindow
    visible: true
    width: 900
    height: 480
    minimumWidth: 320
    minimumHeight: 240
    title: qsTr("Waveform")
    color: "#53575A"
    Rectangle {
        id: waveformParent
        x: 0
        y: 0
        width: mainWindow.width
        height: mainWindow.height - 60
        color: "#53575A"
        Waveform {
            id: waveform
            anchors.fill: parent
            left: leftVal
            right: rightVal
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onWheel: {
                    var currentWidth = rightVal - leftVal;

                    // normalized
                    var xPos = mouseX / waveform.width;

                    var scrollAmt = wheel.angleDelta.y * 0.0005;
                    var l = leftVal + currentWidth * xPos * scrollAmt;
                    var r = rightVal - currentWidth * (1 - xPos) * scrollAmt;
                    if (l < 0)
                        leftVal = 0;
                    else
                        leftVal = l;
                    if (r > 1)
                        rightVal = 1;
                    else
                        rightVal = r;
                }
            }
        }
    }
    Rectangle {
        id: footer
        x: 0
        y: mainWindow.height - 60
        width: mainWindow.width
        height: 60
        color: "#323538"
        border.color: "#262728"
        border.width: 1
        Rectangle {
            id: btnLoadFile
            width: footer.height * 2
            height: footer.height
            color: "#323538"
            border.color: "#262728"
            border.width: 1
            Text {
                id: loadFileText
                text: qsTr("Load file...")
                color: "#37A483"
                anchors.centerIn: parent
                font.pixelSize: parent.height / 3
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    messageDialog.visible = true;
                }
            }
        }
    }
    MessageDialog {
        id: messageDialog
        title: "Info"
        text: "Load file clicked."
    }
}
