#include "DelayCmdWizard.h"

#include <QVBoxLayout>

DelayCmdWizard::DelayCmdWizard(QWidget* parent)
    : QWizard { parent }
{
    setWindowTitle(tr("创建延时命令"));
    setWizardStyle(ModernStyle);
    addPage(new DelaySecondPage);
}

QString DelayCmdWizard::cmd()
{
    return "Start-Sleep -Milliseconds " + QString::number(field("value").toInt()) + ";";
}

QString DelayCmdWizard::description()
{
    return tr("延时 %1 毫秒").arg(QString::number(field("value").toInt()));
}

DelaySecondPage::DelaySecondPage(QWidget* parent)
    : QWizardPage { parent }
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(timeSpinBox);
    setLayout(layout);
}

void DelaySecondPage::initializePage()
{
    setTitle(tr("延时时间"));
    setSubTitle(tr("请在下方输入要延时的毫秒数。"));
    registerField("value", timeSpinBox);
    timeSpinBox->setMaximum(100000);
    timeSpinBox->setMinimum(1);
    timeSpinBox->setValue(initValue);
}
