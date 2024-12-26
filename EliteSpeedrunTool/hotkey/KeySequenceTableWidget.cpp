#include "KeySequenceTableWidget.h"
#include <QCheckBox>
#include <QHeaderView>
#include <QKeySequenceEdit>
#include <QToolButton>

KeySequenceTableWidget::KeySequenceTableWidget(QWidget* parent)
    : QTableWidget(0, 2, parent)
{
    setHorizontalHeaderLabels({ tr("快捷键"), tr("操作") });
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void KeySequenceTableWidget::addNewRow(QKeySequence keySeq)
{
    int row = rowCount();
    insertRow(row);

    QKeySequenceEdit* keySequenceEdit = new QKeySequenceEdit(this);
    connect(keySequenceEdit, &QKeySequenceEdit::editingFinished, this, [this, keySequenceEdit, keySeq]() {
        auto keySequence = keySequenceEdit->keySequence();
        if (keySequence.isEmpty()) {
            if (!keySeq.isEmpty()) {
                emit onDelete(keySeq);
            }
            removeRow(indexAt(keySequenceEdit->pos()).row());
        } else {
            if (keySeq.isEmpty()) {
                emit onAdd(keySequence);
            } else {
                emit onChanged(keySeq, keySequence);
            }
        }
    });
    setCellWidget(row, 0, keySequenceEdit);

    QToolButton* deleteButton = new QToolButton(this);
    deleteButton->setIcon(QIcon(":/image/ic_cancel.ico"));
    connect(deleteButton, &QAbstractButton::clicked, this, [this, keySequenceEdit, deleteButton, keySeq]() {
        if (!keySeq.isEmpty()) {
            emit onDelete(keySequenceEdit->keySequence());
        }
        removeRow(indexAt(deleteButton->pos()).row());
    });
    setCellWidget(row, 1, deleteButton);
    if (!keySeq.isEmpty()) {
        keySequenceEdit->setKeySequence(QKeySequence(keySeq));
    } else {
        keySequenceEdit->setFocus();
    }
}
