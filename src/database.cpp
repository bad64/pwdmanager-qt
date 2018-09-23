#include "mainwindow.h"

std::vector<DBRow> MainWindow::readFromFile(std::string path)
{
    std::vector<DBRow> database;

    std::fstream file(path, std::ios::in | std::ios::binary);

    if (!file)
    {
        createBinaryFile(path);    //If db does not exist, create it
        file.open(path, std::ios::in | std::ios::binary);
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
        }
        else
        {
            lines = 0;

            //Get the number of entries in the file
            int startOfText = static_cast<int>(file.tellg());

            while (file)
            {
                file.get(buf);
                if (buf == '\x01E')
                    lines++;
            }

            file.close();
            file.open(path, std::ios::in | std::ios::binary);
            file.seekg(startOfText);    //Back to end of header

            //Change status to tell the user that the file is good
            std::stringstream statusbuffer;
            statusbuffer << tr("Read ").toStdString() << lines << tr(" lines").toStdString();
            status->setText(QString(statusbuffer.str().c_str()));

            //Now we can actually read the file into the array
            std::stringstream ssbuf;
            std::string encryptedbuf;
            file.get(buf);

            for (unsigned int i = 0; i < lines; i++)
            {
                database.emplace_back();

                while (buf != ',')
                {
                    if (buf != ',')
                        ssbuf << buf;
                    file.get(buf);
                }

                encryptedbuf = xorCrypt(ssbuf.str());
                database[i].username = encryptedbuf.c_str();
                ssbuf.str(std::string());
                file.get(buf);

                while (buf != ',')
                {
                    if (buf != ',')
                        ssbuf << buf;

                    file.get(buf);
                }

                encryptedbuf = xorCrypt(ssbuf.str());
                database[i].purpose = encryptedbuf.c_str();
                ssbuf.str(std::string());
                file.get(buf);

                while (buf != '\x01E')
                {
                    if (buf != '\x01E')
                        ssbuf << buf;
                    file.get(buf);
                }

                encryptedbuf = xorCrypt(ssbuf.str());
                database[i].password = encryptedbuf.c_str();
                ssbuf.str(std::string());

                file.get(buf);
            }
        }
    }

    file.close();
    return database;
}

void MainWindow::writeToFile()
{
    std::fstream file(user.path, std::ios::out | std::ios::binary);

    file << '\x001' << "CRDNTLS" << '\x002'; //Header

    for (unsigned int i = 0; i < db.size(); i++)
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
                int i = table->selectionModel()->currentIndex().row();
                db.erase(db.begin()+i);
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
        QItemSelectionModel* selection = table->selectionModel();

        if (selection->hasSelection())
        {
            backup();
            unsigned int i = static_cast<unsigned int>(table->selectionModel()->currentIndex().row());
            unsigned int j = static_cast<unsigned int>(table->selectionModel()->currentIndex().column());


            QString textbuffer = table->model()->data(table->model()->index(static_cast<int>(i), static_cast<int>(j))).toString();
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
               newPassword = new MiniGenerateBox(static_cast<int>(i), db[i].password);
               QWidget::connect(newPassword, SIGNAL(returnPassword(uint, std::string)), this, SLOT(setNewPassword(uint, std::string)));
               newPassword->exec();
               delete newPassword;
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

            std::stringstream statusbuffer;
            statusbuffer << tr("Copied password from row ").toStdString() << i+1 << tr("\nto clipboard").toStdString();

            status->setText(QString(statusbuffer.str().c_str()));
        }
    }
}

