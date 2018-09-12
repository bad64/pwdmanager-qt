#include "help.h"

HelpWindow::HelpWindow(QString language)
{
    QWidget::setWindowTitle("Password Manager Help");
    this->setMinimumHeight(300);
    this->setMinimumWidth(300);

    textBrowser = new QTextBrowser(this);
    textBrowser->setMinimumHeight(300);
    textBrowser->setMinimumWidth(300);

    textBrowser->setSource(QUrl(getURL(language)));
}

HelpWindow::~HelpWindow()
{

}

QString HelpWindow::getURL(QString language)
{
    //This is essentially a safety measure to avoid a crash in the event that the program runs in an untranslated language

    struct stat buffer;
    QString path = QString("help/" + language + "/index.html");

    if (stat (path.toStdString().c_str(), &buffer) == 0)
        return path;
    else
    {
        std::cout << "Localized help for language " << language.toStdString() << " not found, defaulting to en" << std::endl;
        return QString("help/en/index.html");
    }
}

void HelpWindow::resizeEvent(QResizeEvent *event)
{
    textBrowser->setMinimumHeight(this->height());
    textBrowser->setMinimumWidth(this->width());
    textBrowser->setMaximumHeight(this->height());
    textBrowser->setMaximumWidth(this->width());
    QWidget::resizeEvent(event);
}
