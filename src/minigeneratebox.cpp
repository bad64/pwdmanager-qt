#include "minigeneratebox.h"

MiniGenerateBox::MiniGenerateBox(int nline, std::string oldpw)
{
    QWidget::setWindowTitle(tr("Edit password"));
    this->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    mainLayout = new QVBoxLayout(this);

    line = nline;
    oldpassword = oldpw;

    passwordLayout = new QVBoxLayout;
    passwordLabel = new QLabel(tr("Enter a new password (or generate one):"));
    passwordLine = new QLineEdit;
    passwordLine->setText(QString(oldpw.c_str()));
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

    mainLayout->addLayout(passwordLayout);
    mainLayout->addLayout(generationOptions);
    mainLayout->addLayout(dialogButtons);
}

MiniGenerateBox::~MiniGenerateBox()
{
    delete cancelButton;
    delete validateButton;

    delete generateButton;
    delete requiresSpecials;
    delete requiresNumerals;
    delete requiresCapitals;
    delete requiredLength;
    delete lengthLabel;
    delete lengthLayout;

    delete passwordLine;
    delete passwordLabel;

    delete dialogButtons;
    delete generationOptions;
    delete passwordLayout;

    delete mainLayout;
}

void MiniGenerateBox::validate()
{
    if (passwordLine->text().isEmpty())
        passwordLine->setText("<none>");

    emit returnPassword(line, passwordLine->text().toStdString().c_str());
    this->close();
}

void MiniGenerateBox::requestGenerate()
{
    passwordLine->setText(QString(generate(requiredLength->value(), requiresCapitals->isChecked(), requiresNumerals->isChecked(), requiresSpecials->isChecked()).c_str()));
}

void MiniGenerateBox::quit()
{
    emit returnPassword(line, oldpassword.c_str());
    this->close();
}
