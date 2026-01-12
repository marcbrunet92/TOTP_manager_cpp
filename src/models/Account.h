#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <QUuid>
#include <QVariantMap>

class Account {
public:
    Account();
    static Account fromVariantMap(const QVariantMap &map);
    QVariantMap toVariantMap() const;

    QString id;
    QString name;
    QString secret;
    QString algorithm;
    int interval;
    QString issuer;
    int digits;
    QString lastCode;

    static constexpr const char* DEFAULT_ALGORITHM = "SHA1";
    static constexpr int DEFAULT_INTERVAL = 30;
    static constexpr int DEFAULT_DIGITS = 6;
};

#endif // ACCOUNT_H
