#ifndef TOTPGENERATOR_H
#define TOTPGENERATOR_H

#include <QString>

class TotpGenerator {
public:
    static QString generateTotp(const QString &secret, 
                               const QString &algorithm = "SHA1", 
                               int interval = 30, 
                               int digits = 6);
    static int timeRemaining(int interval = 30);

private:
    static QByteArray decodeBase32(const QString &base32);
};

#endif // TOTPGENERATOR_H
