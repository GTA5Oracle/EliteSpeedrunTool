#include "FirewallPathTableWidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QSpinBox>
#include <QToolButton>
#include <icftypes.h>

FirewallPathTableWidget::FirewallPathTableWidget(QWidget* parent)
    : QTableWidget(0, 4, parent)
{
    setHorizontalHeaderLabels({ tr("可执行程序路径"), tr("方向"), tr("协议"), tr("操作") });
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void FirewallPathTableWidget::addNewRow(FirewallRule* rule)
{
    int row = rowCount();
    insertRow(row);

    QLineEdit* lineEdit = new QLineEdit(this);
    lineEdit->setProperty("rule", QVariant::fromValue(rule));
    lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\.exe$", QRegularExpression::CaseInsensitiveOption)));

    QComboBox* cbDirection = new QComboBox(this);
    cbDirection->addItem(tr("出站"), NET_FW_RULE_DIR_OUT);
    cbDirection->addItem(tr("入站"), NET_FW_RULE_DIR_IN);

    QSpinBox* sbProtocol = new QSpinBox(this);
    sbProtocol->setMinimum(-1);
    sbProtocol->setMaximum(255);

    QToolButton* deleteButton = new QToolButton(this);
    deleteButton->setIcon(QIcon(":/image/ic_cancel.ico"));

    if (!rule->path.isEmpty()) {
        lineEdit->setText(rule->path);
    } else {
        lineEdit->setFocus();
    }
    cbDirection->setCurrentIndex(cbDirection->findData(rule->direction));
    sbProtocol->setValue(rule->protocol);

    auto onChangedCall = [this, lineEdit]() {
        auto rule = lineEdit->property("rule").value<FirewallRule*>();
        int rowIndex = indexAt(lineEdit->pos()).row();
        auto newRule = getRuleAt(rowIndex);
        lineEdit->setProperty("rule", QVariant::fromValue(newRule));
        emit onChanged(rule, newRule);
    };

    connect(lineEdit, &QLineEdit::editingFinished, this, onChangedCall);

    connect(cbDirection, &QComboBox::currentIndexChanged, this, [onChangedCall](int) {
        onChangedCall();
    });

    connect(sbProtocol, &QSpinBox::valueChanged, this, [onChangedCall](int) {
        onChangedCall();
    });

    connect(deleteButton, &QAbstractButton::clicked, this, [this, lineEdit, deleteButton]() {
        auto lastPath = lineEdit->property("rule").value<FirewallRule*>();
        emit onDelete(lastPath);
        removeRow(indexAt(deleteButton->pos()).row());
    });

    setCellWidget(row, 0, lineEdit);
    setCellWidget(row, 1, cbDirection);
    setCellWidget(row, 2, sbProtocol);
    setCellWidget(row, 3, deleteButton);
}

FirewallRule* FirewallPathTableWidget::getRuleAt(int rowIndex)
{
    auto lineEdit = dynamic_cast<QLineEdit*>(cellWidget(rowIndex, 0));
    auto cbDirection = dynamic_cast<QComboBox*>(cellWidget(rowIndex, 1));
    auto sbProtocol = dynamic_cast<QSpinBox*>(cellWidget(rowIndex, 2));
    return new FirewallRule(lineEdit->property("rule").value<FirewallRule*>()->path,
        cbDirection->currentData().toInt(),
        sbProtocol->value());
}
