#include "Storage.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

Storage::Storage(const QString &dbPath) : m_dbPath(dbPath) {
    ensureSchema();
    maybeMigrateFromJson();
}

void Storage::ensureSchema() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_dbPath);

    if (!db.open()) {
        qCritical() << "Error: connection with database fail" << db.lastError();
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS accounts ("
               "id TEXT PRIMARY KEY,"
               "name TEXT NOT NULL,"
               "secret TEXT NOT NULL,"
               "algorithm TEXT NOT NULL,"
               "interval INTEGER NOT NULL,"
               "issuer TEXT,"
               "digits INTEGER NOT NULL,"
               "last_code TEXT"
               ")");
}

std::vector<Account> Storage::loadAccounts() {
    std::vector<Account> accounts;
    QSqlQuery query("SELECT id, name, secret, algorithm, interval, issuer, digits, last_code FROM accounts ORDER BY name COLLATE NOCASE");

    while (query.next()) {
        QVariantMap map;
        map["id"] = query.value(0);
        map["name"] = query.value(1);
        map["secret"] = query.value(2);
        map["algorithm"] = query.value(3);
        map["interval"] = query.value(4);
        map["issuer"] = query.value(5);
        map["digits"] = query.value(6);
        map["last_code"] = query.value(7);
        accounts.push_back(Account::fromVariantMap(map));
    }
    return accounts;
}

void Storage::saveAccounts(const std::vector<Account> &accounts) {
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.exec("DELETE FROM accounts");

    query.prepare("INSERT INTO accounts (id, name, secret, algorithm, interval, issuer, digits, last_code) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

    for (const auto &acc : accounts) {
        query.addBindValue(acc.id);
        query.addBindValue(acc.name);
        query.addBindValue(acc.secret);
        query.addBindValue(acc.algorithm);
        query.addBindValue(acc.interval);
        query.addBindValue(acc.issuer);
        query.addBindValue(acc.digits);
        query.addBindValue(acc.lastCode);
        query.exec();
    }
    QSqlDatabase::database().commit();
}

Account Storage::createAccount(const QVariantMap &data) {
    Account acc = Account::fromVariantMap(data);
    QSqlQuery query;
    query.prepare("INSERT INTO accounts (id, name, secret, algorithm, interval, issuer, digits, last_code) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(acc.id);
    query.addBindValue(acc.name);
    query.addBindValue(acc.secret);
    query.addBindValue(acc.algorithm);
    query.addBindValue(acc.interval);
    query.addBindValue(acc.issuer);
    query.addBindValue(acc.digits);
    query.addBindValue(acc.lastCode);

    if (!query.exec()) {
        qWarning() << "Error creating account:" << query.lastError();
    }
    return acc;
}

bool Storage::isTableEmpty() {
    QSqlQuery query("SELECT COUNT(1) FROM accounts");
    if (query.next()) {
        return query.value(0).toInt() == 0;
    }
    return true;
}

void Storage::maybeMigrateFromJson() {
    QString jsonPath = "accounts.json";
    if (!QFile::exists(jsonPath) || !isTableEmpty()) {
        return;
    }

    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;

    QJsonArray array = doc.array();
    for (const QJsonValue &value : array) {
        if (value.isObject()) {
            createAccount(value.toObject().toVariantMap());
        }
    }
}
