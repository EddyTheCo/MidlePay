import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import MyDesigns
import midlePay


Popup {
    id: root

    property alias address:pay.address
    property alias nodeaddr:pay.nodeaddr
    property alias initTag:pay.initTag
    property alias amount:pay.amount

    background: Rectangle
    {
        id:bck
        color:CustomStyle.backColor1
        border.width:1
        border.color:CustomStyle.frontColor1
        radius:Math.min(width,height)*0.05

    }
    MidlePay
    {
        id:pay
    }

    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    visible: ! pay.pass
    anchors.centerIn: Overlay.overlay

    ColumnLayout
    {
        anchors.fill: parent
        spacing:10
        MyTextArea
        {
            id:tex
            Layout.fillWidth: true
            Layout.fillHeight:  true
            Layout.alignment: Qt.AlignTop|Qt.AlignHCenter
            Layout.minimumHeight:  100
            label.visible: false
            textarea.text: pay.message
            textarea.readOnly: true
            textarea.wrapMode: Text.Wrap
        }

        MyAddressQr
        {
            id:qrcode_
            address:pay.address
            Layout.preferredWidth: tex.width*0.55
            Layout.preferredHeight: width

            Layout.alignment: Qt.AlignCenter
        }
    }



}
