#include "RunExeCmdWizard.h"

#include <QFileDialog>
#include <QVBoxLayout>

RunExeCmdWizard::RunExeCmdWizard(QWidget* parent)
    : QWizard { parent }
{
    setWindowTitle(tr("创建运行应用程序命令"));
    setWizardStyle(ModernStyle);
    addPage(new SelectExePage);
}

QString RunExeCmdWizard::cmd()
{
    return "start \"" + field("text").toString() + "\";";
}

QString RunExeCmdWizard::description()
{
    QFile file(field("text").toString());
    return tr("运行 %1").arg(QFileInfo(file.fileName()).fileName());
}

SelectExePage::SelectExePage(QWidget* parent)
    : QWizardPage { parent }
{
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* hPathlayout = new QHBoxLayout;
    hPathlayout->addWidget(pathLineEdit);
    hPathlayout->addWidget(selectExeButton);
    layout->addLayout(hPathlayout);
    setLayout(layout);
}

void SelectExePage::initializePage()
{
    setTitle(tr("选择应用程序"));
    setSubTitle(tr("请在下方选择可执行文件或直接输入可执行文件的路径。"));
    selectExeButton->setText("...");
    connect(selectExeButton, &QAbstractButton::clicked, this, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("选择应用程序"),
            selectExeButton->text(), tr("应用程序 (*.exe)"));
        if (!fileName.isEmpty()) {
            pathLineEdit->setText(fileName);
        }
    });
    registerField("text*", pathLineEdit);
}
