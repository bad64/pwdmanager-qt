#include "mainwindow.h"

using namespace std;

DBRow *MainWindow::ReadFromFile()
{
    free(db);

    fstream file(user.path, ios::in);

    if (!file)
    {
        file.open(user.path, ios::out);
        file.close();
        file.open(user.path, ios::in);
    }

    string line;
    lines = 0;
    while(getline(file, line))
        lines++;

    DBRow* db = (DBRow *)malloc(sizeof(DBRow) * lines);
    if (db == NULL)
    {
        cout << "Heap allocation failed !" << endl;
        exit(1);
    }
    else
        cout << lines << " lines read" << endl;

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
        buffer.username = (char *)malloc(strlen(token.c_str())+1);
        strcpy(buffer.username, token.c_str());
        buffer.username[strlen(buffer.username)] = '\0';
        pos = line.find(delimiter, pos) + 1;

        token = line.substr(pos, line.find(delimiter, pos)-pos);
        buffer.purpose = (char *)malloc(strlen(token.c_str())+1);
        strcpy(buffer.purpose, token.c_str());
        buffer.purpose[strlen(buffer.purpose)] = '\0';
        pos = line.find(delimiter, pos) + 1;

        token = line.substr(pos, line.find(delimiter, pos)-pos);
        buffer.password = (char *)malloc(strlen(token.c_str())+1);
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
    fstream file(user.path, ios::out);

    for (unsigned int i = 0; i < lines; i++)
    {
        file << db[i].id << "," << db[i].username << "," << db[i].purpose << "," << db[i].password << ",\n";
    }

    file.close();
}

void MainWindow::Delete()
{
   QItemSelectionModel* selection = table->selectionModel();

   if (selection->hasSelection())
   {
       int areyousure = QMessageBox::warning(this, "Warning", "This will delete the entire row from the database.\nThis operation is not recoverable. Do you wish to proceed ?", QMessageBox::Yes | QMessageBox::No);

        if (areyousure == QMessageBox::No)
            return;
        else
        {
            unsigned int i = table->selectionModel()->currentIndex().row();

            DBRow* temparray = (DBRow *)malloc(sizeof(DBRow) * (lines-1));

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
   db = ReadFromFile();
   RefreshView();
}

void MainWindow::Edit()
{
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

       if (proceed == false)
           return;
   }

   WriteToFile();
   db = ReadFromFile();
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

       stringstream infotext;
       infotext << "Copied password from row " << i+1 << " to clipboard !" << endl;

       QMessageBox::information(this, "Information", infotext.str().c_str());
   }
}
