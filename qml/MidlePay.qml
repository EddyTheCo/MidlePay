import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import MyDesigns
import midlePay


Popup {
    id: root

    property alias address:MidlePay.address
    property alias nodeaddr:MidlePay.nodeaddr
    property alias initTag:MidlePay.initTag
    property alias amount:MidlePay.amount

    background: Rectangle
    {
        id:bck
        color:CustomStyle.backColor1
        border.width:1
        border.color:CustomStyle.frontColor1
        radius:Math.min(width,height)*0.05

    }

    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    visible: ! MidlePay.pass

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
            textarea.text: MidlePay.message
            textarea.readOnly: true
            textarea.wrapMode: Text.Wrap
        }

        MyAddressQr
        {
            id:qrcode_
            address:MidlePay.address
            Layout.preferredWidth: tex.width*0.55
            Layout.preferredHeight: width

            Layout.alignment: Qt.AlignCenter
        }
    }



}
