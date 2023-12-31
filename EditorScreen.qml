import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Dialogs

Rectangle {
    id: container
    width: 800
    height: 600
    color: "#35373B"

    property alias lineNumberFont: lineNumbersContainer.textMetrics.font
    property color lineNumberBackground: "#00000000"
    property color lineNumberColor: "#e0e0e0"
    property alias font: code.font
    property alias text: code.text
    property color textBackground: "white"
    property color textColor: "black"

    property int marginsTop: 10
    property int marginsLeft: 4
    property int lineCountWidth: 40

    property int errorLine: -1
    property string errorValue: ""
    property string codeValue: code.text
    property string logValue: ""

    function addLogValue(value) {
        container.logValue += value
    }

    function clearLog() {
        logValue = ""
    }

    function setErrorLine(value) {
        if (value > 0) {
            errorLine = value - 1
            var res = code.text.split("\n")
            errorValue = res[errorLine]
        }
        else {
            errorLine = -1
            errorValue = ""
        }

//        lineNumbers.contentY = lineNumbers.contentHeight - lineNumbers.height;
    }

    function setInitialCode(value) {
        code.text = value
    }

    RowLayout {
        id: menuFile
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 8
        anchors.leftMargin: 8
        spacing: 4

        Rectangle {
            Layout.preferredWidth: 90
            Layout.preferredHeight: 32
            border.width: 1
            border.color: "white"
            color: "#00000000"
            radius: 4

            Text {
                text: "New File"
                color: "white"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    LuaProjectManagerCpp.newFile()
                    code.focus = false
                    code.cursorPosition = -1
                }
                onDoubleClicked: {}
            }
        }

        Rectangle {
            Layout.preferredWidth: 90
            Layout.preferredHeight: 32
            border.width: 1
            border.color: "white"
            color: "#00000000"
            radius: 4

            Text {
                text: "Open File"
                color: "white"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    fileDialog.open()
                    code.focus = false
                    code.cursorPosition = -1
                }
                onDoubleClicked: {}
            }

            FileDialog {
                id: fileDialog
                title: "Select a .lua file"
                nameFilters: ["Files .lua (*.lua)"]
                onAccepted: {
                    LuaProjectManagerCpp.openFile(fileDialog.currentFile)
                }
            }
        }

        Rectangle {
            Layout.preferredWidth: 90
            Layout.preferredHeight: 32
            border.width: 1
            border.color: "white"
            color: "#00000000"
            radius: 4

            Text {
                text: "Save"
                color: "white"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    saveFileDialog.open()
                    code.focus = false
                    code.cursorPosition = -1
                }
                onDoubleClicked: {}
            }

            FileDialog {
                id: saveFileDialog
                title: "Save file *.lua"
                nameFilters: ["Files .lua (*.lua)"]
                fileMode: FileDialog.SaveFile

                onAccepted: {
                    LuaProjectManagerCpp.save(saveFileDialog.currentFile, container.codeValue)
                }
            }
        }
    }

    RowLayout {
        id: menuBuild
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 8
        anchors.rightMargin: 8
        spacing: 4

        Rectangle {
            Layout.preferredWidth: 90
            Layout.preferredHeight: 32
            border.width: 1
            border.color: "white"
            color: "#00000000"
            radius: 4

            Text {
                text: "Build"
                color: "white"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    LuaCompilatorCpp.build(codeValue)
                    code.focus = false
                    code.cursorPosition = -1
                }
                onDoubleClicked: {}
            }
        }

        Rectangle {
            Layout.preferredWidth: 90
            Layout.preferredHeight: 32
            border.width: 1
            border.color: "white"
            color: "#00000000"
            radius: 4

            Text {
                text: "Run"
                color: "white"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    LuaCompilatorCpp.run(code.text)
                    code.focus = false
                    code.cursorPosition = -1
                }
                onDoubleClicked: {}
            }
        }
    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: divider.bottom
        anchors.top: menuBuild.bottom
        anchors.topMargin: 8

        Rectangle {
            id: lineNumbersContainer
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: textMetrics.boundingRect.width + marginsTop * 2
            color: "#404244"
            property TextMetrics textMetrics: TextMetrics { text: "99999"; font: code.font }

            ListView {
                id: lineNumbers
                model: code.text.split(/\n/g)
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.topMargin: marginsTop
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: marginsTop
                width: parent.width
                clip: true

                delegate: Rectangle {
                    width: lineNumbers.width
                    height: lineNumber.height
                    color: lineNumberBackground
                    Text {
                        id: lineNumber
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: index + 1
                        color: {
                            if (code.linePosition === index) {
                                return "yellow"
                            }
                            else if (errorLine === index) {
                                return "red"
                            }
                            else {
                                return lineNumberColor
                            }
                        }
                        font: lineNumbersContainer.textMetrics.font
                    }
                }

                onContentYChanged: {
                    if (!moving) return
                    flickable.contentY = contentY
                }
                onModelChanged: {
                    if (code.linePosition === 0) {
                        lineNumbers.contentY = -1
                        if (!code.focus)
                            flickable.contentY = lineNumbers.contentY
                    }
                    else {
                        lineNumbers.contentY = flickable.contentY
                    }

                    //- lineNumbers.height;

//                    lineNumbers.contentY = lineNumbers.contentHeight * code.linePosition;
//                    console.log("code.linePosition " + code.linePosition)
//                    console.log("lineNumbers.contentY " + lineNumbers.contentY)

//                    if (!code.focus)
//                        flickable.contentY = lineNumbers.contentY
                }
            }
        }

        Rectangle {
            anchors.left: lineNumbersContainer.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: code.background.color

            Flickable {
                id: flickable
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.topMargin: marginsTop
                anchors.bottomMargin: 0
                anchors.leftMargin: 4
                clip: true


                ScrollBar.vertical: ScrollBar {
                    width: 15
                    stepSize: 0.2
                    size: 0.2
                    minimumSize: 0.2
                    interactive: true
                    active: true
                    policy: ScrollBar.AsNeeded
                }

                ScrollBar.horizontal: ScrollBar {
                    height: 15
                    stepSize: 0.2
                    size: 0.2
                    minimumSize: 0.2
                    active: true
                    policy: ScrollBar.AsNeeded
                }

                TextArea.flickable: TextArea {
                    id: code
                    width: parent.width - 15
                    height: parent.height
                    text: ""
                    textFormat: Qt.PlainText
                    focus: false
                    selectByMouse: true
                    leftPadding: 4
                    rightPadding: 4
                    topPadding: 0
                    bottomPadding: marginsTop
                    color: "white"

                    onCursorPositionChanged: {
                        code.linePosition = code.countNewLines(code.text, code.cursorPosition);
                    }

                    onTextChanged: {
                        if (errorValue !== "") {
                            var res = code.text.split("\n")
                            for (var i = 0; i < res.length; i++) {
                                if (errorValue === res[i])
                                    errorLine = i
                            }
                        }

                        code.linePosition = code.countNewLines(code.text, code.cursorPosition);
                    }

                    property int linePosition: 0

                    function countNewLines(text, position) {
                        var subText = text.substring(0, position);
                        var newLineCount = subText.split("\n").length - 1;
                        return newLineCount;
                    }
                }

                onContentYChanged: {
                    if (lineNumbers.moving) return
                    lineNumbers.contentY = contentY
                }
            }
        }
    }

    Rectangle {
        id: divider
        width: parent.width
        height: 1
        color: "black"
        anchors.bottom: scrollView.top
    }

    ScrollView {
        id: scrollView
        width: parent.width
        height: 200
        anchors.bottom:  parent.bottom
//        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.interactive: true
//        ScrollBar.horizontal.interactive: true
        padding: 0


        TextArea {
            width: scrollView.width - 20
            height: scrollView.height
            wrapMode: TextArea.Wrap
            topPadding: 8
            bottomPadding: 0
            leftPadding: 0
            readOnly: true
            background: Rectangle {
                color: "#35373B"
            }
            text: logValue
        }
    }

}