void MainWindow::backup()
{
    std::string backuppath = user.path;
    backuppath.append(".bak");

    std::fstream file(backuppath.c_str(), std::ios::out | std::ios::binary);

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

    std::fstream originalfile(user.path.c_str(), std::ios::out | std::ios::binary);
    std::fstream backup(backuppath.c_str(), std::ios::in | std::ios::binary);

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
               DBRow temp = db[static_cast<size_t>(i)-1];

               db[static_cast<size_t>(i)-1] = db[static_cast<size_t>(i)];
               db[static_cast<size_t>(i)] = temp;

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
           int i = table->selectionModel()->currentIndex().row();
           int j = table->selectionModel()->currentIndex().column();

           if (static_cast<unsigned int>(i) + 1 < lines)
           {
               DBRow temp = db[static_cast<size_t>(i)+1];

               db[static_cast<size_t>(i)+1] = db[static_cast<size_t>(i)];
               db[static_cast<size_t>(i)] = temp;

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
    #elif (defined (LINUX) || defined (__linux__))
        buf = getenv("HOME");
    #endif

    QString filepath = QFileDialog::getOpenFileName(this, "Import", QString(buf.c_str()), QString("Credentials Database (*.crdb *.db, *.crdb.bak, *db.bak);;All files (*.*)"));

    if (filepath.isEmpty())
        return;

    backup();
    db.clear();

    std::fstream newfile(filepath.toStdString().c_str(), std::ios::in | std::ios::binary);
    QFileInfo info(filepath);
    QString ext = info.suffix();

    //Check if valid file
    if ((ext.toStdString() == "crdb") || (ext.toStdString() == "bak"))
    {
        char nbuf;
        newfile.get(nbuf);

        if (nbuf != '\x001')
        {
            QMessageBox::critical(this, tr("Invalid file"), tr("This file does not appear to be a valid file !"), QMessageBox::Ok);

            newfile.close();
            refreshView();

            return;
        }

        int areyousure = QMessageBox::warning(this, tr("Warning"), tr("This will replace all entries in the current database with those of the imported database.\n"
                                                                        "This operation is not recoverable. Do you wish to proceed ?"), QMessageBox::Yes | QMessageBox::No);

        if (areyousure == QMessageBox::No)
            return;

        std::fstream originalfile(user.path, std::ios::out | std::ios::binary);
        originalfile << '\x001' << newfile.rdbuf();

        newfile.close();
        originalfile.close();
        refreshView();
    }
    else if (ext.toStdString() == "db")
    {
        convertOldFile(filepath);
    }
    else
    {
        std::cout << "Tried to open " << ext.toStdString() << " file" << std::endl;
    }
}

void MainWindow::merge()
{
    std::string buf;
    #if (defined (_WIN32) || defined (_WIN64))
        buf = getenv("USERPROFILE");
        buf.append("\\Documents");
    #elif (defined (LINUX) || defined (__linux__))
        buf = getenv("HOME");
    #endif

    QString filepath = QFileDialog::getOpenFileName(this, "Merge with...", QString(buf.c_str()), QString("Credentials Database (*.crdb *.crdb.bak);;All files (*.*)"));

    if (filepath.isEmpty())
        return;

    backup();

    std::fstream newfile(filepath.toStdString().c_str(), std::ios::in | std::ios::binary);
    QFileInfo info(filepath);
    QString ext = info.suffix();

    //Check if valid file
    if ((ext == "crdb") || (ext == "bak"))
    {
        char nbuf;
        newfile.get(nbuf);

        if (nbuf != '\x001')
        {
            QMessageBox::critical(this, tr("Invalid file"), tr("This file does not appear to be a valid file !"), QMessageBox::Ok);

            newfile.close();
            refreshView();

            return;
        }

        std::cout << "Reading file " << filepath.toStdString() << " into temp db" << std::endl;
        std::vector<DBRow> tempdb = readFromFile(filepath.toStdString());
        bool match = false;

        std::cout << "Entering comparison test loop" << std::endl;
        for (unsigned int i = 0; i < tempdb.size(); i++)
        {
            for (unsigned int j = 0; j < db.size(); j++)
            {
                if ((tempdb[i].username == db[j].username) && (tempdb[i].username == db[j].username) && (tempdb[i].password == db[j].password))
                {
                    match = true;
                    break;
                }
                else
                    match = false;
            }

            if (!match)
            {
                db.emplace_back();
                db[db.size()-1].username = tempdb[i].username;
                db[db.size()-1].purpose = tempdb[i].purpose;
                db[db.size()-1].password = tempdb[i].password;
            }
        }
    }
    else if (ext == "db")
    {
        QMessageBox::critical(this, tr("Error"), tr("Cannot convert old file format for merging !"), QMessageBox::Ok);

        newfile.close();
        refreshView();

        return;
    }

    newfile.close();
    writeToFile();
    refreshView();
}

void MainWindow::exportToFile()
{
    std::string buf;
    #if (defined (_WIN32) || defined (_WIN64))
        buf = getenv("USERPROFILE");
        buf.append("\\Documents");
    #elif (defined (LINUX) || defined (__linux__))
        buf = getenv("HOME");
    #endif

    QString filepath = QFileDialog::getSaveFileName(this, "exportToFile", QString(buf.c_str()), QString("Credentials Database (*.crdb)"));

    std::fstream file(filepath.toStdString().c_str(), std::ios::out | std::ios::binary);
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

void MainWindow::convertOldFile(QString filepath)
{
    std::string buf;
    #if (defined (_WIN32) || defined (_WIN64))
        buf = getenv("APPDATA");
        buf.append("\\passwordmanager");
    #elif (defined (LINUX) || defined (__linux__))
        buf = getenv("HOME");
    #endif

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

    if (db.size() == 0)
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
            db.emplace_back();

            getline(file, templine);

            char delimiter = ',';
            unsigned int pos = 0;

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
