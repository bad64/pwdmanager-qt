#include "mainwindow.h"

using namespace std;

class readEncryptedBufferException : public std::runtime_error
{
public:
    readEncryptedBufferException(std::string const& msg):
        std::runtime_error(msg)
    {}
};

DBRow *MainWindow::readFromFile()
{
    free(db);

    fstream file(user.path, ios::in | ios::binary);

    if (!file)
    {
        createBinaryFile(user.path);    //If db does not exist, create it
        file.open(user.path, ios::in | ios::binary);
    }

    //Checking header to see if file is valid
    char buf;
    file.get(buf);

    if (buf != '\x001')
    {
        //Bad header
        std::stringstream statusbuffer;
        statusbuffer << tr("Invalid file format").toStdString();

        status->setText(QString(statusbuffer.str().c_str()));

        lines = 0;
        db = (DBRow *)calloc(0, sizeof(DBRow));
    }
    else
    {
        std::stringstream signature;

        while (buf != '\x002')
        {
            file.get(buf);
            if (buf != '\x002')
                signature << buf;
        }

        if (signature.str().compare("CRDNTLS") != 0)
        {
            //Bad signature (possible file corruption or wrong import)
            std::stringstream statusbuffer;
            statusbuffer << tr("Invalid header signature").toStdString();

            status->setText(QString(statusbuffer.str().c_str()));

            lines = 0;
            db = (DBRow *)calloc(0, sizeof(DBRow));
        }
        else
        {
            lines = 0;
            db = (DBRow *)calloc(0, sizeof(DBRow));

            //Get the number of entries in the file
            int startOfText = file.tellg();

            while (file)
            {
                file.get(buf);
                if (buf == '\x01E')
                    lines++;
            }

            file.close();
            file.open(user.path, ios::in | ios::binary);
            file.seekg(startOfText);    //Back to end of header
            db = (DBRow *)calloc(lines, sizeof(DBRow));

            //Change status to tell the user that the file is good
            std::stringstream statusbuffer;
            statusbuffer << tr("Read ").toStdString() << lines << tr(" lines").toStdString();
            status->setText(QString(statusbuffer.str().c_str()));

            //Now we can actually read the file into the array
            std::stringstream ssbuf;
            std::string encryptedbuf;
            file.get(buf);

            try
            {
                for (unsigned int i = 0; i < lines; i++)
                {
                    while (buf != ',')
                    {
                        if (buf != ',')
                            ssbuf << buf;
                        file.get(buf);
                    }

                    encryptedbuf = xorCrypt(ssbuf.str());
                    if (encryptedbuf.empty())
                        throw readEncryptedBufferException("Error reading from file\n");
                    db[i].username = encryptedbuf.c_str();
                    ssbuf.str(std::string());
                    file.get(buf);

                    while (buf != ',')
                    {
                        if (buf != ',')
                            ssbuf << buf;

                        file.get(buf);
                    }

                    encryptedbuf = xorCrypt(ssbuf.str());
                    if (encryptedbuf.empty())
                        throw readEncryptedBufferException("Error reading from file\n");
                    db[i].purpose = encryptedbuf.c_str();
                    ssbuf.str(std::string());
                    file.get(buf);

                    while (buf != '\x01E')
                    {
                        if (buf != '\x01E')
                            ssbuf << buf;
                        file.get(buf);
                    }

                    encryptedbuf = xorCrypt(ssbuf.str());
                    if (encryptedbuf.empty())
                        throw readEncryptedBufferException("Error reading from file\n");
                    db[i].password = encryptedbuf.c_str();
                    ssbuf.str(std::string());

                    file.get(buf);
                }
            }
            catch (std::exception &e)
            {
                restore();
            }
        }
    }

    file.close();
    return db;
}

void MainWindow::writeToFile()
{
    fstream file(user.path, ios::out | ios::binary);

    file << '\x001' << "CRDNTLS" << '\x002'; //Header

    for (unsigned int i = 0; i < lines; i++)
    {
        file << xorCrypt(db[i].username) << "," << xorCrypt(db[i].purpose) << "," << xorCrypt(db[i].password) << "\x01E";
    }

    file << EOF;
    file.close();

    std::stringstream statusbuffer;
    if (lines == 1)
        statusbuffer << tr("Wrote 1 entry.").toStdString();
    else
        statusbuffer << tr("Wrote ").toStdString() << lines << tr(" entries.").toStdString();

    status->setText(QString(statusbuffer.str().c_str()));
}

