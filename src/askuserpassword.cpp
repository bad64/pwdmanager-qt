#include "mainwindow.h"

int MainWindow::askUserPassword()
{

    #if (defined (_WIN32) || defined (_WIN64))
        bool proceed;
        QString password = QInputDialog::getText(this, tr("Enter password"), tr("Please enter your Windows account password:"), QLineEdit::Password, QString(), &proceed, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

        if (proceed)
        {
            HANDLE hToken;
            if (LogonUserA(user.username.c_str(), ".", password.toStdString().c_str(), LOGON32_LOGON_NETWORK, LOGON32_PROVIDER_DEFAULT, &hToken) != 0)
            {
                password = QString();
                return 1;
            }
            else
            {
                password = QString();
                return 0;
            }
        }
        else
            return 0;
    #elif (defined (LINUX) || defined (__linux__))
        return 1;

        //Why PAM isn't providing simple authentication facilities (say, taking as parameter a username and a password) is beyond me.
        //Instead, the developer is made to jump through ridiculous hoops, most of which **DO NOT WORK IN GRAPHICAL APPLICATIONS**.

        //Really, just look above. Microsoft did it right. MICROSOFT. There is a function that takes a username, a domain name, a password,
        //a logon service (which is well documented), a logon provider (equally well documented), and a return handle to store the login token
        //(which I do not need for this purpose). The function returns a boolean indicating whether the login attempt is successful or not.

        //Meanwhile, in the wonderful world of overengineering, I have to try to decipher the linux.die.net "documentation" (and using that
        //term is frankly an insult to the *concept* of documentation) to figure out how can I get pam_authenticate() to accept a QInputDialog
        //provided password, because my program doesn't have a console window. And that seems to be impossible short of stdout hacks.

        //Have y'all forgotten the KISS principle at some point ?
    #endif
}
