#pragma once

#include <QLineEdit>
#include <QSpinBox>
#include <QToolButton>
#include <QWizard>

class RunExeCmdWizard : public QWizard {
    Q_OBJECT
public:
    RunExeCmdWizard(QWidget* parent = nullptr);
    QString cmd();
    QString description();
};

class SelectExePage : public QWizardPage {
    Q_OBJECT
public:
    SelectExePage(QWidget* parent = nullptr);

    void initializePage();

private:
    int initValue = 10000;
    QLineEdit* pathLineEdit = new QLineEdit;
    QToolButton* selectExeButton = new QToolButton;
};
