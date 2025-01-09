#include "CmdSequenceWizard.h"

#include "DelayCmdWizard.h"
#include "RunExeCmdWizard.h"
#include "ShutdownCmdWizard.h"

#include <QHeaderView>
#include <QInputDialog>
#include <QMenu>
#include <QVBoxLayout>

CmdSequenceWizard::CmdSequenceWizard(QString currentEvent, QWidget* parent)
    : QWizard { parent }
    , currentEvent { currentEvent }
{
    setWindowTitle(tr("创建命令序列向导"));
    setWizardStyle(ModernStyle);
    setPage(PageIntro, new IntroPage);
    setPage(PageCmdList, new CmdListPage);
    setPage(PageInsertMode, new InsertModePage);
    setStartId(PageIntro);
}

QString CmdSequenceWizard::cmdSequence()
{
    return dynamic_cast<CmdListPage*>(page(PageCmdList))->cmdList().join("\n");
}

IntroPage::IntroPage(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(currentEventLabel);
    setLayout(layout);
}

void IntroPage::initializePage()
{
    setTitle(tr("介绍"));
    topLabel->setWordWrap(true);
    currentEventLabel->setWordWrap(true);

    auto w = dynamic_cast<CmdSequenceWizard*>(wizard());
    currentEventLabel->setText(tr("您将为“%1”事件创建命令").arg(w->currentEvent));
    auto eventFont = currentEventLabel->font();
    eventFont.setBold(true);
    eventFont.setPointSize(12);
    currentEventLabel->setFont(eventFont);
}

CmdListPage::CmdListPage(QWidget* parent)
{
    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(topLabel);

    QHBoxLayout* hLayout = new QHBoxLayout;
    vLayout->addLayout(hLayout);
    QVBoxLayout* vLayoutLeft = new QVBoxLayout;
    QVBoxLayout* vToolButtonsLayout = new QVBoxLayout;
    hLayout->addLayout(vLayoutLeft);
    hLayout->addLayout(vToolButtonsLayout);
    vLayoutLeft->addWidget(cmdTable);
    vLayoutLeft->addWidget(addCmdButton);

    vToolButtonsLayout->addWidget(upLineToolButton);
    vToolButtonsLayout->addWidget(downLineToolButton);
    vToolButtonsLayout->addWidget(removeLineToolButton);
    vToolButtonsLayout->addSpacerItem(new QSpacerItem(20, 1, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding));

    setLayout(vLayout);
}

void CmdListPage::initializePage()
{
    setTitle(tr("命令序列"));

    topLabel->setWordWrap(true);

    cmdTable->setHorizontalHeaderLabels({ tr("命令") });
    cmdTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    removeLineToolButton->setIcon(QIcon("://image/ic_cancel.ico"));
    downLineToolButton->setArrowType(Qt::DownArrow);
    upLineToolButton->setArrowType(Qt::UpArrow);
    removeLineToolButton->setEnabled(false);
    downLineToolButton->setEnabled(false);
    upLineToolButton->setEnabled(false);

    connect(removeLineToolButton, &QAbstractButton::clicked, this, [this]() {
        auto currentRow = cmdTable->currentRow();
        if (currentRow >= 0) {
            cmdTable->removeRow(cmdTable->currentRow());
        }
    });
    connect(downLineToolButton, &QAbstractButton::clicked, this, [this]() {
        if (upTableLine(cmdTable->currentRow() + 1)) {
            auto curItem = cmdTable->currentItem();
            cmdTable->setCurrentItem(cmdTable->item(curItem->row() + 1, curItem->column()));
        }
    });
    connect(upLineToolButton, &QAbstractButton::clicked, this, [this]() {
        if (upTableLine(cmdTable->currentRow())) {
            auto curItem = cmdTable->currentItem();
            cmdTable->setCurrentItem(cmdTable->item(curItem->row() - 1, curItem->column()));
        }
    });

    connect(cmdTable, &QTableWidget::currentItemChanged, this, [this](QTableWidgetItem* current, QTableWidgetItem* previous) {
        updateToolButtonState(current);
    });
    connect(cmdTable->model(), &QAbstractItemModel::rowsRemoved, this, [this](const QModelIndex& parent, int first, int last) {
        updateToolButtonState(cmdTable->currentItem());
        emit completeChanged();
    });
    connect(cmdTable->model(), &QAbstractItemModel::rowsInserted, this, [this](const QModelIndex& parent, int first, int last) {
        emit completeChanged();
    });
    connect(cmdTable, &QTableWidget::cellChanged, this, [this](int row, int column) {
        if (column == 0) {
            auto item = cmdTable->item(row, column);
            if (item) {
                auto text = item->text();
                text = text.trimmed();
                if (!text.endsWith(";")) {
                    text = text + ";";
                }
                item->setText(text);
            }
        }
    });

    addCmdButton->setText(tr("添加一行命令"));
    initAddCmdMenu();
}

