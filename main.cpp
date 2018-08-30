#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //Some command line arguments
    QString locale = QLocale::system().name().section('_', 0, 0);

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--lang") == 1)
        {
            locale = QString(argv[i+1]);
            i++;
        }
    }

    //Begin Qt
    QApplication app(argc, argv);

    //Translation
    QTranslator translator;
    #if (defined (_WIN32) || defined (_WIN64))
        translator.load(QString("translations\\pwdmanager-qt_") + locale);
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        translator.load(QString("/usr/local/share/pwdmanager-qt/translations/pwdmanager-qt_") + locale);
    #endif
    app.installTranslator(&translator);

    MainWindow window;

    //Setting up user struct, starting with username

    #if (defined (_WIN32) || defined (_WIN64))
        window.user.username = getenv("USERNAME");
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        window.user.username = getenv("USER");
    #endif

    if (window.user.username.empty())
    {
        std::string errorString = "Heap allocation error: Cannot store username. Exiting.";

        QMessageBox::critical(nullptr, "Error", QString(errorString.c_str()));
        exit(1);
    }

    //Seed RNG with username
    srand(getSeed(window.user.username.c_str()));

    //Setting up database path
    #if (defined (_WIN32) || defined (_WIN64))
        window.user.path = getenv("APPDATA");
        window.user.path.append("\\passwordmanager\\passwords.crdb");
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        window.user.path = getenv("HOME");
        window.user.path.append("/.passwordmanager/passwords.crdb");
    #endif

    if (window.user.path.empty())
    {

        std::string errorString = "Heap allocation error: Cannot store path to database file. Exiting.";

        QMessageBox::critical(nullptr, "Error", QString(errorString.c_str()));
        exit(1);
    }

    //Check if directory exists && we have read/write access to it
    if (IsDir() != 0)
    {
        std::string errorString = window.user.path.c_str();
        errorString.append(" is not a user-writable directory !\nCheck if you have read/write permissions in its parent folder.");

        QMessageBox::critical(nullptr, "Error", QString(errorString.c_str()));
        exit(1);
    }

    //Setting up the database array
    window.db = window.readFromFile();
    window.init();

    //Linux file permissions
    #if (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        chmod(window.user.path.c_str(), S_IRWXU);
    #endif

    //Then render !
    window.show();

    return app.exec();
}
