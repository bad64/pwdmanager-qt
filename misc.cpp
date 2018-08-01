#include "mainwindow.h"

void MainWindow::Help()
{

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
