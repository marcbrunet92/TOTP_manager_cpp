#include "TotpCard.h"
#include "controllers/AccountController.h"
#include "utils/Clipboard.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

TotpCard::TotpCard(const Account &account, AccountController *controller, QWidget *parent)
    : QWidget(parent)
    , m_account(account)
    , m_controller(controller)
{
    auto *layout = new QHBoxLayout(this);

    m_labelAccountName = new QLabel(account.name, this);
    layout->addWidget(m_labelAccountName);

    m_labelTotp = new QLabel("------", this);
    m_labelTotp->setStyleSheet("font-weight: bold; font-size: 16px;");
    layout->addWidget(m_labelTotp);

    m_progressTimer = new QProgressBar(this);
    m_progressTimer->setRange(0, account.interval);
    m_progressTimer->setValue(account.interval);
    m_progressTimer->setTextVisible(false);
    layout->addWidget(m_progressTimer);

    m_copyButton = new QPushButton("Copier", this);
    connect(m_copyButton, &QPushButton::clicked, this, &TotpCard::onCopyClicked);
    layout->addWidget(m_copyButton);
}

void TotpCard::updateTotp(const QString &code, int timeLeft) {
    m_labelTotp->setText(code);
    m_progressTimer->setValue(timeLeft);
}

void TotpCard::onCopyClicked() {
    Clipboard::copyToClipboard(m_labelTotp->text());
}
