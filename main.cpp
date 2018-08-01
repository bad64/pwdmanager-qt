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

    //Setting up the database array
    window.db = window.ReadFromFile();
    window.Init();

    //Then render !
    window.show();

    return app.exec();
}
