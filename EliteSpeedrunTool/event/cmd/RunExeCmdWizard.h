#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QToolButton>
#include <QWizard>

class RunExeCmdWizard : public QWizard {
    Q_OBJECT
public:
    RunExeCmdWizard(QWidget* parent = nullptr);
    QString cmd();
};

class SelectExePage : public QWizardPage {
    Q_OBJECT
public:
    SelectExePage(QWidget* parent = nullptr);

    void initializePage();

private:
    int initValue = 10000;
    QLabel* topLabel = new QLabel(tr("请在下方选择可执行文件或直接输入可执行文件的路径。"));
    QLineEdit* pathLineEdit = new QLineEdit;
    QToolButton* selectExeButton = new QToolButton;
};
