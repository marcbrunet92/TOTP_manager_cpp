#include "MainWindow.h"
#include "controllers/AccountController.h"
#include "gui/widgets/TotpCard.h"
#include "gui/SelectAccountsDialog.h"
#include "utils/OtpauthMigration.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QToolButton>
#include <QTimer>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(AccountController *controller, QWidget *parent)
    : QMainWindow(parent)
    , m_controller(controller)
{
    setWindowTitle("Gestionnaire TOTP");
    resize(600, 800);

    auto *scrollArea = new QScrollArea(this);
    auto *scrollAreaWidget = new QWidget(scrollArea);
    m_scrollLayout = new QVBoxLayout(scrollAreaWidget);
    scrollAreaWidget->setLayout(m_scrollLayout);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaWidget);
    setCentralWidget(scrollArea);

    addToolbar();
    loadAccounts();

    connect(m_controller, &AccountController::accountAdded, this, &MainWindow::onAccountAdded);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateTotps);
    m_timer->start(1000);

    updateTotps();
}

void MainWindow::addToolbar() {
    auto *toolbar = addToolBar("Main Toolbar");

    auto *addAction = new QAction("Ajouter", this);
    connect(addAction, &QAction::triggered, m_controller, &AccountController::addAccountDialog);
    toolbar->addAction(addAction);

    auto *importMenu = new QMenu(this);
    auto *importFromLinkAction = new QAction("À partir d'un lien otpauth-migration…", this);
    connect(importFromLinkAction, &QAction::triggered, this, &MainWindow::importFromOtpauthMigration);
    importMenu->addAction(importFromLinkAction);

    auto *importButton = new QToolButton(this);
    importButton->setText("Importer");
    importButton->setMenu(importMenu);
    importButton->setPopupMode(QToolButton::InstantPopup);
    toolbar->addWidget(importButton);

    auto *exportAction = new QAction("Exporter", this);
    toolbar->addAction(exportAction);
}

void MainWindow::loadAccounts() {
    auto accounts = m_controller->getAllAccounts();
    for (const auto &acc : accounts) {
        addCard(acc);
    }
}

void MainWindow::addCard(const Account &account) {
    auto *card = new TotpCard(account, m_controller, this);
    m_scrollLayout->addWidget(card);
    m_cards[account.id] = card;
}

void MainWindow::onAccountAdded(const Account &account) {
    addCard(account);
    updateTotps();
}

void MainWindow::importFromOtpauthMigration() {
    bool ok;
    QString url = QInputDialog::getText(this, "Importer", 
                                       "À partir d'un lien otpauth-migration:\n\nCollez le lien ici:", 
                                       QLineEdit::Normal, "", &ok);
    if (!ok || url.trimmed().isEmpty()) return;

    try {
        auto result = OtpauthMigration::parse(url.trimmed());
        if (result.accounts.empty()) {
            QMessageBox::information(this, "Importation", "Aucun compte trouvé dans le lien fourni.");
            return;
        }

        SelectAccountsDialog dialog(result.accounts, this);
        if (dialog.exec() != QDialog::Accepted) return;

        auto selected = dialog.getSelectedAccounts();
        if (selected.empty()) {
            QMessageBox::information(this, "Importation", "Aucun compte sélectionné.");
            return;
        }

        int imported = 0;
        for (const auto &data : selected) {
            if (data.value("type").toString() == "totp") {
                m_controller->addAccount(data);
                imported++;
            }
        }

        if (imported > 0) {
            QMessageBox::information(this, "Importation", QString("%1 compte(s) importé(s) avec succès.").arg(imported));
        }
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erreur", QString("Erreur lors de l'importation: %1").arg(e.what()));
    }
}

void MainWindow::updateTotps() {
    for (auto const& [id, card] : m_cards) {
        auto [code, timeLeft] = m_controller->getTotp(card->getAccount());
        card->updateTotp(code, timeLeft);
    }
}
