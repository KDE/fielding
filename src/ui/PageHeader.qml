// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
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

			Controller.fetch(text, {
				method: type,
				headers: {
					"Content-Type": "application/json"
				}
			})
        }

		onAccepted: {
			if(text.length === 0){
				return
			}
			searchField.makeRequest()
			runSpinner.running = true
		}
    }

    QQC2.ToolButton {
        text: i18nc("@action:intoolbar", "Submit")
        enabled: searchField.text.length > 0
        onClicked: searchField.accepted()
    }
}
