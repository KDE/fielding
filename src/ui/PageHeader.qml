// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami

import org.kde.fielding

RowLayout {
    id: pageHeader

    Layout.fillWidth: true
    spacing: Kirigami.Units.smallSpacing / 2

    QQC2.ComboBox {
        Layout.maximumWidth: Kirigami.Units.largeSpacing * 10
        model: ["GET", "POST", "PUT", "PATCH", "DELETE"]
        currentIndex: Config.method
        onCurrentIndexChanged: {
            Config.method = currentIndex
            Config.save()
        }
    }

    Kirigami.ActionTextField {
        id: searchField
        Layout.fillWidth: true

        placeholderText: i18n("Enter URL here…")

        function makeRequest() {
            let type;

            switch (Config.method) {
                case 0:
                    type = "get"
                    break;
                case 1:
                    type = "post"
                    break;
                case 2:
                    type = "put"
                    break;
                case 3:
                    type = "patch"
                    break;
                case 4:
                    type = "delete"
                    break;
            }

            if (text.length > 0) {
                Controller.fetch(text, {
                    method: type,
                    headers: {
                        "Content-Type": "application/json"
                    }
                })
            }
        }

        onAccepted: searchField.makeRequest(), runSpinner.running = true
    }

    QQC2.ToolButton {
        text: i18nc("@action:intoolbar", "Submit")
        onClicked: searchField.makeRequest(), runSpinner.running = true
    }

    QQC2.ToolButton {
        id: menuButton

        focusPolicy: Qt.NoFocus

        display: QQC2.AbstractButton.IconOnly
        down: applicationMenu.opened
        action: Kirigami.Action {
            text: i18n("Menu")
            icon.name: "application-menu-symbolic"
            onTriggered: applicationMenu.popup(menuButton, 0.0, menuButton.height)
        }

        QQC2.Menu {
            id: applicationMenu
            QQC2.MenuItem {
                action: Kirigami.Action {
                    text: i18nc("@action:inmenu", "About Fielding")
                    icon.name: "org.kde.fielding"
                    shortcut: StandardKey.HelpContents
                    onTriggered: pageStack.layers.push(Qt.createComponent("org.kde.kirigamiaddons.formcard", "AboutPage"))
                }
            }
            QQC2.MenuItem {
                text: i18nc("@action:inmenu", "About KDE")
                icon.name: "kdeapp"
                onClicked: pageStack.layers.push(Qt.createComponent("org.kde.kirigamiaddons.formcard", "AboutKDEPage"))
            }
            QQC2.MenuItem {
                action: Kirigami.Action {
                    icon.name: "application-exit"
                    text: i18nc("@action:inmenu", "Quit")
                    shortcut: StandardKey.Quit
                    onTriggered: Qt.quit()
                }
            }
        }

        QQC2.ToolTip.visible: hovered
        QQC2.ToolTip.text: text
        QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
    }
}
