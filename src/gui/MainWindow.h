#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <string>
#include "models/Account.h"

class AccountController;
class TotpCard;
class QVBoxLayout;
class QTimer;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(AccountController *controller, QWidget *parent = nullptr);

private slots:
    void importFromOtpauthMigration();
    void updateTotps();
    void onAccountAdded(const Account &account);

private:
    void addToolbar();
    void loadAccounts();
    void addCard(const Account &account);

    AccountController *m_controller;
    QVBoxLayout *m_scrollLayout;
    std::map<QString, TotpCard*> m_cards;
    QTimer *m_timer;
};

#endif // MAINWINDOW_H
