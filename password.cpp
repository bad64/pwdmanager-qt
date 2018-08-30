#include "mainwindow.h"

std::string generate(unsigned int requiredLength, bool requireUppercase, bool requireNumerals, bool requireSpecials)
{
    std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
    std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string numeric = "0123456789";
    std::string special = "@&+!_%$*";

    std::string password = "";

    //Now we actually generate the password
    for (unsigned int i = 0; i < requiredLength; i++)
    {
        unsigned char listindex = randint(0, 3);

        switch (listindex)
        {
        case 0:
            password.push_back(lowercase[randint(0, lowercase.length()-1)]);
            break;
        case 1:
            if (requireUppercase == true)
                password.push_back(uppercase[randint(0, uppercase.length()-1)]);
            else
                password.push_back(lowercase[randint(0, lowercase.length()-1)]);
            break;
        case 2:
            if (requireNumerals == true)
                password.push_back(numeric[randint(0, numeric.length()-1)]);
            else
                password.push_back(lowercase[randint(0, lowercase.length()-1)]);
            break;
        case 3:
            if (requireSpecials == true)
                password.push_back(special[randint(0, special.length()-1)]);
            else
                password.push_back(lowercase[randint(0, lowercase.length()-1)]);
            break;
        }
    }

    return password;
}

void MainWindow::newEntry()
{
    if (!hide)  //Disallow creation of credentials while table is hidden
    {
        backup();

        userWizard = new CreateUser();
        QWidget::connect(userWizard, SIGNAL(returnUserStruct(std::string, std::string, std::string)), this, SLOT(addRow(std::string, std::string, std::string)));
        userWizard->exec();
    }
}

void MainWindow::addRow(std::string n_username, std::string n_purpose, std::string n_password)
{
    DBRow* temp = db;

    db = (DBRow *)calloc(lines+1, sizeof(DBRow));
    for (unsigned int i = 0; i < lines; i++)
        db[i] = temp[i];

    db[lines].username = n_username;
    db[lines].purpose = n_purpose;
    db[lines].password = n_password;

    lines++;

    writeToFile();
    refreshView();
}

void MainWindow::setNewPassword(unsigned int line, std::string n_password)
{
    db[line].password = n_password;

    writeToFile();
    refreshView();
}
