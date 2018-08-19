#include "mainwindow.h"

MainWindow::MainWindow()
{
    QWidget* mainArea = new QWidget;
    QWidget::setWindowTitle("Password Manager");
    setCentralWidget(mainArea);

    //Prepping the database
    db = (DBRow *)calloc(0, sizeof(DBRow));
    lines = 0;

    //Menu bar
    QMenu* optionFile = menuBar()->addMenu(tr("&File"));
        QAction* actionImport = new QAction(tr("Import"), this);
        optionFile->addAction(actionImport);
        QObject::connect(actionImport, SIGNAL(triggered()), this, SLOT(importFromFile()));

        QAction* actionExport = new QAction(tr("Export"), this);
        optionFile->addAction(actionExport);
        QObject::connect(actionExport, SIGNAL(triggered()), this, SLOT(exportToFile()));

        QAction* actionRefresh = new QAction(tr("Refresh View"), this);
        actionRefresh->setShortcut(Qt::Key_F5);
        optionFile->addAction(actionRefresh);
        QObject::connect(actionRefresh, SIGNAL(triggered()), this, SLOT(refreshView()));

        QAction* actionQuit = new QAction(tr("Quit"), this);
        optionFile->addAction(actionQuit);
        QObject::connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu* optionEdit = menuBar()->addMenu(tr("&Edit"));
        QAction* actionNew = new QAction(tr("New row"), this);
        optionEdit->addAction(actionNew);
        QObject::connect(actionNew, SIGNAL(triggered()), this, SLOT(newEntry()));

        QAction* actionDelete = new QAction(tr("Delete row"), this);
        optionEdit->addAction(actionDelete);
        QObject::connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteRow()));

        QAction* actionEdit = new QAction(tr("Edit cell content"), this);
        optionEdit->addAction(actionEdit);
        QObject::connect(actionEdit, SIGNAL(triggered()), this, SLOT(edit()));

        QAction* actionCopy = new QAction(tr("Copy password"), this);
        optionEdit->addAction(actionCopy);
        QObject::connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));

    QMenu* optionMisc = menuBar()->addMenu("?");
        QAction* actionHelp = new QAction(tr("Help"), this);
        actionHelp->setShortcut(Qt::Key_F1);
        optionMisc->addAction(actionHelp);
        QObject::connect(actionHelp, SIGNAL(triggered()), this, SLOT(help()));

        QAction* actionAbout = new QAction(tr("About"), this);
        optionMisc->addAction(actionAbout);
        QObject::connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    //Main layout
    mainLayout = new QGridLayout;

    //Table holding contents of passwords file
    table = new QTableWidget(0, 3);
    table->setHorizontalHeaderLabels(QString(tr("USERNAME;USED FOR;PASSWORD")).split(";"));
    table->horizontalHeader()->setStretchLastSection(true);
    table->setColumnWidth(0, 100);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QObject::connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit()));

    //Search box
    searchBoxFrame = new QFrame(this);
    searchBoxFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    searchBoxFrame->setLineWidth(0);

    searchBoxLayout = new QVBoxLayout;
    searchBoxSubLayout = new QHBoxLayout;

    searchBoxLabel = new QLabel(tr("Search:"));
    searchBoxLabel->setContentsMargins(0,0,0,0);

    searchBox = new QLineEdit();
    searchBox->setMinimumSize(150, 25);
    searchBox->setMaximumHeight(25);
    searchBox->setContentsMargins(0,0,0,0);
    QObject::connect(searchBox, SIGNAL(textChanged(QString)), this, SLOT(search()));

    exactMatch = new QCheckBox(tr("E&xact match"), this);
    exactMatch->setTristate(false);
    exactMatch->setCheckState(Qt::Unchecked);
    QObject::connect(exactMatch, SIGNAL(stateChanged(int)), this, SLOT(search()));

    clearButton = new QPushButton(tr("Clear"));
    QWidget::connect(clearButton, SIGNAL(clicked(bool)), this, SLOT(clearSearch()));

    searchBoxSubLayout->addWidget(exactMatch);
    searchBoxSubLayout->addWidget(clearButton);

    searchBoxLayout->addWidget(searchBoxLabel);
    searchBoxLayout->addWidget(searchBox);
    searchBoxLayout->addLayout(searchBoxSubLayout);

    searchBoxFrame->setLayout(searchBoxLayout);

    //Button box
    buttonsFrame = new QFrame(this);
    buttonsLayout = new QGridLayout;

    newButton = new QPushButton(tr("New Entry"));
    newButton->setMinimumSize(100, 25);
    QObject::connect(newButton, SIGNAL(clicked(bool)), this, SLOT(newEntry()));

    deleteButton = new QPushButton(tr("Delete"));
    deleteButton->setMinimumSize(100, 25);
    QObject::connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteRow()));

    editButton = new QPushButton(tr("Edit"));
    editButton->setMinimumSize(100, 25);
    QObject::connect(editButton, SIGNAL(clicked(bool)), this, SLOT(edit()));

    copyButton = new QPushButton(tr("Copy"));
    copyButton->setMinimumSize(100, 25);
    QObject::connect(copyButton, SIGNAL(clicked(bool)), this, SLOT(copy()));

    buttonsLayout->addWidget(newButton, 0, 0);
    buttonsLayout->addWidget(deleteButton, 0, 1);
    buttonsLayout->addWidget(editButton, 1, 0);
    buttonsLayout->addWidget(copyButton, 1, 1);

    buttonsFrame->setLayout(buttonsLayout);

    //Other
    statusFrame = new QFrame(this);
    statusFrame->setLineWidth(0);
    statusLayout = new QVBoxLayout;

    status = new QLabel("This should change", statusFrame);
    status->setAlignment(Qt::AlignRight);
    status->setContentsMargins(0,0,0,0);

    statusLayout->addWidget(status);
    statusFrame->setLayout(statusLayout);

    //Piece it all together
    mainLayout->addWidget(table, 0, 0, 10, 7);
    mainLayout->addWidget(searchBoxFrame, 0, 7, 1, 2);
    mainLayout->addWidget(buttonsFrame, 1, 7, 2, 2);
    mainLayout->addWidget(statusFrame, 9, 7, 1, 2);

    this->centralWidget()->setLayout(mainLayout);

    setMinimumSize(640,480);
}

