#include "ShutdownCmdWizard.h"

#include <QVBoxLayout>

ShutdownCmdWizard::ShutdownCmdWizard(QWidget* parent)
    : QWizard { parent }
{
    setWindowTitle(tr("创建关机命令"));
    setWizardStyle(ModernStyle);
    addPage(new ShutdownDelaySecondPage);
}

QString ShutdownCmdWizard::cmd()
{
    return "shutdown -s -t " + QString::number(field("value").toInt()) + ";";
}

QString ShutdownCmdWizard::description()
{
    return tr("%1 秒后关机").arg(QString::number(field("value").toInt()));
}

ShutdownDelaySecondPage::ShutdownDelaySecondPage(QWidget* parent)
    : QWizardPage { parent }
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(timeSpinBox);
    setLayout(layout);
}

void ShutdownDelaySecondPage::initializePage()
{
    setTitle(tr("等待时间"));
    setSubTitle(tr("请在下方输入关机前要等待的秒数。"));
    registerField("value", timeSpinBox);
    timeSpinBox->setMaximum(600);
    timeSpinBox->setMinimum(0);
    timeSpinBox->setValue(initValue);
}
