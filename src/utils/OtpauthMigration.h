#ifndef OTPAUTHMIGRATION_H
#define OTPAUTHMIGRATION_H

#include <QString>
#include <vector>
#include <QVariantMap>

class OtpauthMigration {
public:
    struct MigrationResult {
        int version;
        std::vector<QVariantMap> accounts;
    };

    static MigrationResult parse(const QString &url);

private:
    static QString decodeSecret(const std::string &secret);
};

#endif // OTPAUTHMIGRATION_H
