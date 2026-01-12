#ifndef ACCOUNTCONTROLLER_H
#define ACCOUNTCONTROLLER_H

#include <QObject>
#include <vector>
#include <memory>
#include "models/Account.h"
#include "models/Storage.h"

class AccountController : public QObject {
    Q_OBJECT
public:
    explicit AccountController(QObject *parent = nullptr);
    
    std::vector<Account> getAllAccounts() const;
    void addAccountDialog();
    Account addAccount(const QVariantMap &data);
    
    std::pair<QString, int> getTotp(const Account &account);

signals:
    void accountAdded(const Account &account);

private:
    std::unique_ptr<Storage> m_storage;
    std::vector<Account> m_accounts;
};

#endif // ACCOUNTCONTROLLER_H
