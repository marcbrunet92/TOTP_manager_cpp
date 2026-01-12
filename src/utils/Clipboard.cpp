#include "Clipboard.h"
#include <QApplication>
#include <QClipboard>

void Clipboard::copyToClipboard(const QString &text) {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);
}
