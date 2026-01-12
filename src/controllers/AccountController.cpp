#include "AccountController.h"
#include "models/TotpGenerator.h"
#include "gui/AddAccountDialog.h"
#include <QDialog>

AccountController::AccountController(QObject *parent) 
    : QObject(parent)
    , m_storage(std::make_unique<Storage>())
{
    m_accounts = m_storage->loadAccounts();
}

std::vector<Account> AccountController::getAllAccounts() const {
    return m_accounts;
}

void AccountController::addAccountDialog() {
    AddAccountDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap data = dialog.getData();
        addAccount(data);
    }
}

Account AccountController::addAccount(const QVariantMap &data) {
    Account account = m_storage->createAccount(data);
    m_accounts.push_back(account);
    emit accountAdded(account);
    return account;
}

std::pair<QString, int> AccountController::getTotp(const Account &account) {
    QString code = TotpGenerator::generateTotp(account.secret, account.algorithm, account.interval, account.digits);
    int timeLeft = TotpGenerator::timeRemaining(account.interval);
    return {code, timeLeft};
}
