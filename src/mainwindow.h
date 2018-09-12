#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "includes.h"
#include "createuser.h"
#include "minigeneratebox.h"
#include "credentialsfile.h"
#include "help.h"
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void init();
    void setLanguage(QString language);

    User user;
    std::vector<DBRow> db;
    unsigned int lines;

public slots:
    //database.cpp
    std::vector<DBRow> readFromFile();
    void writeToFile();
    void deleteRow();
    void edit();
    void copy();
    void backup();
    void restore();
    void moveRowUp();
    void moveRowDown();
    void importFromFile();
    void exportToFile();
    void convertOldFile(QString filepath);

    //mainwindow.cpp
    void hideTable();
    void refreshView();
    void search();
    void clearSearch();

    //password.cpp (should probably be moved to database.cpp)
    void newEntry();
    void addRow(std::string nusername, std::string npurpose, std::string npassword);
    void setNewPassword(unsigned int line, std::string);

    //askuserpassword.cpp
    int askUserPassword();

    //misc.cpp
    void help();
    void about();

private:
    QString currentLanguage;

    //Main window
    QGridLayout *mainLayout;
    QTableWidget *table;

    QFrame *searchBoxFrame;
    QVBoxLayout *searchBoxLayout;
    QHBoxLayout *searchBoxSubLayout;
    QLabel *searchBoxLabel;
    QLineEdit *searchBox;
    QCheckBox *exactMatch;
    QPushButton *clearButton;

    QFrame *buttonsFrame;
    QGridLayout *buttonsLayout;
    QPushButton *newButton;
    QPushButton *deleteButton;
    QPushButton *editButton;
    QPushButton *copyButton;

    QFrame *statusFrame;
    QVBoxLayout *statusLayout;
    QLabel *status;

    //User creation wizard
    CreateUser *userWizard;

    //Help window
    QWidget *helpWizard;

    //Password re-generator
    MiniGenerateBox *newPassword;

    //Optical camouflage technology
    bool hide;
};

#endif // MAINWINDOW_H
