#include "mainwindow.h"

using namespace std;

DBRow *MainWindow::ReadFromFile()
{
    free(db);

    fstream file(user.path);

    if (!file)
    {
        file.open(user.path, ios::out);
        //cout << "Created database at " << user.path << endl;
        file.close();
        file.open(user.path, ios::in);
    }
    else
    {
        //cout << "Database located at " << user.path << endl;
    }

    string line;
    lines = 0;
    while(getline(file, line))
        lines++;

    DBRow* db = (DBRow *)calloc(lines, sizeof(DBRow));
    if (db == NULL)
    {
        cout << "Heap allocation failed !" << endl;
        exit(1);
    }
    else
    {
        //cout << "Retrieved " << lines << " entries" << endl;
    }

    file.close();
    file.open(user.path, ios::in);

    for (unsigned int i = 0; i < lines; i++)
    {
        DBRow buffer;

        getline(file, line);

        char delimiter = ',';
        int pos = 0;

        string token = line.substr(0, line.find(delimiter));
        buffer.id = stoi(token);
        pos = line.find(delimiter, pos) + 1;

        token = line.substr(pos, line.find(delimiter, pos)-pos);
        buffer.username = (char *)calloc(strlen(token.c_str())+1, sizeof(char));
        strcpy(buffer.username, token.c_str());
        buffer.username[strlen(buffer.username)] = '\0';
        pos = line.find(delimiter, pos) + 1;

        token = line.substr(pos, line.find(delimiter, pos)-pos);
        buffer.purpose = (char *)calloc(strlen(token.c_str())+1, sizeof(char));
        strcpy(buffer.purpose, token.c_str());
        buffer.purpose[strlen(buffer.purpose)] = '\0';
        pos = line.find(delimiter, pos) + 1;

        token = line.substr(pos, line.find(delimiter, pos)-pos);
        buffer.password = (char *)calloc(strlen(token.c_str())+1, sizeof(char));
        strcpy(buffer.password, token.c_str());
        buffer.password[strlen(buffer.password)] = '\0';
        pos = line.find(delimiter, pos) + 1;

        db[i] = buffer;
    }

    file.close();
    return db;
}

void MainWindow::WriteToFile()
{
    ofstream file(user.path);

    for (unsigned int i = 0; i < lines; i++)
    {
        file << db[i].id << "," << db[i].username << "," << db[i].purpose << "," << db[i].password << ",\n";
    }

    file.close();

    std::stringstream statusbuffer;
    statusbuffer << "Wrote " << lines << " entries.";

    status->setText(QString(statusbuffer.str().c_str()));
}

void MainWindow::Import()
{
    char* buf;
    size_t buffersize;
    #if (defined (_WIN32) || defined (_WIN64))
        buffersize = sizeof(char) * (strlen(getenv("USERPROFILE")) + strlen("\\Documents") + 1);
        buf = (char *)calloc(buffersize, sizeof(char));
        strcpy(buf, getenv("USERPROFILE"));
        strcat(buf, "\\Documents");
        buf[buffersize] = '\0';
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        buffersize = sizeof(char) * strlen(getenv("HOME"));
        buf = (char *)calloc(buffersize, sizeof(char));
        strcpy(buf, getenv("HOME"));
        buf[buffersize] = '\0';
    #endif

    QString filepath = QFileDialog::getOpenFileName(this, "Import", QString(buf));

    if (filepath.isEmpty())
        return;

    int areyousure = QMessageBox::warning(this, "Warning", "This will replace all entries in the current database with those of the imported database.\n"
                                                           "This operation is not recoverable. Do you wish to proceed ?", QMessageBox::Yes | QMessageBox::No);

    if (areyousure == QMessageBox::No)
        return;

    free(db);

    ifstream newfile(filepath.toStdString().c_str());
    ofstream originalfile(user.path);

    originalfile << newfile.rdbuf();

    newfile.close();
    originalfile.close();
    RefreshView();
}

