#pragma once

#include <QTableWidget>

class KeySequenceTableWidget : public QTableWidget {
    Q_OBJECT
public:
    KeySequenceTableWidget(QWidget* parent = nullptr);

    void addNewRow(QKeySequence keySeq);

signals:
    void onDelete(QKeySequence keySeq);
    void onChanged(QKeySequence oldKeySeq, QKeySequence newKeySeq);
    void onAdd(QKeySequence keySeq);
};
