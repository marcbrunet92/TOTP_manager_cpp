#include "SelectAccountsDialog.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDialogButtonBox>

SelectAccountsDialog::SelectAccountsDialog(const std::vector<QVariantMap> &accounts, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Sélectionner les comptes à importer");
    auto *layout = new QVBoxLayout(this);

    m_listWidget = new QListWidget(this);
    m_listWidget->setSelectionMode(QListWidget::MultiSelection);

    for (const auto &acc : accounts) {
        QString label = acc.value("issuer").toString();
        if (!label.isEmpty() && !acc.value("name").toString().isEmpty()) {
            label += " - ";
        }
        label += acc.value("name").toString();
        if (label.isEmpty()) label = "Compte";

        auto *item = new QListWidgetItem(label, m_listWidget);
        item->setCheckState(Qt::Checked);
        item->setData(Qt::UserRole, acc);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
    layout->addWidget(m_listWidget);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::StandardButton::Ok |
        QDialogButtonBox::StandardButton::Cancel,
        this
    );
    connect(buttons, &QDialogButtonBox::accepted, this, &SelectAccountsDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &SelectAccountsDialog::reject);
    layout->addWidget(buttons);
}

std::vector<QVariantMap> SelectAccountsDialog::getSelectedAccounts() const {
    std::vector<QVariantMap> selected;
    for (int i = 0; i < m_listWidget->count(); ++i) {
        auto *item = m_listWidget->item(i);
        if (item->checkState() == Qt::Checked || item->isSelected()) {
            selected.push_back(item->data(Qt::UserRole).toMap());
        }
    }
    return selected;
}