void MainWindow::deleteRow()
{
    if (!hide)  //Disallow delete if table is hidden
    {
        backup();

        QItemSelectionModel* selection = table->selectionModel();

        if (selection->hasSelection())
        {
           int areyousure = QMessageBox::warning(this, tr("Warning"), tr("This will delete the entire row from the database.\n"
                                                                            "This operation is not recoverable. Do you wish to proceed ?"), QMessageBox::Yes | QMessageBox::No);

            if (areyousure == QMessageBox::No)
                return;
            else
            {
                unsigned int i = table->selectionModel()->currentIndex().row();

                //Create db array of size n-1
                DBRow* temparray = (DBRow *)calloc(lines-1, sizeof(DBRow));

                //Copy current db to temporary array except the line to be deleted
                int k = 0;
                for (unsigned int j = 0; j < lines; j++)
                {
                    if (j != i)
                    {
                        temparray[k] = db[j];
                        k++;
                    }
                }

                //Downsize actual db
                lines--;
                db = (DBRow *)realloc(db, sizeof(DBRow) * lines);   //Realloc doesn't keep array contents, hence why we store the db in a temp array

                //Copy back everything into the database
                for (unsigned int j = 0; j < lines; j++)
                {
                    db[j] = temparray[j];
                }
            }
        }

        //Finally, write db to file and present view to the user
        writeToFile();
        refreshView();
    }
}

void MainWindow::edit()
{
    if (!hide)  //Disallow edits while table is hidden
    {
        backup();
        QItemSelectionModel* selection = table->selectionModel();

        if (selection->hasSelection())
        {
           int i = table->selectionModel()->currentIndex().row();
           int j = table->selectionModel()->currentIndex().column();

           QString textbuffer = table->model()->data(table->model()->index(i, j)).toString();
           std::string previousValue;
           bool proceed = true;

           switch (j)
           {
           case 0:
               previousValue = db[i].username;
               db[i].username = QInputDialog::getText(this, tr("Edit"), tr("Enter a new username:"), QLineEdit::Normal, textbuffer, &proceed, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint).toStdString();
               break;
           case 1:
               previousValue = db[i].purpose;
               db[i].purpose = QInputDialog::getText(this, tr("Edit"), tr("Enter what these credentials will be used for:"), QLineEdit::Normal, textbuffer, &proceed, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint).toStdString();
               break;
           case 2:
               previousValue = db[i].password;
               newPassword = new MiniGenerateBox(i, db[i].password);
               QWidget::connect(newPassword, SIGNAL(returnPassword(uint, std::string)), this, SLOT(setNewPassword(uint, std::string)));
               newPassword->exec();
               break;
           }

           if (proceed == false)
           {
               switch (j)
               {
               case 0:
                   db[i].username = previousValue;
                   break;
               case 1:
                   db[i].purpose = previousValue;
                   break;
               case 2:
                   //Should not be reachable due to how MiniGenerateBox works, but just in case
                   db[i].password = previousValue;
                   break;
               }
           }
           else
           {
               if (db[i].username.empty())
                   db[i].username = "<none>";

               if (db[i].purpose.empty())
                   db[i].purpose = "<none>";

               if (db[i].password.empty())
                   db[i].password = "<none>";

               writeToFile();
               refreshView();
           }
        }
    }
}

void MainWindow::copy()
{
    if (!hide)  //Disallow copy while table is hidden
    {
        QItemSelectionModel* selection = table->selectionModel();

        if (selection->hasSelection())
        {
            int i = table->selectionModel()->currentIndex().row();

            QClipboard* clipboard = QApplication::clipboard();
            clipboard->setText(QString(table->model()->data(table->model()->index(i, 2)).toString()));

            stringstream statusbuffer;
            statusbuffer << tr("Copied password from row ").toStdString() << i+1 << tr("\nto clipboard").toStdString();

            status->setText(QString(statusbuffer.str().c_str()));
        }
    }
}

void MainWindow::backup()
{
    std::string backuppath = user.path;
    backuppath.append(".bak");

    fstream file(backuppath.c_str(), ios::out | ios::binary);

    file << '\x001' << "CRDNTLS" << '\x002'; //Header

    for (unsigned int i = 0; i < lines; i++)
    {
        file << xorCrypt(db[i].username) << "," << xorCrypt(db[i].purpose) << "," << xorCrypt(db[i].password) << "\x01E";
    }

    file << EOF;
    file.close();
}

void MainWindow::restore()
{
    std::string backuppath = user.path;
    backuppath.append(".bak");

    fstream originalfile(user.path.c_str(), std::ios::out | std::ios::binary);
    fstream backup(backuppath.c_str(), std::ios::in | std::ios::binary);

    originalfile << backup.rdbuf();

    backup.close();
    originalfile.close();
    refreshView();
}

void MainWindow::moveRowUp()
{
    if (!hide)  //Disallow moves while table is hidden
    {
        backup();
        QItemSelectionModel* selection = table->selectionModel();

        if (selection->hasSelection())
        {
           int i = table->selectionModel()->currentIndex().row();
           int j = table->selectionModel()->currentIndex().column();

           if (i - 1 >= 0)
           {
               DBRow temp = db[i-1];

               db[i-1] = db[i];
               db[i] = temp;

               writeToFile();
               refreshView();

               QModelIndex next = table->model()->index(i-1, j);
               table->setCurrentIndex(next);
           }
        }
    }
}

