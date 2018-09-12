#ifndef HELP_H
#define HELP_H

#include "includes.h"
#include <QtWidgets>
#include <QStringList>

class HelpWindow : public QWidget
{
    Q_OBJECT

public:
    HelpWindow(QString language);
    ~HelpWindow();
    void resizeEvent(QResizeEvent *event);
    QString getURL(QString language);

private:
    QTextBrowser *textBrowser;
};

#endif // HELP_H
