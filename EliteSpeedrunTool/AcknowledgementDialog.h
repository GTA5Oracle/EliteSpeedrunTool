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
        "愚蠢的土拨鼠",
        "康康",
        "_-K32-_",
        "LanChaTu-C",
        "StudenT_Tan02",
        "steeptakagi",
        "AnotherDaaay27",
        "wdxdzl",
        "___Sakiko___",
        "Going_Bad",
        "colourpen907",
        "-oll006-",
        "___Ady-sxr___",
        "安琪拉",
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
        "-neverRE",
        "愚蠢的土拨鼠",
        "12",
        "xiakeyi",
        "超级无敌大王",
        "老司机",
        "Roffzei",
        "____-669-____",
        "巨大糖丸🐷",
        "GodfatherCDW",
        "9527_SSR",
        "Capucino_ovo",
        "laosepi1996",
        "dqyxsagain",
        "0528",
        "柴柴游戏君",
        "Henry_Li",
        "因帅被罚五十块",
        "千棘元婴修士",
        "浮华",
    };
};