void MainWindow::moveRowDown()
{
    if (!hide)  //Disallow moves while table is hidden
    {
        backup();
        QItemSelectionModel* selection = table->selectionModel();

        if (selection->hasSelection())
        {
           unsigned int i = table->selectionModel()->currentIndex().row();
           unsigned int j = table->selectionModel()->currentIndex().column();

           if (i + 1 < lines)
           {
               DBRow temp = db[i+1];

               db[i+1] = db[i];
               db[i] = temp;

               writeToFile();
               refreshView();

               QModelIndex next = table->model()->index(i+1, j);
               table->setCurrentIndex(next);
           }
        }
    }
}

void MainWindow::importFromFile()
{
    std::string buf;
    #if (defined (_WIN32) || defined (_WIN64))
        buf = getenv("USERPROFILE");
        buf.append("\\Documents");
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        buf = getenv("HOME");
    #endif

    QString filepath = QFileDialog::getOpenFileName(this, "Import", QString(buf.c_str()), QString("Credentials Database (*.crbd)"));

    if (filepath.isEmpty())
        return;

    int areyousure = QMessageBox::warning(this, tr("Warning"), tr("This will replace all entries in the current database with those of the imported database.\n"
                                                                    "This operation is not recoverable. Do you wish to proceed ?"), QMessageBox::Yes | QMessageBox::No);

    if (areyousure == QMessageBox::No)
        return;

    free(db);

    fstream newfile(filepath.toStdString().c_str(), ios::in | ios::binary);

    //Check if valid file
    char nbuf;
    newfile.get(nbuf);

    if (nbuf != '\x001')
    {
        QMessageBox::critical(this, tr("Invalid file"), tr("This file does not appear to be a valid .crdb file !"), QMessageBox::Ok);

        newfile.close();
        refreshView();

        return;
    }

    fstream originalfile(user.path, ios::out | ios::binary);
    originalfile << '\x001' << newfile.rdbuf();

    newfile.close();
    originalfile.close();
    refreshView();
}

void MainWindow::exportToFile()
{
    std::string buf;
    #if (defined (_WIN32) || defined (_WIN64))
        buf = getenv("USERPROFILE");
        buf.append("\\Documents");
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        buf = getenv("HOME");
    #endif

    QString filepath = QFileDialog::getSaveFileName(this, "exportToFile", QString(buf.c_str()), QString("Credentials Database (*.crbd)"));

    fstream file(filepath.toStdString().c_str(), ios::out | ios::binary);
    file << '\x001' << "CRDNTLS" << '\x002';

    for (unsigned int i = 0; i < lines; i++)
    {
        file << xorCrypt(db[i].username) << "," << xorCrypt(db[i].purpose) << "," << xorCrypt(db[i].password) << "\x01E";
    }

    file << EOF;
    file.close();

    std::stringstream statusbuffer;
    statusbuffer << tr("Exported database to\n").toStdString() << filepath.toStdString().c_str() << ".";

    status->setText(QString(statusbuffer.str().c_str()));
}

void MainWindow::convertOldFile()
{
    std::string buf;
    #if (defined (_WIN32) || defined (_WIN64))
        buf = getenv("APPDATA");
        buf.append("\\passwordmanager");
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        buf = getenv("HOME");
    #endif

    QString filepath = QFileDialog::getOpenFileName(this, "Convert", QString(buf.c_str()), QString("Old Credentials Database (*.db)"));

    if (filepath.isEmpty())
        return;

    int areyousure = QMessageBox::warning(this, tr("Warning"), tr("The software will now attempt to convert this file to the new format.\n"
                                                                    "This operation may fail if the file is malformed. Do you wish to proceed ?"), QMessageBox::Yes | QMessageBox::No);

    if (areyousure == QMessageBox::No)
        return;

    std::fstream file(filepath.toStdString().c_str(), std::ios::in);
    std::string templine;

    lines = 0;
    while(getline(file, templine))
        lines++;

    db = (DBRow *)calloc(lines, sizeof(DBRow));
    if (db == NULL)
    {
        std::cout << "Heap allocation failed !" << std::endl;
        exit(1);
    }
    else
    {
        file.close();
        file.open(filepath.toStdString().c_str(), std::ios::in);

        for (unsigned int i = 0; i < lines; i++)
        {
            DBRow buffer;

            getline(file, templine);

            char delimiter = ',';
            int pos = 0;

            std::string token = templine.substr(0, templine.find(delimiter));
            pos = templine.find(delimiter, pos) + 1;

            token = templine.substr(pos, templine.find(delimiter, pos)-pos);
            buffer.username = token;
            pos = templine.find(delimiter, pos) + 1;

            token = templine.substr(pos, templine.find(delimiter, pos)-pos);
            buffer.purpose = token;
            pos = templine.find(delimiter, pos) + 1;

            token = templine.substr(pos, templine.find(delimiter, pos)-pos);
            buffer.password = token;
            pos = templine.find(delimiter, pos) + 1;

            db[i] = buffer;
        }

        writeToFile();
        refreshView();
    }
}
