#ifndef TOTPCARD_H
#define TOTPCARD_H

#include <QWidget>
#include "models/Account.h"

class QLabel;
class QProgressBar;
class QPushButton;
class AccountController;

class TotpCard : public QWidget {
    Q_OBJECT
public:
    TotpCard(const Account &account, AccountController *controller, QWidget *parent = nullptr);
    
    void updateTotp(const QString &code, int timeLeft);
    Account getAccount() const { return m_account; }

private slots:
    void onCopyClicked();

private:
    Account m_account;
    AccountController *m_controller;
    
    QLabel *m_labelAccountName;
    QLabel *m_labelTotp;
    QProgressBar *m_progressTimer;
    QPushButton *m_copyButton;
};

#endif // TOTPCARD_H
