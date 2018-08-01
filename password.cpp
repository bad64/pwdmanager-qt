#include "mainwindow.h"

const char* MainWindow::Generate()
{
    char lowercase[27] = "abcdefghijklmnopqrstuvwxyz";
    char uppercase[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char numeric[11] = "0123456789";
    char special[] = "@&§!€ù$*";

    lowercase[26] = '\0';
    uppercase[26] = '\0';
    numeric[10] = '\0';

    unsigned int length = 10;
    bool requireUppercase = false, requireNumeric = false, requireSpecial = false;
    bool proceed = true;

    //Qt stuff
    int intlimit = std::numeric_limits<int>::max();
    length = QInputDialog::getInt(this, "Password generation", "Please enter the desired length of the password:", 10, 0, intlimit, 1, &proceed);
    if (proceed == false)
    {
        return "";
    }

    int qUppercase = QMessageBox::question(this, "Password Generation", "Does your password requires uppercase letters ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    int qNumeric = QMessageBox::question(this, "Password Generation", "Does your password requires numeric characters ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    int qSpecial = QMessageBox::question(this, "Password Generation", "Does your password requires special characters (i.e. non alphanumeric) ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (qUppercase == QMessageBox::Yes)
        requireUppercase = true;

    if (qNumeric == QMessageBox::Yes)
        requireNumeric = true;

    if (qSpecial == QMessageBox::Yes)
        requireSpecial = true;

    char* password = (char *)malloc(sizeof(char) * (length+1));

    //Now we actually generate the password
    do
    {
        for (unsigned int i = 0; i < length+1; i++)
        {
            unsigned char listindex = randint(0, 3);

            switch (listindex)
            {
            case 0:
                password[i] = lowercase[randint(0, strlen(lowercase))];
                break;
            case 1:
                if (requireUppercase == true)
                    password[i] = uppercase[randint(0, strlen(uppercase))];
                else
                    password[i] = lowercase[randint(0, strlen(lowercase))];
                break;
            case 2:
                if (requireNumeric == true)
                    password[i] = numeric[randint(0, strlen(numeric))];
                else
                    password[i] = lowercase[randint(0, strlen(lowercase))];
                break;
            case 3:
                if (requireSpecial == true)
                    password[i] = special[randint(0, strlen(special))];
                else
                    password[i] = lowercase[randint(0, strlen(lowercase))];
                break;
            }
        }
        password[length] = '\0';

    } while(strlen(password) < length);

    return password;
}

void MainWindow::NewEntry()
{
    DBRow buffer;
    int needsGeneration = 0;
    bool proceed = true;

    buffer.username = (char *)QInputDialog::getText(this, "Enter username", "Please enter the user name for this entry:", QLineEdit::Normal, QString(user.username), &proceed).toStdString().c_str();
    if (proceed == false)
        return;

    do
    {
        buffer.purpose =(char *)QInputDialog::getText(this, "Enter reason", "Please enter what these credentials will be used for:", QLineEdit::Normal, QString(""), &proceed).toStdString().c_str();
        if (proceed == false)
            return;

    }while (strcmp(buffer.purpose, "") == 0);

    needsGeneration = QMessageBox::question(this, "Password Generation", "Shall the software generate a password ?", QMessageBox::Yes | QMessageBox::No);

    if (needsGeneration == QMessageBox::No)
    {
        buffer.password = (char *)QInputDialog::getText(this, "Enter password", "Please enter the password that will be stored:", QLineEdit::Normal, QString(""), &proceed).toStdString().c_str();
        if (proceed == false)
            return;
    }
    else
    {
        buffer.password = (char *)Generate();
        if (strcmp(buffer.password, "") == 0)
            return;
    }

    //lines += 1;
    buffer.id = lines+1;

    DBRow* temp = db;

    db = (DBRow *)malloc(sizeof(DBRow) * lines+1);
    for (unsigned int i = 0; i < lines; i++)
        db[i] = temp[i];

    db[lines] = buffer;
    lines++;

    WriteToFile();
    db = ReadFromFile();
    RefreshView();
}
