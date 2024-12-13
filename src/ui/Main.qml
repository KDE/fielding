// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.syntaxhighlighting 1.0

import org.kde.fielding
import org.kde.fielding.config

Kirigami.ApplicationWindow {
    id: root

    title: i18n("Fielding")

    width: Kirigami.Units.gridUnit * 42
    height: Kirigami.Units.gridUnit * 32
    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    Timer {
        id: saveWindowGeometryTimer
        interval: 1000
        onTriggered: App.saveWindowGeometry(root)
    }

    Connections {
        id: saveWindowGeometryConnections
        enabled: false // Disable on startup to avoid writing wrong values if the window is hidden
        target: root

        function onClosing() { App.saveWindowGeometry(root) }
        function onWidthChanged() { saveWindowGeometryTimer.restart() }
        function onHeightChanged() { saveWindowGeometryTimer.restart() }
        function onXChanged() { saveWindowGeometryTimer.restart() }
        function onYChanged() { saveWindowGeometryTimer.restart() }
    }

    Loader {
        active: !Kirigami.Settings.isMobile
        source: Qt.resolvedUrl("GlobalMenu.qml")
    }

    pageStack.initialPage: Kirigami.Page {
        id: page

        padding: 0
        titleDelegate: PageHeader {}

        QQC2.SplitView {
            anchors.fill: parent

            handle: Rectangle {
                implicitWidth: Kirigami.Units.largeSpacing
                implicitHeight: Kirigami.Units.largeSpacing
                color: Kirigami.Theme.backgroundColor

                Kirigami.Separator {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                }

                Kirigami.Separator {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                }
            }

            QQC2.ScrollView {
                QQC2.SplitView.minimumWidth: Kirigami.Units.gridUnit * 10
                QQC2.SplitView.preferredWidth: Kirigami.Units.gridUnit * 17
                QQC2.SplitView.fillHeight: true

                QQC2.TextArea {
                    id: bodyTextArea

                    wrapMode: Text.Wrap

                    onEditingFinished: {
                        if (text.length > 0) {
                            Controller.setData(JSON.parse(text))
                        }
                    }

                    placeholderText: i18n("Request body...")

                    background: Rectangle {
                        Kirigami.Theme.colorSet: Kirigami.Theme.View
                        Kirigami.Theme.inherit: false
                        color: Kirigami.Theme.backgroundColor
                    }

                    Kirigami.SpellCheck.enabled: false

                    SyntaxHighlighter {
                        textEdit: bodyTextArea
                        definition: "JSON"
                    }
                }
            }

            QQC2.ScrollView {
                QQC2.SplitView.fillWidth: true
                QQC2.SplitView.fillHeight: true
                QQC2.SplitView.minimumWidth: Kirigami.Units.gridUnit * 10

                QQC2.TextArea {
                    id: responseTextArea

                    wrapMode: Text.Wrap

                    readOnly: true

                    background: Rectangle {
                        Kirigami.Theme.colorSet: Kirigami.Theme.View
                        Kirigami.Theme.inherit: false
                        color: Kirigami.Theme.backgroundColor

                        Kirigami.PlaceholderMessage {
                            anchors.centerIn: parent
                            visible: responseTextArea.text.length <= 0
                            width: parent.width - Kirigami.Units.largeSpacing * 4
                            text: i18n("No Response Yet")
                            explanation: i18n("Your request response will appear here")
                        }
                    }

                    QQC2.BusyIndicator {
                        id: runSpinner
                        anchors.centerIn: parent
                        width: parent.width - Kirigami.Units.largeSpacing * 4
                        running: false
                    }

                    SyntaxHighlighter {
                        id: responseHighlighter

                        textEdit: responseTextArea
                    }

                    Connections {
                        target: Controller

                        function onResponse(response: string, definition: string): void {
                            responseTextArea.text = response;
                            responseHighlighter.definition = definition;
                            runSpinner.running = false;
                        }
                    }
                }
            }
        }

        footer: QQC2.ToolBar {
            implicitHeight: content.implicitHeight + Kirigami.Units.smallSpacing * 2
            position: QQC2.ToolBar.Footer

            RowLayout {
                id: content
                anchors.fill: parent

                QQC2.Label {
                    id: statusLabel
                    Layout.alignment: Qt.AlignLeft
                    visible: false
                    text: i18n("Status: ")
                }
                QQC2.Label {
                    id: statusLabelCodeText
                    Layout.alignment: Qt.AlignLeft
                }

                Item {
                    Layout.fillWidth: true
                }

                Connections {
                    target: Controller

                    function onStatus(statusCode, statusText) {
                        statusLabelCodeText.text = `${statusCode} ${statusText}`
                        statusLabel.visible = true

                        if (statusCode >= 100 && statusCode <= 199) {
                            statusLabelCodeText.color = Kirigami.Theme.disabledTextColor
                        } else if (statusCode >= 200 && statusCode <= 299) {
                            statusLabelCodeText.color = Kirigami.Theme.positiveTextColor
                        } else if (statusCode >= 300 && statusCode <= 399) {
                            statusLabelCodeText.color = Kirigami.Theme.linkColor
                        } else if (statusCode >= 400 && statusCode <= 499) {
                            statusLabelCodeText.color = Kirigami.Theme.negativeTextColor
                        } else if (statusCode >= 500 && statusCode <= 599) {
                            statusLabelCodeText.color = Kirigami.Theme.neutralTextColor
                        } else {
                            statusLabelCodeText.color = Kirigami.Theme.textColor
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        if (!Kirigami.Settings.isMobile) {
            saveWindowGeometryConnections.enabled = true
        }
    }
}
