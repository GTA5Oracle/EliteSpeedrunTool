#include "AcknowledgementDialog.h"

AcknowledgementDialog::AcknowledgementDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    techHelpers.sort();
    sponsors.sort();

    ui.textEdit->setText(
        "<p>" + tr("特别感谢 ACT3 帮会和 CEHT 帮会对本工具的宣传和支持。特别感谢 CZ9946 和 Adstargod 两位大佬，他们对本工具的研发提供了大量的帮助！特别感谢 Alice2333 对本工具的开发提供技术支持！") + "</p>"
        + "<p>" + tr("感谢所有提供帮助与赞助的朋友们！") + "</p>"
        + "<h3>" + tr("测试/技术支持：") + "</h3>"
        + techHelpers.join("<br/>")
        + "<h3>" + tr("赞助支持：") + "</h3>"
        + sponsors.join("<br/>") + "<br/><br/>"
        + tr("以及其他未透露 / 不愿透露昵称的朋友！"));
}
