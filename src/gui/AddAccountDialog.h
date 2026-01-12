#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QDialog>
#include <QVariantMap>

class QLineEdit;
class QComboBox;
class QSpinBox;

class AddAccountDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddAccountDialog(QWidget *parent = nullptr);
    QVariantMap getData() const;

private:
    QLineEdit *m_inputName;
    QLineEdit *m_inputSecret;
    QComboBox *m_comboAlgorithm;
    QSpinBox *m_spinInterval;
};

#endif // ADDACCOUNTDIALOG_H
