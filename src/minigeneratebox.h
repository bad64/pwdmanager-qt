#ifndef MINIGENERATEBOX_H
#define MINIGENERATEBOX_H

#include "includes.h"

class MiniGenerateBox : public QDialog
{
    Q_OBJECT

public:
    MiniGenerateBox(int nline, std::string oldpw);

public slots:
    void requestGenerate();
    void validate();
    void quit();

signals:
    void returnPassword(unsigned int line, std::string password);

private:
    int line;
    std::string oldpassword;

    //Layouts
    QVBoxLayout* mainLayout;

    QVBoxLayout* passwordLayout;
    QGridLayout* generationOptions;
    QHBoxLayout* dialogButtons;

    //Widgets

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

#endif // MINIGENERATEBOX_H
