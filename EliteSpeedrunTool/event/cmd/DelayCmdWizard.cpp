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
    return QString("Start-Sleep -Milliseconds " + QString::number(field("value").toInt()) + ";");
}

DelaySecondPage::DelaySecondPage(QWidget* parent)
    : QWizardPage { parent }
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(timeSpinBox);
    setLayout(layout);
}

void DelaySecondPage::initializePage()
{
    setTitle(tr("延时时间"));
    topLabel->setWordWrap(true);
    registerField("value", timeSpinBox);
    timeSpinBox->setMaximum(100000);
    timeSpinBox->setMinimum(1);
    timeSpinBox->setValue(initValue);
}
