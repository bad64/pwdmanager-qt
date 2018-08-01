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

    void RefreshView();
    void NewEntry();
    void Delete();
    void Edit();
    void Copy();
    const char *Generate();
    void Help();
    void About();

private:
    QTableWidget* table;
    QTableWidget* searchresults;
    QLabel* sboxlabel;
    QLineEdit* searchbox;
    QPushButton* newButton;
    QPushButton* deleteButton;
    QPushButton* editButton;
    QPushButton* copyButton;
};

#endif // MAINWINDOW_H
