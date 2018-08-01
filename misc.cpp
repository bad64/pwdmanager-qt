#include "mainwindow.h"

void MainWindow::Help()
{
    QMessageBox::information(this, "Help", "Add a new row with New Entry.\n"
                                            "Double click on a cell, or highlight it then press the Edit button to edit its contents.\n"
                                            "Highlight a cell and press Delete to erase the entire row.\n"
                                            "Copy copies the highlighted row's password field into your OS' clipboard.\n\n"
                                            "The search field looks for a case-sensitive exact match of the string entered (for example, "
                                            "it will not match \"test\" with either \"Test\" or \"testing\".");
}

void MainWindow::About()
{
    QMessageBox::information(this, "About", "pwdmanager-qt version 1.0 by Lou VINCENT aka Bad64\n"
                                            "Made with Qt 5.10, based on pwdmanager (also by Bad64)\n\n"
                                            "This software is provided as-is, free of charge, for personal use.\n"
                                            "It is not meant to be used in a professional environment, although it can be.\n"
                                            "Bad64 shall not be held responsible for any stolen passwords by nosy coworkers, "
                                            "loss of important login credentials, or broken coffee machines.");
}
