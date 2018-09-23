#include "mainwindow.h"

MainWindow::MainWindow()
{
    QWidget* mainArea = new QWidget;
    QWidget::setWindowTitle("Password Manager");
    setCentralWidget(mainArea);
    this->setWindowIcon(QIcon(":/resources/pwdmanager-qt.png"));

    hide = false;
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_H), this, SLOT(hideTable()));

    //Prepping the database
    lines = 0;

    //Menu bar
    QMenu *optionFile = menuBar()->addMenu(tr("&File"));
        QAction *actionImport = new QAction(tr("Import"), this);
        optionFile->addAction(actionImport);
        QObject::connect(actionImport, SIGNAL(triggered()), this, SLOT(importFromFile()));

        QAction *actionExport = new QAction(tr("Export"), this);
        optionFile->addAction(actionExport);
        QObject::connect(actionExport, SIGNAL(triggered()), this, SLOT(exportToFile()));

        QAction *actionRestore = new QAction(tr("Restore from backup"), this);
        optionFile->addAction(actionRestore);
        QObject::connect(actionRestore, SIGNAL(triggered()), this, SLOT(restore()));

        QAction *actionMerge = new QAction(tr("Merge"), this);
        optionFile->addAction(actionMerge);
        QObject::connect(actionMerge, SIGNAL(triggered()), this, SLOT(merge()));

        QAction *actionRefresh = new QAction(tr("Refresh View"), this);
        actionRefresh->setShortcut(Qt::Key_F5);
        optionFile->addAction(actionRefresh);
        QObject::connect(actionRefresh, SIGNAL(triggered()), this, SLOT(refreshView()));

        QAction *actionQuit = new QAction(tr("Quit"), this);
        optionFile->addAction(actionQuit);
        QObject::connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu *optionMisc = menuBar()->addMenu("?");
        QAction *actionHelp = new QAction(tr("Help"), this);
        actionHelp->setShortcut(Qt::Key_F1);
        optionMisc->addAction(actionHelp);
        QObject::connect(actionHelp, SIGNAL(triggered()), this, SLOT(help()));

        QAction *actionAbout = new QAction(tr("About"), this);
        optionMisc->addAction(actionAbout);
        QObject::connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    //Actions to move rows up/down
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up), this, SLOT(moveRowUp()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down), this, SLOT(moveRowDown()));

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

MainWindow::~MainWindow()
{

}

void MainWindow::setLanguage(QString language)
{
    currentLanguage = language;
}

int MainWindow::askUserPassword()
{
    bool proceed;
    QString password = QInputDialog::getText(this, tr("Enter password"), tr("Please enter your account password:"), QLineEdit::Password, QString(), &proceed, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    if (try_auth(user.username, password.toStdString()))
        return 1;
    else
        return 0;
}

void MainWindow::init()
{
    //Database to QTableWidget conversion
    for (unsigned int i = 0; i < lines; i++)
    {
        table->insertRow(table->rowCount());

        table->setItem(i, 0, new QTableWidgetItem(db[i].username.c_str()));
        table->setItem(i, 1, new QTableWidgetItem(db[i].purpose.c_str()));
        table->setItem(i, 2, new QTableWidgetItem(db[i].password.c_str()));
    }

    std::stringstream statusbuffer;
    if (lines == 0)
        statusbuffer << tr("Retrieved 0 entries.").toStdString();
    else if (lines == 1)
        statusbuffer << tr("Retrieved 1 entry.").toStdString();
    else if (lines > 1)
        statusbuffer << tr("Retrieved ").toStdString().c_str() << lines << tr(" entries.").toStdString().c_str();
}

void MainWindow::hideTable()
{
    if (hide)
    {
        if (askUserPassword() == 1)
        {
            hide = false;
            refreshView();
            search();
        }
        else
        {
            return;
        }
    }
    else
    {
        hide = true;

        refreshView();
        search();

        std::stringstream statusbuffer;

        if (hide)
            statusbuffer << tr("Hidden table view.\nPress Ctrl+H to unhide").toStdString().c_str();
        else
        {
            if (lines == 0)
                statusbuffer << tr("Retrieved 0 entries.").toStdString();
            else if (lines == 1)
                statusbuffer << tr("Retrieved 1 entry.").toStdString();
            else if (lines > 1)
                statusbuffer << tr("Retrieved ").toStdString().c_str() << lines << tr(" entries.").toStdString().c_str();
        }

        status->setText(QString(statusbuffer.str().c_str()));
    }
}

void MainWindow::refreshView()
{
    db.clear();
    db = readFromFile(user.path);

    table->setRowCount(0);

    if (!hide)
    {
        for (unsigned int i = 0; i < lines; i++)
        {
            table->insertRow(table->rowCount());

            table->setItem(i, 0, new QTableWidgetItem(db[i].username.c_str()));
            table->setItem(i, 1, new QTableWidgetItem(db[i].purpose.c_str()));
            table->setItem(i, 2, new QTableWidgetItem(db[i].password.c_str()));
        }
    }
    else
    {
        table->insertRow(table->rowCount());

        table->setItem(0, 0, new QTableWidgetItem(""));
        table->setRowHeight(0, 0);
    }
}

void MainWindow::search()
{
    if (!hide)  //Disallow search if table is hidden
    {
        table->setRowCount(0);

        if (searchBox->text().isEmpty())
        {
            if (!hide)
            {
                for (unsigned int i = 0; i < lines; i++)
                {
                    table->insertRow(table->rowCount());

                    table->setItem(i, 0, new QTableWidgetItem(db[i].username.c_str()));
                    table->setItem(i, 1, new QTableWidgetItem(db[i].purpose.c_str()));
                    table->setItem(i, 2, new QTableWidgetItem(db[i].password.c_str()));
                }
            }
            else
            {
                for (unsigned int i = 0; i < lines; i++)
                {
                    table->insertRow(table->rowCount());

                    table->setItem(i, 0, new QTableWidgetItem(""));
                    table->setItem(i, 1, new QTableWidgetItem(""));
                    table->setItem(i, 2, new QTableWidgetItem(""));
                }
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
                    QRegularExpressionMatch usernameMatch = regex.match(db[i].username.c_str());
                    QRegularExpressionMatch purposeMatch = regex.match(db[i].purpose.c_str());
                    QRegularExpressionMatch passwordMatch = regex.match(db[i].password.c_str());

                    if ((usernameMatch.hasMatch()) || (purposeMatch.hasMatch()) || (passwordMatch.hasMatch()))
                    {
                        table->insertRow(table->rowCount());

                        table->setItem(j, 0, new QTableWidgetItem(db[i].username.c_str()));
                        table->setItem(j, 1, new QTableWidgetItem(db[i].purpose.c_str()));
                        table->setItem(j, 2, new QTableWidgetItem(db[i].password.c_str()));
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
                    if ((strcmp(searchBox->text().toStdString().c_str(), db[i].username.c_str()) == 0) || (strcmp(searchBox->text().toStdString().c_str(), db[i].purpose.c_str()) == 0) || (strcmp(searchBox->text().toStdString().c_str(), db[i].password.c_str()) == 0))
                    {
                        table->insertRow(table->rowCount());

                        table->setItem(j, 0, new QTableWidgetItem(db[i].username.c_str()));
                        table->setItem(j, 1, new QTableWidgetItem(db[i].purpose.c_str()));
                        table->setItem(j, 2, new QTableWidgetItem(db[i].password.c_str()));
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
}

void MainWindow::clearSearch()
{
    searchBox->setText("");
}
