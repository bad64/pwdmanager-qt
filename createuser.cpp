#include "createuser.h"

CreateUser::CreateUser()
{
    QWidget::setWindowTitle(tr("New Credentials Wizard"));
    this->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    mainLayout = new QVBoxLayout(this);

    usernameLayout = new QVBoxLayout;
    usernameLabel = new QLabel(tr("Username: "), this);
    usernameLine = new QLineEdit;
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameLine);

    purposeLayout = new QVBoxLayout;
    purposeLabel = new QLabel(tr("Used for: "), this);
    purposeLine = new QLineEdit;
    purposeLayout->addWidget(purposeLabel);
    purposeLayout->addWidget(purposeLine);

    passwordLayout = new QVBoxLayout;
    passwordLabel = new QLabel(tr("Enter your password below, or generate a new one: "), this);
    passwordLine = new QLineEdit;
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordLine);

    lengthLayout = new QHBoxLayout;
    lengthLabel = new QLabel(tr("Length:"));
    requiredLength = new QSpinBox();
    requiredLength->setValue(10);
    requiredLength->setMinimum(5);
    lengthLayout->addWidget(lengthLabel);
    lengthLayout->addWidget(requiredLength);

    generationOptions = new QGridLayout;
    requiresCapitals = new QCheckBox(tr("Uppercase characters"));
    requiresCapitals->setCheckState(Qt::Checked);
    requiresNumerals = new QCheckBox(tr("Numeric characters"));
    requiresNumerals->setCheckState(Qt::Checked);
    requiresSpecials = new QCheckBox(tr("Special characters"));
    requiresSpecials->setCheckState(Qt::Unchecked);
    generateButton = new QPushButton(tr("Generate"));
    QWidget::connect(generateButton, SIGNAL(clicked(bool)), this, SLOT(requestGenerate(void)));
    generationOptions->addLayout(lengthLayout, 0, 0);
    generationOptions->addWidget(requiresCapitals, 0, 1);
    generationOptions->addWidget(requiresNumerals, 1, 0);
    generationOptions->addWidget(requiresSpecials, 1, 1);
    generationOptions->addWidget(generateButton, 2, 1);

    dialogButtons = new QHBoxLayout;
    validateButton = new QPushButton(tr("Validate"));
    QWidget::connect(validateButton, SIGNAL(clicked(bool)), this, SLOT(validate()));
    cancelButton = new QPushButton(tr("Cancel"));
    QWidget::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(quit()));
    dialogButtons->addStretch(2);
    dialogButtons->addWidget(validateButton);
    dialogButtons->addWidget(cancelButton);
    dialogButtons->setContentsMargins(0, 25, 0, 0);

    mainLayout->addLayout(usernameLayout);
    mainLayout->addLayout(purposeLayout);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addLayout(generationOptions);
    mainLayout->addLayout(dialogButtons);
}

void CreateUser::validate()
{
    if (usernameLine->text().isEmpty())
        usernameLine->setText("<none>");
    if (purposeLine->text().isEmpty())
        purposeLine->setText("<none>");
    if (passwordLine->text().isEmpty())
        passwordLine->setText("<none>");

    emit returnUserStruct(usernameLine->text().toStdString(), purposeLine->text().toStdString(), passwordLine->text().toStdString());
    this->close();
}

void CreateUser::requestGenerate()
{
    passwordLine->setText(QString(generate(requiredLength->value(), requiresCapitals->isChecked(), requiresNumerals->isChecked(), requiresSpecials->isChecked()).c_str()));
}

void CreateUser::quit()
{
    this->close();
}