void MainWindow::Export()
{
    char* buf;
    size_t buffersize;
    #if (defined (_WIN32) || defined (_WIN64))
        buffersize = sizeof(char) * (strlen(getenv("USERPROFILE")) + strlen("\\Documents") + 1);
        buf = (char *)calloc(buffersize, sizeof(char));
        strcpy(buf, getenv("USERPROFILE"));
        strcat(buf, "\\Documents");
        buf[buffersize] = '\0';
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        buffersize = sizeof(char) * strlen(getenv("HOME"));
        buf = (char *)calloc(buffersize, sizeof(char));
        strcpy(buf, getenv("HOME"));
        buf[buffersize] = '\0';
    #endif

    QString filepath = QFileDialog::getSaveFileName(this, "Export", QString(buf));

    ofstream file(filepath.toStdString().c_str());

    for (unsigned int i = 0; i < lines; i++)
    {
        file << db[i].id << "," << db[i].username << "," << db[i].purpose << "," << db[i].password << ",\n";
    }

    file.close();

    std::stringstream statusbuffer;
    statusbuffer << "Exported database to " << filepath.toStdString().c_str() << ".";

    status->setText(QString(statusbuffer.str().c_str()));
}

void MainWindow::Backup()
{
    char backuppath[255];
    strcpy(backuppath, user.path);
    strcat(backuppath, ".bak");

    ofstream file(backuppath);

    for (unsigned int i = 0; i < lines; i++)
    {
        file << db[i].id << "," << db[i].username << "," << db[i].purpose << "," << db[i].password << ",\n";
    }

    file.close();
}

void MainWindow::Delete()
{
    Backup();

    QItemSelectionModel* selection = table->selectionModel();

    if (selection->hasSelection())
    {
       int areyousure = QMessageBox::warning(this, "Warning", "This will delete the entire row from the database.\n"
                                                              "This operation is not recoverable. Do you wish to proceed ?", QMessageBox::Yes | QMessageBox::No);

        if (areyousure == QMessageBox::No)
            return;
        else
        {
            unsigned int i = table->selectionModel()->currentIndex().row();

            DBRow* temparray = (DBRow *)calloc(lines-1, sizeof(DBRow));

            int k = 0;
            for (unsigned int j = 0; j < lines; j++)
            {
                if (j != i)
                {
                    temparray[k] = db[j];
                    k++;
                }
            }

            lines--;
            db = (DBRow *)realloc(db, sizeof(DBRow) * lines);

            for (unsigned int j = 0; j < lines; j++)
            {
                db[j] = temparray[j];
            }

            for (unsigned int j = i; j < lines; j++)
                db[j].id--;
        }
    }

    WriteToFile();
    RefreshView();
}

void MainWindow::Edit()
{
    Backup();
    QItemSelectionModel* selection = table->selectionModel();

    if (selection->hasSelection())
    {
       int i = table->selectionModel()->currentIndex().row();
       int j = table->selectionModel()->currentIndex().column();

       QString textbuffer = table->model()->data(table->model()->index(i, j)).toString();
       bool proceed = true;

       switch (j)
       {
       case 0:
           return;
       case 1:
           db[i].username = (char *)QInputDialog::getText(this, "Edit", "", QLineEdit::Normal, textbuffer, &proceed).toStdString().c_str();
           break;
       case 2:
           db[i].purpose = (char *)QInputDialog::getText(this, "Edit", "", QLineEdit::Normal, textbuffer, &proceed).toStdString().c_str();
           break;
       case 3:
           db[i].password = (char *)QInputDialog::getText(this, "Edit", "", QLineEdit::Normal, textbuffer, &proceed).toStdString().c_str();
           break;
       }

       if (strcmp(db[i].username, "") == 0)
           strcpy(db[i].username, "<none>");

       if (strcmp(db[i].purpose, "") == 0)
           strcpy(db[i].purpose, "<none>");

       if (strcmp(db[i].password, "") == 0)
           strcpy(db[i].password, "<none>");

       if (proceed == false)
           return;
    }

    WriteToFile();
    RefreshView();
}

void MainWindow::Copy()
{
    QItemSelectionModel* selection = table->selectionModel();

    if (selection->hasSelection())
    {
        int i = table->selectionModel()->currentIndex().row();

        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(QString(table->model()->data(table->model()->index(i, 3)).toString()));

        stringstream statusbuffer;
        statusbuffer << "Copied password from row " << i+1 << " to clipboard";

        status->setText(QString(statusbuffer.str().c_str()));
    }
}
