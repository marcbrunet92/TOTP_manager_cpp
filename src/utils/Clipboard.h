#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QString>

class Clipboard {
public:
    static void copyToClipboard(const QString &text);
};

#endif // CLIPBOARD_H
