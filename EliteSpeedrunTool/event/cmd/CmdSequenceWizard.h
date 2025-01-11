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
    QTableWidget* cmdTable = new QTableWidget(0, 2);
    QToolButton* removeLineToolButton = new QToolButton;
    QToolButton* upLineToolButton = new QToolButton;
    QToolButton* downLineToolButton = new QToolButton;
    QToolButton* addCmdButton = new QToolButton;

    void addLineToTable(QString cmd, QString description);
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
    QRadioButton* replaceRadioButton = new QRadioButton(tr("替换已有命令"));
    QRadioButton* appendRadioButton = new QRadioButton(tr("在已有命令后面追加"));
};
