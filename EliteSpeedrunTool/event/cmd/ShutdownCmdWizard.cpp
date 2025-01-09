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
    return QString("shutdown -s -t " + QString::number(field("value").toInt()) + ";");
}

ShutdownDelaySecondPage::ShutdownDelaySecondPage(QWidget* parent)
    : QWizardPage { parent }
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(timeSpinBox);
    setLayout(layout);
}

void ShutdownDelaySecondPage::initializePage()
{
    setTitle(tr("等待时间"));
    topLabel->setWordWrap(true);
    registerField("value", timeSpinBox);
    timeSpinBox->setMaximum(600);
    timeSpinBox->setMinimum(0);
    timeSpinBox->setValue(initValue);
}
