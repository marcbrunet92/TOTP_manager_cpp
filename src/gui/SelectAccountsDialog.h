#ifndef SELECTACCOUNTDIALOG_H
#define SELECTACCOUNTDIALOG_H

#include <QDialog>
#include <QVariantMap>
#include <vector>

class QListWidget;

class SelectAccountsDialog : public QDialog {
    Q_OBJECT
public:
    SelectAccountsDialog(const std::vector<QVariantMap> &accounts, QWidget *parent = nullptr);
    std::vector<QVariantMap> getSelectedAccounts() const;

private:
    QListWidget *m_listWidget;
};

#endif // SELECTACCOUNTDIALOG_H
