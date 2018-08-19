#include "mainwindow.h"

const char* generate(unsigned int requiredLength, bool requireUppercase, bool requireNumerals, bool requireSpecials)
{
    char lowercase[27] = "abcdefghijklmnopqrstuvwxyz";
    char uppercase[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char numeric[11] = "0123456789";

    char special[9];
    special[0] = '@';
    special[1] = '&';
    special[2] = '+';
    special[3] = '!';
    special[4] = '_';
    special[5] = '%';
    special[6] = '$';
    special[7] = '*';
    special[8] = '\0';

    lowercase[26] = '\0';
    uppercase[26] = '\0';
    numeric[10] = '\0';

    char* password = (char *)calloc(requiredLength+1, sizeof(char));

    //Now we actually generate the password
    do
    {
        for (unsigned int i = 0; i < requiredLength+1; i++)
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
                if (requireNumerals == true)
                    password[i] = numeric[randint(0, strlen(numeric))];
                else
                    password[i] = lowercase[randint(0, strlen(lowercase))];
                break;
            case 3:
                if (requireSpecials == true)
                    password[i] = special[randint(0, strlen(special))];
                else
                    password[i] = lowercase[randint(0, strlen(lowercase))];
                break;
            }
        }
        password[requiredLength] = '\0';

    } while(strlen(password) < requiredLength);

    return password;
}

void MainWindow::newEntry()
{
    backup();

    userWizard = new CreateUser();
    QWidget::connect(userWizard, SIGNAL(returnUserStruct(const char*,const char*,const char*)), this, SLOT(addRow(const char*,const char*,const char*)));
    userWizard->exec();
}

void MainWindow::addRow(const char* n_username, const char* n_purpose, const char* n_password)
{
    int id = lines+1;

    DBRow* temp = db;

    db = (DBRow *)calloc(id, sizeof(DBRow));
    for (unsigned int i = 0; i < lines; i++)
        db[i] = temp[i];

    db[lines].id = id;

    db[lines].username = (char *)calloc(strlen(n_username+1), sizeof(char));
    strcpy(db[lines].username, n_username);

    db[lines].purpose = (char *)calloc(strlen(n_purpose+1), sizeof(char));
    strcpy(db[lines].purpose, n_purpose);

    db[lines].password = (char *)calloc(strlen(n_password+1), sizeof(char));
    strcpy(db[lines].password, n_password);

    lines++;

    appendToFile();
    refreshView();
}

void MainWindow::setNewPassword(unsigned int line, const char* n_password)
{
    db[line].password = (char *)calloc(strlen(n_password+1), sizeof(char));
    strcpy(db[line].password, n_password);

    writeToFile();
    refreshView();
}
