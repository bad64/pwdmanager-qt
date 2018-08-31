#ifndef CREATEUSER_H
#define CREATEUSER_H

#include "includes.h"
#include <QtWidgets>

class CreateUser : public QDialog
{
    Q_OBJECT

public:
    CreateUser();

public slots:
    void requestGenerate();
    void validate();
    void quit();

signals:
    void returnUserStruct(std::string username, std::string purpose, std::string password);

private:
    //Layouts
    QVBoxLayout* mainLayout;

    QVBoxLayout* usernameLayout;
    QVBoxLayout* purposeLayout;
    QVBoxLayout* passwordLayout;
    QGridLayout* generationOptions;
    QHBoxLayout* dialogButtons;

    //Widgets
    QLabel* usernameLabel;
    QLineEdit* usernameLine;

    QLabel* purposeLabel;
    QLineEdit* purposeLine;

    QLabel* passwordLabel;
    QLineEdit* passwordLine;

    QHBoxLayout* lengthLayout;
    QLabel* lengthLabel;
    QSpinBox* requiredLength;
    QCheckBox* requiresCapitals;
    QCheckBox* requiresNumerals;
    QCheckBox* requiresSpecials;
    QPushButton* generateButton;

    QPushButton* validateButton;
    QPushButton* cancelButton;
};

#endif // CREATEUSER_H
