import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.14
import QtQuick.Dialogs 1.2
import com.busra.histogramcontroller 1.0
import com.busra.imagepainter 1.1

Window {
    id:root
    visible: true
    width: 640
    height: 480
    title: qsTr("Histogram Stretching")

    // Histogram Controller
    HistogramController{
        id: histogramController
    }

    property var imageUrl: ""

    Rectangle{
        id:baseRectangle
        width: parent.width
        height: 40
        color: "transparent"
        Rectangle{
            id:imageDataPathRectangle
            width: 500
            height: 40
            color: "#efc7b5"
            anchors.top: parent.top
            anchors.left: parent.left

            TextField {
                id: imageDataPath
                width: 500
                height: 40
                text: qsTr("")
                font.pixelSize: 16
                font.styleName: ""
                anchors.topMargin: 5
                //anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                background: Rectangle{
                    id:backgroundRectangle
                    color: "white"
                    width: 490
                    height: parent.height - 8
                    radius: 10
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
            Rectangle{
                id:selectButtonRect
                width: baseRectangle.width-imageDataPathRectangle.width
                height: 40
                anchors.left: imageDataPathRectangle.right
                anchors.top: parent.top
                Button{
                    id:selectButton
                    anchors.fill: parent
                    text: "Select"
                    onClicked: {
                        fileDialog.open();
                    }
                }
            }
        }
    }
    Rectangle{
        id:imageSideRectangle
        width: parent.width
        height: parent.height-baseRectangle.height
        anchors.top: baseRectangle.bottom
        anchors.left: parent.left
        color: "transparent"

        Rectangle{
            id:leftSideImage
            width: parent.width/2
            height: parent.height - 60
            anchors.left: parent.left
            anchors.top: parent.top
            color: "#b3d3dc"

            Rectangle{
                id:leftSideImageRectangle
                width: parent.width- 20
                height: parent.height-20
                anchors.verticalCenter: leftSideImage.verticalCenter
                anchors.horizontalCenter: leftSideImage.horizontalCenter
                radius: 10
                color: "white"

                Image{
                    id: selectedImage
                    source: histogramController.imageUrl
                    anchors.fill:parent
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
        Rectangle{
            id: rightSideImage
            width: parent.width/2
            height: parent.height - 60
            anchors.left: leftSideImage.right
            anchors.top: parent.top
            color: "#b3d3dc"

            Rectangle{
                id:rightSideImageRectangle
                width: parent.width- 20
                height: parent.height-20
                anchors.verticalCenter: rightSideImage.verticalCenter
                anchors.horizontalCenter: rightSideImage.horizontalCenter
                radius: 10
                color: "white"
                ImagePainter{
                    id: changedImage
                    image: histogramController.image
                    anchors.fill:parent
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }
    Rectangle{
        id:buttonSideRectangle
        width: parent.width
        height: 60
        color: "#417e56"
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        Rectangle{
            id:stretchingButtonRectangle
            width: 140
            height: 40
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 90
            Button{
                id:stretchingButton
                text: "Stretch that image"
                anchors.fill: parent
                onClicked: {
                    histogramController.stretchImage(root.imageUrl);
                }
            }
        }
        Rectangle{
            id:equalizationButtonRectangle
            width: 140
            height: 40
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 90
            Button{
                id:equalizationButton
                text: "Equalize that image"
                anchors.fill: parent
                onClicked: {
                    histogramController.equalizeImage(root.imageUrl);
                }
            }
        }
    }
    //Select a file
    FileDialog {
        id: fileDialog
        title: "Please choose a image file"
        folder: shortcuts.home
        selectMultiple: false
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrl)
            root.imageUrl = fileDialog.fileUrl
            histogramController.imageUrl= fileDialog.fileUrl;
            imageDataPath.text = fileDialog.fileUrl;
        }
        onRejected: {
            console.log("Canceled")
        }
    }
}
