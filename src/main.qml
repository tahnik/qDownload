import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.3
import com.tahnik.utils 1.0
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("qDownloader")

    property real received: 0;
    property int time: 0;

    DownloadManager {
        id: downman
    }

    ScrollView {
        id: scrollView
        x: 0
        y: 280
        anchors.topMargin: 100
        anchors.fill: parent
        padding: 0

        ListView {
            spacing: 10
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            clip: true
            cacheBuffer: 0
            boundsBehavior: Flickable.StopAtBounds
            anchors.fill: parent
            keyNavigationWraps: true
            interactive: true
            model: downman.downloads
            delegate: Rectangle {
                id: listItem
                width: parent.width
                height: 67

                anchors.bottomMargin: 10

                Text {
                    id: text1
                    x: 5
                    y: 30
                    color: "#757575"
                    text: modelData.url
                    elide: Text.ElideRight
                    anchors.rightMargin: (parent.width/2) - 5
                    z: 9
                    anchors.right: parent.right
                    renderType: Text.NativeRendering
                    font.family: "Roboto"
                    anchors.top: parent.top
                    anchors.topMargin: 30
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    font.pixelSize: 10
                }

                Text  {
                    text: {
                        if (modelData.errorcode && modelData.fileSize === 0) {
                            return "Could not download the file";
                        }
                        return modelData.name
                    }
                    elide: Text.ElideRight
                    anchors.rightMargin: (parent.width/2) - 5
                    anchors.right: parent.right
                    padding: 1
                    renderType: Text.NativeRendering
                    font.weight: Font.Light
                    font.family: "Roboto Light"
                    font.pointSize: 12
                    z: 9
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 5

                }

                ProgressBar {
                    id: progressCom
                    height: 79
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.top: parent.top
                    to: parseInt(modelData.fileSize)
                    value: parseInt(modelData.received)
                    background: Rectangle {
                        anchors.fill: parent
                        color: "#FFFFFF"
                    }

                    contentItem: Item {
                        anchors.fill: parent
                        Rectangle {
                            width: {
                                if (modelData.errorcode && modelData.fileSize === 0) {
                                    return parent.width;
                                }
                                return progressCom.visualPosition * parent.width
                            }
                            height: parent.height
                            color: {
                                if (modelData.errorcode !== 0) {
                                    console.log(modelData.errorcode);
                                    return "#EF9A9A"
                                } else if (modelData.received === modelData.fileSize) {
                                    return "#A5D6A7"
                                }
                                return "#90CAF9"
                            }
                        }
                    }
                }

                Text {
                    id: speed
                    y: 38
                    text: qsTr(parseFloat(modelData.speed).toFixed(2) + " MB/s")
                    elide: Text.ElideRight
                    anchors.rightMargin: (parent.width/2) -5
                    renderType: Text.NativeRendering
                    font.family: "Roboto"
                    z: 9
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    font.pixelSize: 10
                }
                Button {
                    id: stop
                    x: 226
                    y: 0
                    width: this.height - 20
                    text: ""
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    onClicked: {
                        modelData.abort();
                    }
                    Image {
                        id: image1
                        anchors.rightMargin: 5
                        anchors.leftMargin: 5
                        anchors.bottomMargin: 22
                        anchors.topMargin: 22
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        sourceSize.width: 0
                        z: 9
                        source: "assets/stop.png"
                        sourceSize.height: parent.background.height - 6
                    }
                    anchors.topMargin: 0
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    z: 9
                    anchors.bottomMargin: 0
                    background: Rectangle {
                        color: 'transparent'
                    }
                    onHoveredChanged: {
                        this.background.color = hovered ? "#EEEEEE" : "transparent"
                    }
                }

                Button {
                    id: deleteNF
                    x: 226
                    y: 0
                    width: this.height - 20
                    text: ""
                    anchors.rightMargin: 0
                    anchors.right: stop.left
                    z: 9
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.top: parent.top
                    anchors.topMargin: 0

                    Image {
                        id: image
                        anchors.rightMargin: 5
                        anchors.leftMargin: 5
                        anchors.bottomMargin: 20
                        anchors.topMargin: 20
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        sourceSize.height: parent.background.height - 6
                        sourceSize.width: 0
                        z: 9
                        source: "assets/remove.png"
                    }
                    background: Rectangle {
                        color: 'transparent'
                    }
                    onHoveredChanged: {
                        this.background.color = hovered ? "#EEEEEE" : "transparent"
                    }
                }

                Button {
                    id: deleteF
                    x: 216
                    width: this.height - 20
                    text: ""
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.right: deleteNF.left
                    anchors.rightMargin: 0
                    Image {
                        id: image2
                        anchors.rightMargin: 5
                        anchors.leftMargin: 5
                        anchors.bottomMargin: 20
                        anchors.topMargin: 20
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        sourceSize.width: 0
                        z: 9
                        source: "assets/delete.png"
                        sourceSize.height: parent.background.height - 6
                    }
                    background: Rectangle {
                        color: 'transparent'
                    }
                    onHoveredChanged: {
                        this.background.color = hovered ? "#EEEEEE" : "transparent"
                    }
                    z: 9
                }

                Button {
                    id: restart
                    x: 214
                    y: 7
                    width: this.height - 20
                    text: ""
                    onClicked: {
                        modelData.restart();
                    }

                    Image {
                        id: image3
                        fillMode: Image.PreserveAspectFit
                        anchors.rightMargin: 5
                        anchors.topMargin: 20
                        sourceSize.width: 0
                        z: 9
                        source: "assets/restart.png"
                        sourceSize.height: parent.background.height - 6
                        anchors.leftMargin: 5
                        anchors.bottomMargin: 20
                        anchors.fill: parent
                    }
                    anchors.rightMargin: 0
                    background: Rectangle {
                        color: 'transparent'
                    }
                    onHoveredChanged: {
                        this.background.color = hovered ? "#EEEEEE" : "transparent"
                    }
                    anchors.topMargin: 0
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    z: 9
                    anchors.right: deleteF.left
                    anchors.bottomMargin: 0
                }
            }
        }
    }

    Button {
        id: download
        y: 38
        text: qsTr("Download")
        anchors.right: parent.right
        anchors.rightMargin: 10
        onClicked: {
            downman.addToQueue(textField.text)
        }
    }

    TextField {
        id: textField
        y: 38
        height: 40
        clip: true
        antialiasing: true
        renderType: Text.NativeRendering
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 238
        placeholderText: "Enter URL Here"
        selectByMouse: true
    }

    Button {
        id: addToQueue
        x: 416
        y: 38
        text: qsTr("Add to Queue")
        anchors.rightMargin: 124
        anchors.right: parent.right
    }



}
