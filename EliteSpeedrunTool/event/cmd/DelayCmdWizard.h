#pragma once

#include <QSpinBox>
#include <QWizard>

class DelayCmdWizard : public QWizard {
    Q_OBJECT
public:
    DelayCmdWizard(QWidget* parent = nullptr);
    QString cmd();
    QString description();
};

class DelaySecondPage : public QWizardPage {
    Q_OBJECT
public:
    DelaySecondPage(QWidget* parent = nullptr);

    void initializePage();

private:
    int initValue = 10000;
    QSpinBox* timeSpinBox = new QSpinBox;
};