void MainWindow::init()
{
    //Database to QTableWidget conversion
    for (unsigned int i = 0; i < lines; i++)
    {
        table->insertRow(table->rowCount());

        table->setItem(i, 0, new QTableWidgetItem(db[i].username));
        table->setItem(i, 1, new QTableWidgetItem(db[i].purpose));
        table->setItem(i, 2, new QTableWidgetItem(db[i].password));
    }

    std::stringstream statusbuffer;

    if (lines == 0)
        statusbuffer << tr("Retrieved 0 entries.").toStdString();
    else if (lines == 1)
        statusbuffer << tr("Retrieved 1 entry.").toStdString();
    else
        statusbuffer << tr("Retrieved ").toStdString() << lines << tr(" entries.").toStdString();

    status->setText(QString(statusbuffer.str().c_str()));
}

void MainWindow::refreshView()
{
    db = readFromFile();

    table->setRowCount(0);

    for (unsigned int i = 0; i < lines; i++)
    {
        table->insertRow(table->rowCount());

        table->setItem(i, 0, new QTableWidgetItem(db[i].username));
        table->setItem(i, 1, new QTableWidgetItem(db[i].purpose));
        table->setItem(i, 2, new QTableWidgetItem(db[i].password));
    }
}

void MainWindow::search()
{
    table->setRowCount(0);

    if (searchBox->text().isEmpty())
    {
        for (unsigned int i = 0; i < lines; i++)
        {
            table->insertRow(table->rowCount());

            table->setItem(i, 0, new QTableWidgetItem(db[i].username));
            table->setItem(i, 1, new QTableWidgetItem(db[i].purpose));
            table->setItem(i, 2, new QTableWidgetItem(db[i].password));
        }

        std::stringstream statusbuffer;        
        statusbuffer << tr("Retrieved ").toStdString() << lines << tr(" entries.").toStdString();

        status->setText(QString(statusbuffer.str().c_str()));
    }
    else
    {
        if (!exactMatch->isChecked())
        {
            QRegularExpression regex(searchBox->text().toStdString().c_str(), QRegularExpression::CaseInsensitiveOption);

            int j = 0;
            for (unsigned int i = 0; i < lines; i++)
            {
                QRegularExpressionMatch usernameMatch = regex.match(db[i].username);
                QRegularExpressionMatch purposeMatch = regex.match(db[i].purpose);
                QRegularExpressionMatch passwordMatch = regex.match(db[i].password);

                if ((usernameMatch.hasMatch()) || (purposeMatch.hasMatch()) || (passwordMatch.hasMatch()))
                {
                    table->insertRow(table->rowCount());

                    table->setItem(j, 0, new QTableWidgetItem(db[i].username));
                    table->setItem(j, 1, new QTableWidgetItem(db[i].purpose));
                    table->setItem(j, 2, new QTableWidgetItem(db[i].password));
                    j++;
                }
            }

            //Write query results to status
            std::stringstream statusbuffer;

            if (table->rowCount() == 0)
                statusbuffer << tr("No matches found.").toStdString();
            else if (table->rowCount() == 1)
                statusbuffer << tr("Found 1 match.").toStdString();
            else
                statusbuffer << tr("Found ").toStdString() << table->rowCount() << tr(" matches.").toStdString();

            status->setText(QString(statusbuffer.str().c_str()));
        }
        else
        {
            int j = 0;
            for (unsigned int i = 0; i < lines; i++)
            {
                if ((strcmp(searchBox->text().toStdString().c_str(), db[i].username) == 0) || (strcmp(searchBox->text().toStdString().c_str(), db[i].purpose) == 0) || (strcmp(searchBox->text().toStdString().c_str(), db[i].password) == 0))
                {
                    table->insertRow(table->rowCount());

                    table->setItem(j, 0, new QTableWidgetItem(db[i].username));
                    table->setItem(j, 1, new QTableWidgetItem(db[i].purpose));
                    table->setItem(j, 2, new QTableWidgetItem(db[i].password));
                    j++;
                }
            }

            //Write query results to status
            std::stringstream statusbuffer;

            if (table->rowCount() == 0)
                statusbuffer << tr("No matches found.").toStdString();
            else if (table->rowCount() == 1)
                statusbuffer << tr("Found 1 match.").toStdString();
            else
                statusbuffer << tr("Found ").toStdString() << table->rowCount() << tr(" matches.").toStdString();

            status->setText(QString(statusbuffer.str().c_str()));
        }
    }
}

void MainWindow::clearSearch()
{
    searchBox->setText("");
}
