#include "Account.h"

Account::Account()
    : id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , algorithm(DEFAULT_ALGORITHM)
    , interval(DEFAULT_INTERVAL)
    , digits(DEFAULT_DIGITS)
{
}

Account Account::fromVariantMap(const QVariantMap &map) {
    Account acc;
    acc.id = map.value("id", QUuid::createUuid().toString(QUuid::WithoutBraces)).toString();
    acc.name = map.value("name").toString();
    acc.secret = map.value("secret").toString();
    acc.algorithm = map.value("algorithm", DEFAULT_ALGORITHM).toString();
    acc.interval = map.value("interval", DEFAULT_INTERVAL).toInt();
    acc.issuer = map.value("issuer").toString();
    acc.digits = map.value("digits", DEFAULT_DIGITS).toInt();
    acc.lastCode = map.value("last_code").toString();
    return acc;
}

QVariantMap Account::toVariantMap() const {
    QVariantMap map;
    map["id"] = id;
    map["name"] = name;
    map["secret"] = secret;
    map["algorithm"] = algorithm;
    map["interval"] = interval;
    map["issuer"] = issuer;
    map["digits"] = digits;
    map["last_code"] = lastCode;
    return map;
}
