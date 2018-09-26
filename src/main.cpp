#include "includes.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //Some command line arguments
    QString locale = QLocale::system().name().section('_', 0, 0);
    bool printVersion = false;
    std::string inputfile = "";

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--lang") == 0)
        {
            locale = QString(argv[i+1]);
        }
        else if (strcmp(argv[i], "-v") == 0)
        {
            printVersion = true;
        }
        else if (strcmp(argv[i], "--use") == 0)
        {
            if (std::string(argv[i+1]).substr(strlen(argv[i+1])-5) == ".crdb")
            {
                #if (defined (_WIN32) || defined (_WIN64))
                    TCHAR cwd[255];
                    if (GetCurrentDirectory(255, cwd) != 0)
                    {
                        for (int i = 0; i < 255; i++)
                        {
                            if (cwd[i] != '\0')
                                inputfile += static_cast<char>(cwd[i]);
                            else
                                break;
                        }

                        inputfile += '\\';
                        inputfile += std::string(argv[i+1]);
                    }
                    else
                    {
                        std::cout << "GetCurrentDirectory() error" << std::endl;
                    }
                #elif (defined (LINUX) || defined (__linux__))
                    char cwd[255];
                    if(getcwd(cwd, sizeof(cwd)) != NULL)
                    {
                        inputfile = cwd;
                        inputfile += '/';
                        inputfile += std::string(argv[i+1]);
                    }
                    else
                    {
                        std::cout << "getcwd() error" << std::endl;
                    }
                #endif

                std::cout << inputfile << std::endl;
            }
        }
    }

    //Begin Qt
    QApplication app(argc, argv);

    //Translation
    QTranslator translator;
    #if (defined (_WIN32) || defined (_WIN64))
        translator.load(QString("translations\\pwdmanager-qt_") + locale);
    #elif (defined (LINUX) || defined (__linux__))
        if(!translator.load(QString("/usr/share/pwdmanager-qt/translations/pwdmanager-qt_") + locale))
            translator.load(QString("/usr/local/share/pwdmanager-qt/translations/pwdmanager-qt_") + locale);
    #endif
    app.installTranslator(&translator);

    //If we just want the version
    if (printVersion)
    {
        std::stringstream aboutString;
        aboutString << QObject::tr("pwdmanager-qt version ").toStdString() << VERSION << QObject::tr(" by Lou VINCENT aka Bad64.").toStdString() << std::endl;
        aboutString << QObject::tr("Made with Qt ").toStdString() << qVersion() <<QObject:: tr(", with GCC version ").toStdString() << __VERSION__ << QObject::tr(", based on pwdmanager (also by Bad64)").toStdString() << std::endl;
        aboutString << QObject::tr("If launched with current parameters, the display language would be set as ").toStdString() << locale.toStdString() << "." << std::endl;

        std::cout << aboutString.str() << std::endl;

        exit(0);
    }

    MainWindow window;
    window.setLanguage(locale);

    //Setting up user struct, starting with username

    #if (defined (_WIN32) || defined (_WIN64))
        window.user.username = getenv("USERNAME");
    #elif (defined (LINUX) || defined (__linux__))
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
    if (inputfile == "")
    {
        std::cout << "No file provided, using default" << std::endl;
        #if (defined (_WIN32) || defined (_WIN64))
            window.user.path = getenv("APPDATA");
            window.user.path.append("\\passwordmanager\\passwords.crdb");
        #elif (defined (LINUX) || defined (__linux__))
            window.user.path = getenv("HOME");
            window.user.path.append("/.passwordmanager/passwords.crdb");
        #endif
    }
    else
    {
        std::cout << "Using " << inputfile << std::endl;
        window.user.path = inputfile;
    }

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
    window.db = window.readFromFile(window.user.path);
    window.init();

    //Linux file permissions
    #if (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        chmod(window.user.path.c_str(), S_IRWXU);
    #endif

    //Identity check (to protect against pcjacking)
    bool proceed;
    QString password = QInputDialog::getText(nullptr, QObject::tr("Enter password"), QObject::tr("Please enter your account password:"), QLineEdit::Password, QString(), &proceed, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    if (try_auth(window.user.username, password.toStdString()) == false)
        exit(2);

    //Then render !
    window.show();

    return app.exec();
}
