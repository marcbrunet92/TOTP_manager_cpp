#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <QString>
#include "Account.h"

class Storage {
public:
    Storage(const QString &dbPath = "app.db");
    std::vector<Account> loadAccounts();
    void saveAccounts(const std::vector<Account> &accounts);
    Account createAccount(const QVariantMap &data);

private:
    void ensureSchema();
    void maybeMigrateFromJson();
    bool isTableEmpty();
    QString m_dbPath;
};

#endif // STORAGE_H
