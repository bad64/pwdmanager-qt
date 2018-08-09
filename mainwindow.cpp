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
    QMenu* optionFile = menuBar()->addMenu("&File");
        QAction* actionImport = new QAction("&Import", this);
        optionFile->addAction(actionImport);
        QObject::connect(actionImport, SIGNAL(triggered()), this, SLOT(Import()));

        QAction* actionExport = new QAction("&Export", this);
        optionFile->addAction(actionExport);
        QObject::connect(actionExport, SIGNAL(triggered()), this, SLOT(Export()));

        QAction* actionRefresh = new QAction("&Refresh View", this);
        actionRefresh->setShortcut(Qt::Key_F5);
        optionFile->addAction(actionRefresh);
        QObject::connect(actionRefresh, SIGNAL(triggered()), this, SLOT(RefreshView()));

        QAction* actionQuit = new QAction("&Quit", this);
        optionFile->addAction(actionQuit);
        QObject::connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu* optionEdit = menuBar()->addMenu("&Edit");
        QAction* actionNew = new QAction("&New row", this);
        optionEdit->addAction(actionNew);
        QObject::connect(actionNew, SIGNAL(triggered()), this, SLOT(NewEntry()));

        QAction* actionDelete = new QAction("&Delete row", this);
        optionEdit->addAction(actionDelete);
        QObject::connect(actionDelete, SIGNAL(triggered()), this, SLOT(Delete()));

        QAction* actionEdit = new QAction("&Edit cell content", this);
        optionEdit->addAction(actionEdit);
        QObject::connect(actionEdit, SIGNAL(triggered()), this, SLOT(Edit()));

        QAction* actionCopy = new QAction("&Copy password", this);
        optionEdit->addAction(actionCopy);
        QObject::connect(actionCopy, SIGNAL(triggered()), this, SLOT(Copy()));

    QMenu* optionMisc = menuBar()->addMenu("&?");
        QAction* actionHelp = new QAction("&Help", this);
        actionHelp->setShortcut(Qt::Key_F1);
        optionMisc->addAction(actionHelp);
        QObject::connect(actionHelp, SIGNAL(triggered()), this, SLOT(Help()));

        QAction* actionAbout = new QAction("&About", this);
        optionMisc->addAction(actionAbout);
        QObject::connect(actionAbout, SIGNAL(triggered()), this, SLOT(About()));

    //Main layout
    mainLayout = new QGridLayout;

    //Table holding contents of passwords file
    table = new QTableWidget(0, 4);
    table->setHorizontalHeaderLabels(QString("ID;USERNAME;USED FOR;PASSWORD").split(";"));
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setColumnWidth(0, 50);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QObject::connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(Edit()));

    //Search box
    searchBoxFrame = new QFrame(this);
    searchBoxFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    searchBoxFrame->setLineWidth(0);

    searchBoxLayout = new QVBoxLayout;

    searchBoxLabel = new QLabel("Search:");
    searchBoxLabel->setContentsMargins(0,0,0,0);

    searchBox = new QLineEdit();
    searchBox->setMinimumSize(150, 25);
    searchBox->setMaximumHeight(25);
    searchBox->setContentsMargins(0,0,0,0);
    QObject::connect(searchBox, SIGNAL(textChanged(QString)), this, SLOT(Search()));

    searchBoxLayout->addWidget(searchBoxLabel);
    searchBoxLayout->addWidget(searchBox);

    searchBoxFrame->setLayout(searchBoxLayout);

    //Button box
    buttonsFrame = new QFrame(this);
    buttonsLayout = new QGridLayout;

    newButton = new QPushButton("New Entry");
    newButton->setMinimumSize(100, 25);
    QObject::connect(newButton, SIGNAL(clicked(bool)), this, SLOT(NewEntry()));

    deleteButton = new QPushButton("Delete");
    deleteButton->setMinimumSize(100, 25);
    QObject::connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(Delete()));

    editButton = new QPushButton("Edit");
    editButton->setMinimumSize(100, 25);
    QObject::connect(editButton, SIGNAL(clicked(bool)), this, SLOT(Edit()));

    copyButton = new QPushButton("Copy");
    copyButton->setMinimumSize(100, 25);
    QObject::connect(copyButton, SIGNAL(clicked(bool)), this, SLOT(Copy()));

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

void MainWindow::Init()
{
    //Database to QTableWidget conversion
    for (unsigned int i = 0; i < lines; i++)
    {
        table->insertRow(table->rowCount());

        //printf("%d  %s  %s  %s\n", db[i].id, db[i].username, db[i].purpose, db[i].password);

        table->setItem(i, 0, new QTableWidgetItem(QString::number(db[i].id)));
        table->setItem(i, 1, new QTableWidgetItem(db[i].username));
        table->setItem(i, 2, new QTableWidgetItem(db[i].purpose));
        table->setItem(i, 3, new QTableWidgetItem(db[i].password));
    }

    std::stringstream statusbuffer;
    statusbuffer << "Retrieved " << lines << " entries.";

    status->setText(QString(statusbuffer.str().c_str()));
}

void MainWindow::RefreshView()
{
    db = ReadFromFile();

    table->setRowCount(0);

    for (unsigned int i = 0; i < lines; i++)
    {
        table->insertRow(table->rowCount());

        table->setItem(i, 0, new QTableWidgetItem(QString::number(db[i].id)));
        table->setItem(i, 1, new QTableWidgetItem(db[i].username));
        table->setItem(i, 2, new QTableWidgetItem(db[i].purpose));
        table->setItem(i, 3, new QTableWidgetItem(db[i].password));
    }
}

void MainWindow::Search()
{
    table->setRowCount(0);

    if (searchBox->text().isEmpty())
    {
        for (unsigned int i = 0; i < lines; i++)
        {
            table->insertRow(table->rowCount());

            table->setItem(i, 0, new QTableWidgetItem(QString::number(db[i].id)));
            table->setItem(i, 1, new QTableWidgetItem(db[i].username));
            table->setItem(i, 2, new QTableWidgetItem(db[i].purpose));
            table->setItem(i, 3, new QTableWidgetItem(db[i].password));
        }

        std::stringstream statusbuffer;
        statusbuffer << "Retrieved " << lines << " entries.";

        status->setText(QString(statusbuffer.str().c_str()));
    }
    else
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

                table->setItem(j, 0, new QTableWidgetItem(QString::number(db[i].id)));
                table->setItem(j, 1, new QTableWidgetItem(db[i].username));
                table->setItem(j, 2, new QTableWidgetItem(db[i].purpose));
                table->setItem(j, 3, new QTableWidgetItem(db[i].password));
                j++;
            }
        }

        std::stringstream statusbuffer;
        statusbuffer << "Found " << table->rowCount() << " matches.";

        status->setText(QString(statusbuffer.str().c_str()));
    }
}
