#include "TotpGenerator.h"
#include <QDateTime>
#include <QMessageAuthenticationCode>
#include <QCryptographicHash>
#include <QtEndian>
#include <cmath>

QString TotpGenerator::generateTotp(const QString &secret, const QString &algorithm, int interval, int digits) {
    QByteArray key = decodeBase32(secret);
    if (key.isEmpty()) return "000000";

    qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    qint64 counter = timestamp / interval;

    QByteArray msg;
    msg.resize(8);
    qToBigEndian<quint64>(counter, reinterpret_cast<uchar*>(msg.data()));

    QCryptographicHash::Algorithm hashAlgo = QCryptographicHash::Sha1;
    if (algorithm.toUpper() == "SHA256") hashAlgo = QCryptographicHash::Sha256;
    else if (algorithm.toUpper() == "SHA512") hashAlgo = QCryptographicHash::Sha512;
    else if (algorithm.toUpper() == "MD5") hashAlgo = QCryptographicHash::Md5;

    QByteArray hmac = QMessageAuthenticationCode::hash(msg, key, hashAlgo);

    int offset = hmac[hmac.length() - 1] & 0x0F;
    quint32 bin_code = (
        (static_cast<uchar>(hmac[offset]) & 0x7F) << 24 |
        (static_cast<uchar>(hmac[offset + 1]) & 0xFF) << 16 |
        (static_cast<uchar>(hmac[offset + 2]) & 0xFF) << 8 |
        (static_cast<uchar>(hmac[offset + 3]) & 0xFF)
    );

    quint32 otp = bin_code % static_cast<quint32>(std::pow(10, digits));
    return QString("%1").arg(otp, digits, 10, QLatin1Char('0'));
}

int TotpGenerator::timeRemaining(int interval) {
    return interval - (QDateTime::currentSecsSinceEpoch() % interval);
}

QByteArray TotpGenerator::decodeBase32(const QString &base32) {
    static const QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    QByteArray out;
    int buffer = 0;
    int bitsLeft = 0;

    QString cleaned = base32.toUpper().trimmed();
    for (int i = 0; i < cleaned.length(); ++i) {
        int val = alphabet.indexOf(cleaned[i]);
        if (val == -1) {
            if (cleaned[i] == '=') break;
            continue;
        }
        buffer = (buffer << 5) | val;
        bitsLeft += 5;
        if (bitsLeft >= 8) {
            out.append(static_cast<char>((buffer >> (bitsLeft - 8)) & 0xFF));
            bitsLeft -= 8;
        }
    }
    return out;
}
