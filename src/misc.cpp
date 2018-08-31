#include "mainwindow.h"

void MainWindow::help()
{
    QMessageBox::about(this, tr("Help"), tr("Add a new row with New Entry.\n"
                                            "Double click on a cell, or highlight it then press the Edit button to edit its contents.\n"
                                            "Highlight a cell and press Delete to erase the entire row.\n"
                                            "Copy copies the highlighted row's password field into your OS' clipboard.\n"
                                            "Move rows around with Ctrl+Up or Ctrl+Down.\n\n"
                                            "The search field looks for a case-insensitive match of the string entered. It is, however, accent-sensitive (e.g. "
                                            "it will not match \"Umlaut\" and \"Ümlaut\").\n"
                                            "Press Ctrl+H to hide/unhide the table (if someone is spying on you !)\n\n"
                                            "In case things go horribly wrong during a transaction (e.g. power loss), you can find a backup of the database "
                                            "in the application data (%AppData%\\Roaming\\passwordmanager on Windows, $HOME/.passwordmanager on Linux) as a .bak "
                                            "file. Import it in the manager, and you should get your passwords back like they were before the transaction.") );
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

int IsDir() //Not part of the MainWindow namespace
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
