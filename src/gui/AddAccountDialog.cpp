#include "AddAccountDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>

AddAccountDialog::AddAccountDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Ajouter un compte TOTP");
    auto *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Nom:", this));
    m_inputName = new QLineEdit(this);
    m_inputName->setPlaceholderText("Nom du compte");
    layout->addWidget(m_inputName);

    layout->addWidget(new QLabel("Secret:", this));
    m_inputSecret = new QLineEdit(this);
    m_inputSecret->setPlaceholderText("Secret Base32");
    layout->addWidget(m_inputSecret);

    layout->addWidget(new QLabel("Algorithme:", this));
    m_comboAlgorithm = new QComboBox(this);
    m_comboAlgorithm->addItems({"SHA1", "SHA256", "SHA512"});
    layout->addWidget(m_comboAlgorithm);

    layout->addWidget(new QLabel("Intervalle (s):", this));
    m_spinInterval = new QSpinBox(this);
    m_spinInterval->setRange(10, 120);
    m_spinInterval->setValue(30);
    layout->addWidget(m_spinInterval);

    auto *btnOk = new QPushButton("Ajouter", this);
    connect(btnOk, &QPushButton::clicked, this, &AddAccountDialog::accept);
    layout->addWidget(btnOk);
}

QVariantMap AddAccountDialog::getData() const {
    QVariantMap map;
    map["name"] = m_inputName->text();
    map["secret"] = m_inputSecret->text();
    map["algorithm"] = m_comboAlgorithm->currentText();
    map["interval"] = m_spinInterval->value();
    return map;
}
