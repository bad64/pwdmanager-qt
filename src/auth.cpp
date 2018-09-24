#include "includes.h"

bool try_auth(std::string username, std::string password)
{
    #if (defined (_WIN32) || defined (_WIN64))
        HANDLE hToken;
        if (LogonUserA(username.c_str(), ".", password.c_str(), LOGON32_LOGON_NETWORK, LOGON32_PROVIDER_DEFAULT, &hToken) != 0)
        {
            for (unsigned int i = 0; i < password.length(); i++) //Securely erase the password string
                password[i] = '\0';
            return true;
        }
        else
        {
            for (unsigned int i = 0; i < password.length(); i++)
                password[i] = '\0';
            return false;
        }
    #elif (defined (LINUX) || defined (__linux__))
        //Most of the code in this section was provided by Zombie on the New Blood Discord.
        //Credit where it's due and all that, cause I would have never figured that out on my own

        int result = 0;
        pam_handle_t *handle = nullptr;

        pam_response *reply = static_cast<decltype(reply)>(malloc(sizeof(struct pam_response)));
        reply->resp = strdup(password.c_str());
        reply->resp_retcode = 0;

        pam_conv loc_conv;
        loc_conv.appdata_ptr = reply;
        loc_conv.conv = [](int num_msg, pam_message const **msg, pam_response **resp, void *appdata_ptr)
                            {
                                (void)num_msg;
                                (void)msg;
                                *resp = static_cast<pam_response *>(appdata_ptr);
                                return PAM_SUCCESS;
                            };

        result = pam_start("sudo", username.c_str(), &loc_conv, &handle);
        if(result != PAM_SUCCESS)
        {
            for (unsigned int i = 0; i < password.length(); i++)
                password[i] = '\0';
            return false;
        }

        result = pam_authenticate(handle, 0);
        if(result != PAM_SUCCESS)
        {
            for (unsigned int i = 0; i < password.length(); i++)
                password[i] = '\0';
            return false;
        }

        result = pam_end(handle, result);
        if(result != PAM_SUCCESS)
        {
            for (unsigned int i = 0; i < password.length(); i++)
                password[i] = '\0';
            return false;
        }

        for (unsigned int i = 0; i < password.length(); i++)
            password[i] = '\0';
        return true;
    #endif
}
