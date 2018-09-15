#include "mainwindow.h"

#if (defined (LINUX) || defined (__linux__))
    //Most of the code in this section was provided by Zombie on the New Blood Discord. Credit where it's due and all that, cause I would have never figured that out on my own
    int try_auth(std::string username, std::string password)
    {
        int result = 0;
        pam_handle_t *handle = NULL;

        pam_response *reply = (decltype(reply))malloc(sizeof(struct pam_response));
        reply->resp = strdup(password.c_str());
        reply->resp_retcode = 0;

        pam_conv loc_conv;
        loc_conv.appdata_ptr = reply;
        loc_conv.conv = [](int num_msg, pam_message const **msg, pam_response **resp, void *appdata_ptr)
                            {
                                *resp = static_cast<pam_response *>(appdata_ptr);
                                return PAM_SUCCESS;
                            };

        #define HANDLE_PAM_ERROR(handle, result) \
            if(result != PAM_SUCCESS) { \
                printf("%s\n", pam_strerror(handle, result)); \
                return EXIT_FAILURE; \
            }

            result = pam_start("sudo", username.c_str(), &loc_conv, &handle);
            HANDLE_PAM_ERROR(handle, result);

            result = pam_authenticate(handle, 0);
            HANDLE_PAM_ERROR(handle, result);

            result = pam_end(handle, result);
            HANDLE_PAM_ERROR(handle, result);
        #undef HANDLE_PAM_ERROR

        return 0;
    }
#endif

int MainWindow::askUserPassword()
{
    bool proceed;
    QString password = QInputDialog::getText(this, tr("Enter password"), tr("Please enter your account password:"), QLineEdit::Password, QString(), &proceed, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    #if (defined (_WIN32) || defined (_WIN64))
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
        if (proceed)
        {
            if (try_auth(user.username, password.toStdString()) == 0)
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
    #endif
}
