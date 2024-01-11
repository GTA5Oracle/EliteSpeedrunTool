#pragma once

#include "ui_DescriptionDialog.h"
#include <QDialog>

class DescriptionDialog : public QDialog {
    Q_OBJECT

public:
    explicit DescriptionDialog(QWidget* parent = nullptr);
    ~DescriptionDialog();

private:
    Ui::DescriptionDialog ui;
};
