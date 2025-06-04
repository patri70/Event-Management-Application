#include "AddEvent.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

AddEvent::AddEvent(Service& service, CommandManager& commandManager, QWidget* parent)
    : QWidget(parent), service(service), commandManager(commandManager)
{
    setWindowTitle("Add Event");
    auto* layout = new QVBoxLayout(this);
    auto* titleLabel = new QLabel("Title:", this);
    titleEdit = new QLineEdit(this);

    auto* descriptionLabel = new QLabel("Description:", this);
    descriptionEdit = new QTextEdit(this);

    auto* linkLabel = new QLabel("Link:", this);
    linkEdit = new QLineEdit(this);

    auto* nrPeopleLabel = new QLabel("Number of People:", this);
    nrPeopleEdit = new QLineEdit(this);

    auto* dateTimeLabel = new QLabel("Date and Time:", this);
    dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    dateTimeEdit->setCalendarPopup(true);

    addButton = new QPushButton("Add", this);
    cancelButton = new QPushButton("Cancel", this);

    layout->addWidget(titleLabel);
    layout->addWidget(titleEdit);
    layout->addWidget(descriptionLabel);
    layout->addWidget(descriptionEdit);
    layout->addWidget(linkLabel);
    layout->addWidget(linkEdit);
    layout->addWidget(nrPeopleLabel);
    layout->addWidget(nrPeopleEdit);
    layout->addWidget(dateTimeLabel);
    layout->addWidget(dateTimeEdit);

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &AddEvent::onAddClicked);
    connect(cancelButton, &QPushButton::clicked, this, &AddEvent::onCancelClicked);
}

void AddEvent::onAddClicked()
{
    QString title = titleEdit->text();
    QString description = descriptionEdit->toPlainText();
    QString link = linkEdit->text();
    int nrPeople = nrPeopleEdit->text().toInt();
    QDateTime dateTime = dateTimeEdit->dateTime();

    if (title.isEmpty() || description.isEmpty() || link.isEmpty() || nrPeople <= 0) {
        QMessageBox::warning(this, "Input Error", "Please fill all fields correctly.");
        return;
    }
    
    Event event(
        title.toStdString(),
        description.toStdString(),
        link.toStdString(),
        nrPeople,
        DateAndTime(dateTime.date().year(), dateTime.date().month(), dateTime.date().day(),
            dateTime.time().hour(), dateTime.time().minute())
    );

    auto result = commandManager.executeCommand(std::make_unique<AddCommand>(service, event));
    if (result) {
        QMessageBox::information(this, "Success", "Event added successfully!");
        close();
    }
    else {
        QMessageBox::warning(this, "Error", "Event could not be added.");
    }
}

void AddEvent::onCancelClicked()
{
    close();
}
