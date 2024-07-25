#pragma once

#include "ui_AcknowledgementDialog.h"
#include <QDialog>

class AcknowledgementDialog : public QDialog {
    Q_OBJECT

public:
    explicit AcknowledgementDialog(QWidget* parent = nullptr);

private:
    Ui::AcknowledgementDialog ui;

    QList<QString> techHelpers = {
        "007",
        "AdstarGod",
        "Capucino_ovo",
        "cc0",
        "cocoa184684",
        "CZ9946",
        "Exce1sior",
        "limerenxey",
        "Mannlicher",
        "QuoVadis8",
        "RA东方",
        "VERY-LONELY",
        "KK",
        "xiakeyi",
        "Alice2333",
    };

    QList<QString> sponsors = {
        "CZ9946",
        "FairyFan-寒冰",
        "青霉素",
        "AdstarGod",
        "呆漂亮",
        "FDAFDAFSDAF",
        "RedBull",
        "Chou",
        "____TTx28____",
        "RA东方",
        "Easy",
        "Zeroday",
        "Azusa ～",
    };
};
