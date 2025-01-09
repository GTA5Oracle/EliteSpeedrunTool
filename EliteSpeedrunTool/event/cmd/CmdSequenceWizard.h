#pragma once

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QTableWidget>
#include <QToolButton>
#include <QWizard>

class CmdSequenceWizard : public QWizard {
    Q_OBJECT
public:
    enum { PageIntro,
        PageCmdList,
        PageInsertMode };
    CmdSequenceWizard(QString currentEvent, QWidget* parent = nullptr);

    QString cmdSequence();

    bool insertModeReplace = true;
    QString currentEvent;
};

class IntroPage : public QWizardPage {
    Q_OBJECT
public:
    IntroPage(QWidget* parent = nullptr);

    void initializePage();

private:
    QLabel* topLabel = new QLabel(tr("本向导将帮助您创建命令序列，请先确认您选择了正确的事件。"));
    QLabel* currentEventLabel = new QLabel;
};

class CmdListPage : public QWizardPage {
    Q_OBJECT
public:
    CmdListPage(QWidget* parent = nullptr);

    void initializePage();

    bool isComplete() const;

    QStringList cmdList();

private:
    QLabel* topLabel = new QLabel(tr("下方列表显示了要执行的命令序列，系统将从上往下依次执行命令，请点击下方按钮来添加每行命令。"));
    QTableWidget* cmdTable = new QTableWidget(0, 1);
    QToolButton* removeLineToolButton = new QToolButton;
    QToolButton* upLineToolButton = new QToolButton;
    QToolButton* downLineToolButton = new QToolButton;
    QToolButton* addCmdButton = new QToolButton;

    void addLineToTable(QString cmd);
    bool upTableLine(int row);
    void updateToolButtonState(QTableWidgetItem* current);
    void initAddCmdMenu();
    void updateWizardNextButtonEnabled(bool enabled);
};

class InsertModePage : public QWizardPage {
    Q_OBJECT
public:
    InsertModePage(QWidget* parent = nullptr);

    void initializePage();

private:
    QLabel* topLabel = new QLabel(tr("是否使用新命令覆盖已有的命令？"));
    QRadioButton* replaceRadioButton = new QRadioButton(tr("替换已有命令"));
    QRadioButton* appendRadioButton = new QRadioButton(tr("在已有命令后面追加"));
};
