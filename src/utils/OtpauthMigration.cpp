#include "OtpauthMigration.h"
#include "otpauth-migration.pb.h"
#include <QUrl>
#include <QUrlQuery>
#include <QByteArray>
#include <stdexcept>

OtpauthMigration::MigrationResult OtpauthMigration::parse(const QString &urlStr) {
    QUrl url(urlStr);
    if (url.scheme() != "otpauth-migration" || url.host() != "offline") {
        throw std::runtime_error("Invalid otpauth-migration URL");
    }

    QUrlQuery query(url);
    if (!query.hasQueryItem("data")) {
        throw std::runtime_error("Missing migration payload");
    }

    QString payloadB64 = query.queryItemValue("data", QUrl::FullyDecoded);
    QByteArray payloadBin = QByteArray::fromBase64(payloadB64.toUtf8());

    otpauth_migration::Payload payload;
    if (!payload.ParseFromArray(payloadBin.data(), payloadBin.size())) {
        throw std::runtime_error("Failed to parse protobuf payload");
    }

    MigrationResult result;
    result.version = payload.version();

    for (int i = 0; i < payload.otp_parameters_size(); ++i) {
        const auto &otp = payload.otp_parameters(i);
        QVariantMap acc;
        acc["name"] = QString::fromStdString(otp.name());
        acc["issuer"] = QString::fromStdString(otp.issuer());
        acc["secret"] = decodeSecret(otp.secret());
        
        QString algo = "SHA1";
        switch (otp.algorithm()) {
            case otpauth_migration::Payload::ALGORITHM_SHA256: algo = "SHA256"; break;
            case otpauth_migration::Payload::ALGORITHM_SHA512: algo = "SHA512"; break;
            case otpauth_migration::Payload::ALGORITHM_MD5: algo = "MD5"; break;
            default: algo = "SHA1"; break;
        }
        acc["algorithm"] = algo;

        int digits = 6;
        if (otp.digits() == otpauth_migration::Payload::DIGIT_COUNT_EIGHT) {
            digits = 8;
        }
        acc["digits"] = digits;

        QString type = "totp";
        if (otp.type() == otpauth_migration::Payload::OTP_TYPE_HOTP) {
            type = "hotp";
        }
        acc["type"] = type;

        result.accounts.push_back(acc);
    }

    return result;
}

QString OtpauthMigration::decodeSecret(const std::string &secret) {
    static const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    QString out;
    int buffer = 0;
    int bitsLeft = 0;

    for (unsigned char c : secret) {
        buffer = (buffer << 8) | c;
        bitsLeft += 8;
        while (bitsLeft >= 5) {
            out.append(alphabet[(buffer >> (bitsLeft - 5)) & 0x1F]);
            bitsLeft -= 5;
        }
    }
    // Note: Python code rstrips '='. Our manual base32 encoding doesn't add padding.
    return out;
}
