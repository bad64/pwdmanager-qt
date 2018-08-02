#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //Seed RNG
    srand(GetSeed());

    //Begin Qt
    QApplication app(argc, argv);
    MainWindow window;

    //Setting up user struct
    #if (defined (_WIN32) || defined (_WIN64))
        window.user.username = (char *)malloc(sizeof(char) * strlen(getenv("USERNAME")));
        strcpy(window.user.username, getenv("USERNAME"));
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        window.user.username = (char *)malloc(sizeof(char) * strlen(getenv("USER")));
        strcpy(window.user.username, getenv("USER"));
    #endif

    if (window.user.username == NULL)
    {
        std::cout << "Failed heap allocation ! Cannot store user name !" << std::endl;
        exit(1);
    }

    char* buf = new char[100];
    #if (defined (_WIN32) || defined (_WIN64))
        strcpy(buf, getenv("USERPROFILE"));
        strcat(buf, "\\Documents\\passwordmanager\\.passwords");
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        strcpy(buf, getenv("HOME"));
        strcat(buf, "/.passwordmanager/passwords");
    #endif

    window.user.path = (char *)malloc(sizeof(char) * (strlen(buf) + 1));

    if (window.user.path == NULL)
    {
        std::cout << "Failed heap allocation ! Cannot store file path !" << std::endl;
        exit(1);
    }

    strcpy(window.user.path, buf);
    delete[] buf;
    window.user.path[strlen(window.user.path)] = '\0';

    //Check if directory really exists at user.path
    struct stat info;

    char temppath[255];
    #if (defined (_WIN32) || defined (_WIN64))
        strcpy(temppath, getenv("USERPROFILE"));
        strcat(temppath, "\\Documents\\passwordmanager");
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        strcpy(temppath, getenv("HOME"));
        strcat(temppath, "/.passwordmanager");
    #endif

    if (stat(temppath, &info) == 0)
    {
        if (info.st_mode & S_IFDIR)
        {
            //Means everything is fine. No need to do anything here.
        }
        else
        {
            char errorString[255];
            strcpy(errorString, window.user.path);
            strcat(errorString, " is not a user-writable directory !\nCheck if you have read/write permissions in its parent folder.");

            QMessageBox::critical(nullptr, "Error", QString(errorString));
            return 1;
        }
    }
    else
    {
        #if (defined (_WIN32) || defined (_WIN64))
            CreateDirectoryA(temppath, NULL);
        #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
            mkdir(temppath, S_IRWXU);
            chmod(temppath, S_IRWXU);
        #endif
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
