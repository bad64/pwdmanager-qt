#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "includes.h"
#include "createuser.h"
#include "minigeneratebox.h"
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void init();

    User user;
    DBRow* db;
    unsigned int lines;

public slots:
    DBRow* readFromFile();
    void writeToFile();
    void appendToFile();

    void importFromFile();
    void exportToFile();
    void backup();

    void refreshView();
    void search();
    void clearSearch();

    void newEntry();
    void addRow(const char*,const char*,const char*);
    void deleteRow();
    void edit();
    void copy();

    void setNewPassword(unsigned int line, const char*);

    void help();
    void about();

private:
    //Main window
    QGridLayout* mainLayout;
    QTableWidget* table;

    QFrame* searchBoxFrame;
    QVBoxLayout* searchBoxLayout;
    QHBoxLayout* searchBoxSubLayout;
    QLabel* searchBoxLabel;
    QLineEdit* searchBox;
    QCheckBox* exactMatch;
    QPushButton* clearButton;

    QFrame* buttonsFrame;
    QGridLayout* buttonsLayout;
    QPushButton* newButton;
    QPushButton* deleteButton;
    QPushButton* editButton;
    QPushButton* copyButton;

    QFrame* statusFrame;
    QVBoxLayout* statusLayout;
    QLabel* status;

    //User creation wizard
    CreateUser* userWizard;

    //Password re-generator
    MiniGenerateBox* newPassword;
};

#endif // MAINWINDOW_H
