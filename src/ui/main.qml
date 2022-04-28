// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.syntaxhighlighting 1.0

import org.kde.roy 1.0

Kirigami.ApplicationWindow {
    id: root

    title: i18n("Roy")

    width: Kirigami.Units.gridUnit * 42
    height: Kirigami.Units.gridUnit * 32
    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

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
                visible: Config.method != 0 && Config.method != 4

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

                    background: Rectangle {
                        Kirigami.Theme.colorSet: Kirigami.Theme.View
                        Kirigami.Theme.inherit: false
                        color: Kirigami.Theme.backgroundColor

                        Kirigami.PlaceholderMessage {
                            anchors.centerIn: parent
                            width: parent.width - Kirigami.Units.largeSpacing * 4
                            visible: bodyTextArea.text.length <= 0
                            text: i18n("Body Text")
                        }
                    }

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

                    SyntaxHighlighter {
                        textEdit: responseTextArea
                        definition: "JSON"
                    }

                    Connections {
                        target: Controller

                        function onResponse(response) {
                            responseTextArea.text = response
                        }
                    }
                }
            }
        }
    }
}
