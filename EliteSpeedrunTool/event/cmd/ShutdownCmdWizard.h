#pragma once

#include <QSpinBox>
#include <QWizard>

class ShutdownCmdWizard : public QWizard {
    Q_OBJECT
public:
    ShutdownCmdWizard(QWidget* parent = nullptr);
    QString cmd();
    QString description();
};

class ShutdownDelaySecondPage : public QWizardPage {
    Q_OBJECT
public:
    ShutdownDelaySecondPage(QWidget* parent = nullptr);

    void initializePage();

private:
    int initValue = 60000;
    QSpinBox* timeSpinBox = new QSpinBox;
};
