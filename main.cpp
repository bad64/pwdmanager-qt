#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //Begin Qt
    QApplication app(argc, argv);
    MainWindow window;

    //Setting up user struct, starting with username
    size_t usernamesize;
    #if (defined (_WIN32) || defined (_WIN64))
        usernamesize = (sizeof(char) * (strlen(getenv("USERNAME")) + 1) );
        window.user.username = (char *)calloc(usernamesize, sizeof(char));
        strcpy(window.user.username, getenv("USERNAME"));
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        usernamesize = (sizeof(char) * (strlen(getenv("USER")) + 1) );
        window.user.username = (char *)calloc(usernamesize, sizeof(char));
        strcpy(window.user.username, getenv("USER"));
    #endif
    window.user.username[usernamesize] = '\0';

    if (window.user.username == NULL)
    {
        char errorString[255];
        strcpy(errorString, window.user.path);
        strcat(errorString, "Heap allocation error: Cannot store username. Exiting.");

        QMessageBox::critical(nullptr, "Error", QString(errorString));
        exit(1);
    }

    //Seed RNG with username
    srand(GetSeed(window.user.username));

    //Check if directory exists && we have read/write access to it
    if (IsDir() != 0)
    {
        char errorString[255];
        strcpy(errorString, window.user.path);
        strcat(errorString, " is not a user-writable directory !\nCheck if you have read/write permissions in its parent folder.");

        QMessageBox::critical(nullptr, "Error", QString(errorString));
        exit(1);
    }

    //Setting up database path
    size_t pathsize;
    #if (defined (_WIN32) || defined (_WIN64))
        pathsize = (sizeof(char) * (strlen(getenv("APPDATA")) + strlen("\\passwordmanager\\passwords.db") + 1) );
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        pathsize = (sizeof(char) * (strlen(getenv("HOME")) + strlen("/.passwordmanager/passwords.db") + 1) );
    #endif

    window.user.path = (char *)calloc(pathsize, sizeof(char));

    #if (defined (_WIN32) || defined (_WIN64))
        strcpy(window.user.path, getenv("APPDATA"));
        strcat(window.user.path, "\\passwordmanager\\passwords.db");
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        strcpy(window.user.path, getenv("HOME"));
        strcat(window.user.path, "/.passwordmanager/passwords.db");
    #endif

    window.user.path[pathsize] = '\0';

    if (window.user.path == NULL)
    {
        char errorString[255];
        strcpy(errorString, window.user.path);
        strcat(errorString, "Heap allocation error: Cannot store path to database file. Exiting.");

        QMessageBox::critical(nullptr, "Error", QString(errorString));
        exit(1);
    }

    //Setting up the database array
    window.db = window.ReadFromFile();
    window.Init();

    //Linux file permissions
    #if (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        chmod(window.user.path, S_IRWXU);
    #endif

    //Then render !
    window.show();

    return app.exec();
}
