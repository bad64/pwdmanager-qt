#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "includes.h"
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void Init();

    User user;
    DBRow* db;
    unsigned int lines;

public slots:
    DBRow* ReadFromFile();
    void WriteToFile();

    void Import();
    void Export();
    void Backup();

    void RefreshView();
    void Search();

    void NewEntry();
    void Delete();
    void Edit();
    void Copy();

    const char* Generate();

    void Help();
    void About();

private:
    QGridLayout* mainLayout;
    QTableWidget* table;

    QFrame* searchBoxFrame;
    QVBoxLayout* searchBoxLayout;
    QLabel* searchBoxLabel;
    QLineEdit* searchBox;

    QFrame* buttonsFrame;
    QGridLayout* buttonsLayout;
    QPushButton* newButton;
    QPushButton* deleteButton;
    QPushButton* editButton;
    QPushButton* copyButton;

    QFrame* statusFrame;
    QVBoxLayout* statusLayout;
    QLabel* status;
};

#endif // MAINWINDOW_H
