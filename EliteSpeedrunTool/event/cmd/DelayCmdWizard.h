#pragma once

#include <QLabel>
#include <QSpinBox>
#include <QWizard>

class DelayCmdWizard : public QWizard {
    Q_OBJECT
public:
    DelayCmdWizard(QWidget* parent = nullptr);
    QString cmd();
};

class DelaySecondPage : public QWizardPage {
    Q_OBJECT
public:
    DelaySecondPage(QWidget* parent = nullptr);

    void initializePage();

private:
    int initValue = 10000;
    QLabel* topLabel = new QLabel(tr("请在下方输入要延时的毫秒数。"));
    QSpinBox* timeSpinBox = new QSpinBox;
};
