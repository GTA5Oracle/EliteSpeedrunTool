#pragma once

#include <QLabel>
#include <QSpinBox>
#include <QWizard>

class ShutdownCmdWizard : public QWizard {
    Q_OBJECT
public:
    ShutdownCmdWizard(QWidget* parent = nullptr);
    QString cmd();
};

class ShutdownDelaySecondPage : public QWizardPage {
    Q_OBJECT
public:
    ShutdownDelaySecondPage(QWidget* parent = nullptr);

    void initializePage();

private:
    int initValue = 60000;
    QLabel* topLabel = new QLabel(tr("请在下方输入关机前要等待的秒数。"));
    QSpinBox* timeSpinBox = new QSpinBox;
};
