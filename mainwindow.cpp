#include "mainwindow.h"

MainWindow::MainWindow()
{
    QWidget* mainArea = new QWidget;
    setCentralWidget(mainArea);

    //Prepping the database
    db = (DBRow *)malloc(0);
    lines = 0;

    //Menu bar
    QMenu* optionFile = menuBar()->addMenu("&File");
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
    QGridLayout *mainLayout = new QGridLayout;

    //Table holding contents of passwords file
    table = new QTableWidget(0, 4);
    table->setHorizontalHeaderLabels(QString("ID;USERNAME;USED FOR;PASSWORD").split(";"));
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setColumnWidth(0, 50);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QObject::connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(Edit()));

    //Widgets for the right bar
    sboxlabel = new QLabel("Search:");
    sboxlabel->setContentsMargins(0,0,0,0);

    searchbox = new QLineEdit();
    searchbox->setMinimumSize(150, 25);
    searchbox->setMaximumHeight(25);
    searchbox->setContentsMargins(0,0,0,0);
    QObject::connect(searchbox, SIGNAL(textChanged(QString)), this, SLOT(RefreshView()));

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

    //Piece it all together
    mainLayout->addWidget(table, 0, 0, 10, 7);
    mainLayout->addWidget(sboxlabel, 0, 7, 1, 2);
    mainLayout->addWidget(searchbox, 1, 7, 1, 2);
    mainLayout->addWidget(newButton, 3, 7, 1, 1);
    mainLayout->addWidget(deleteButton, 3, 8, 1, 1);
    mainLayout->addWidget(editButton, 4, 7, 1, 1);
    mainLayout->addWidget(copyButton, 4, 8, 1, 1);

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
}

void MainWindow::RefreshView()
{
    db = ReadFromFile();

    table->setRowCount(0);

    if (searchbox->text().isEmpty())
    {
        for (unsigned int i = 0; i < lines; i++)
        {
            table->insertRow(table->rowCount());

            table->setItem(i, 0, new QTableWidgetItem(QString::number(db[i].id)));
            table->setItem(i, 1, new QTableWidgetItem(db[i].username));
            table->setItem(i, 2, new QTableWidgetItem(db[i].purpose));
            table->setItem(i, 3, new QTableWidgetItem(db[i].password));
        }
    }
    else
    {
        int j = 0;
        for (unsigned int i = 0; i < lines; i++)
        {
            if ((QString::compare(searchbox->text(), db[i].username) == 0) || (QString::compare(searchbox->text(), db[i].purpose) == 0) || (QString::compare(searchbox->text(), db[i].password) == 0))
            {
                table->insertRow(table->rowCount());

                table->setItem(j, 0, new QTableWidgetItem(QString::number(db[i].id)));
                table->setItem(j, 1, new QTableWidgetItem(db[i].username));
                table->setItem(j, 2, new QTableWidgetItem(db[i].purpose));
                table->setItem(j, 3, new QTableWidgetItem(db[i].password));
                j++;
            }
        }
    }
}
