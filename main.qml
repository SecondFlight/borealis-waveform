import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import Borealis.Modules.Audio.Static 1.0

Window {
    id: mainWindow
    visible: true
    width: 900
    height: 480
    minimumWidth: 320
    minimumHeight: 240
    title: qsTr("Waveform")
    color: "#53575A"
    Rectangle {
        id: waveform
        x: 0
        y: 0
        width: mainWindow.width
        height: mainWindow.height - 60
        color: "#53575A"
        Waveform {
            anchors.fill: parent
        }

        /*Canvas {
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            onPaint: {
                var ctx = getContext('2d');
                ctx.fillStyle = Qt.rgba(1, 1, 1, 0.1);
                ctx.fillRect(0, 0, parent.width * 0.5, parent.height * 0.5);
                ctx.font = '46px Arial';
                ctx.fillStyle = Qt.rgba(1, 1, 0, 1);
                ctx.fillText('Yay canvas!', parent.width * 0.5, parent.height * 0.5);
            }
        }*/
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