bool CmdListPage::isComplete() const
{
    bool defaultComplete = QWizardPage::isComplete();
    if (defaultComplete) {
        if (cmdTable->rowCount() == 0) {
            return false;
        }
    }
    return defaultComplete;
}

QStringList CmdListPage::cmdList()
{
    QStringList list;
    for (int i = 0; i < cmdTable->rowCount(); i++) {
        list << cmdTable->item(i, 0)->text();
    }
    return list;
}

void CmdListPage::addLineToTable(QString cmd)
{
    if (cmd.isEmpty()) {
        return;
    }
    int row = cmdTable->currentRow() + 1;
    cmdTable->insertRow(row);
    auto item = new QTableWidgetItem(cmd);
    cmdTable->setItem(row, 0, item);
    if (row == cmdTable->rowCount() - 1) {
        cmdTable->setCurrentItem(item);
    }
}

bool CmdListPage::upTableLine(int row)
{
    if (row > 0 && row < cmdTable->rowCount()) {
        for (int col = 0; col < cmdTable->columnCount(); col++) {
            QTableWidgetItem* item1 = cmdTable->takeItem(row, col);
            QTableWidgetItem* item2 = cmdTable->takeItem(row - 1, col);
            cmdTable->setItem(row - 1, col, item1);
            cmdTable->setItem(row, col, item2);
        }
        return true;
    }
    return false;
}

void CmdListPage::updateToolButtonState(QTableWidgetItem* current)
{
    removeLineToolButton->setEnabled(current);
    downLineToolButton->setEnabled(current && cmdTable->row(current) < cmdTable->rowCount() - 1);
    upLineToolButton->setEnabled(current && cmdTable->row(current) > 0);
}

void CmdListPage::initAddCmdMenu()
{
    QList<QPair<QString, std::function<QString()>>> actionList = {
        qMakePair(tr("直接输入命令..."), []() {
            bool ok;
            QString text = QInputDialog::getText(nullptr, "", "请输入一行命令", QLineEdit::Normal, "", &ok);
            return ok ? text : QString("");
        }),
        qMakePair(tr("通过 Steam 启动 GTA5"), []() { return "start \"steam://rungameid/271590\";"; }),
        qMakePair(tr("通过 Epic 启动 GTA5"), []() { return "start \"com.epicgames.launcher://apps/0584d2013f0149a791e7b9bad0eec102%3A6e563a2c0f5f46e3b4e88b5f4ed50cca%3A9d2d0eb64d5c44529cece33fe2a46482?action=launch&silent=true\";"; }),
        qMakePair(tr("启动 MSI Afterburner"), []() { return "start \"${env:ProgramFiles(x86)}/MSI Afterburner/MSIAfterburner.exe\";"; }),
        qMakePair(tr("延时..."), []() {
            DelayCmdWizard w;
            return w.exec() == QDialog::Accepted ? w.cmd() : QString("");
        }),
        qMakePair(tr("执行应用程序..."), []() {
            RunExeCmdWizard w;
            return w.exec() == QDialog::Accepted ? w.cmd() : QString("");
        }),
        qMakePair(tr("关机..."), []() {
            ShutdownCmdWizard w;
            return w.exec() == QDialog::Accepted ? w.cmd() : QString("");
        }),
        qMakePair(tr("取消存在的关机计划"), []() { return "shutdown -a;"; }),
    };

    QMenu* menu = new QMenu(this);
    for (auto i = actionList.constBegin(); i != actionList.constEnd(); i++) {
        QAction* action = menu->addAction(i->first);
        auto func = i->second;
        connect(action, &QAction::triggered, this, [this, func]() { addLineToTable(func()); });
    }
    addCmdButton->setMenu(menu);
    addCmdButton->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);
}

void CmdListPage::updateWizardNextButtonEnabled(bool enabled)
{
    wizard()->button(QWizard::NextButton)->setEnabled(enabled);
    wizard()->button(QWizard::FinishButton)->setEnabled(enabled);
    wizard()->button(QWizard::CommitButton)->setEnabled(enabled);
}

InsertModePage::InsertModePage(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(replaceRadioButton);
    layout->addWidget(appendRadioButton);
    setLayout(layout);
}

void InsertModePage::initializePage()
{
    setTitle(tr("命令插入方式"));
    topLabel->setWordWrap(true);
    CmdSequenceWizard* w = dynamic_cast<CmdSequenceWizard*>(wizard());
    replaceRadioButton->setChecked(w->insertModeReplace);
    appendRadioButton->setChecked(!w->insertModeReplace);

    connect(replaceRadioButton, &QAbstractButton::toggled, this, [this, w](bool checked) {
        w->insertModeReplace = checked;
    });
}
