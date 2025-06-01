#pragma once

#include "FirewallRule.h"
#include <QTableWidget>

class FirewallPathTableWidget : public QTableWidget {
    Q_OBJECT
public:
    FirewallPathTableWidget(QWidget* parent = nullptr);

    void addNewRow(FirewallRule* rule);

private:
    FirewallRule* getRuleAt(int rowIndex);

signals:
    void onDelete(FirewallRule* rule);
    void onChanged(FirewallRule* oldRule, FirewallRule* newRule);
};
