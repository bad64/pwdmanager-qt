#include "mainwindow.h"

//Not part of the MainWindow namespace

QString getURL(QString language)
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

int IsDir()
{
    struct stat info;

    std::string temppath;
    #if (defined (_WIN32) || defined (_WIN64))
        temppath = getenv("APPDATA");
        temppath.append("\\passwordmanager");
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        temppath = getenv("HOME");
        temppath.append("/.passwordmanager");
    #endif

    if (stat(temppath.c_str(), &info) == 0)
    {
        if (info.st_mode & S_IFDIR)
        {
            //Means everything is fine. No need to do anything here.
        }
        else
        {
            return 1;
        }
    }
    else
    {
        #if (defined (_WIN32) || defined (_WIN64))
            CreateDirectoryA(temppath.c_str(), NULL);
        #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
            mkdir(temppath.c_str(), S_IRWXU);
            chmod(temppath.c_str(), S_IRWXU);
        #endif
    }

    return 0;
}

std::string xorCrypt(std::string msg, std::string key)
{
    if (key.empty())
        key = "MJOLNIR"; //IMPORTANT: Not the actual key used in the release

    std::string tmp(key);
    while(tmp.size() < msg.size())
        tmp += key;

    for (std::string::size_type i = 0; i < msg.size(); i++)
    {
        if  (((msg[i] ^ tmp[i % tmp.size()]) != '\n') && ((msg[i] ^ tmp[i % tmp.size()]) != ',') && ((msg[i] ^ tmp[i % tmp.size()]) != '\x01E'))
            msg[i] ^= tmp[i % tmp.size()];
        else
            msg[i] = msg[i];
    }

    return msg;
}

//More mainwindow namespace

void MainWindow::help()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(getURL(currentLanguage)));
}

void MainWindow::about()
{
    std::stringstream aboutString;
    aboutString << tr("pwdmanager-qt version ").toStdString() << VERSION << tr(" by Lou VINCENT aka Bad64.\n").toStdString();
    aboutString << tr("Made with Qt ").toStdString() << qVersion() << tr(", with GCC version ").toStdString() << __VERSION__ << tr(", based on pwdmanager (also by Bad64).\n\n").toStdString();
    aboutString << tr("This software is provided as-is, free of charge, for personal use.\n").toStdString();
    aboutString << tr("Bad64 shall not be held responsible for passwords stolen by coworkers looking above one's shoulder, ").toStdString();
    aboutString << tr("loss of important credentials, or broken coffee machines.").toStdString();

    QMessageBox::about(this, tr("About"), aboutString.str().c_str());
}
