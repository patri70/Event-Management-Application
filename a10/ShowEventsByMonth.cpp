#include "ShowEventsByMonth.h"
#include <QDesktopServices>
#include <QUrl>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

ShowEventsByMonth::ShowEventsByMonth(Service& service, QWidget* parent)
    : QWidget(parent), service(service), currentIndex(0)
{
    setWindowTitle("Add User Event");
    auto* layout = new QVBoxLayout(this);

    auto* monthLabel = new QLabel("Enter the number of the month:", this);
    monthEdit = new QLineEdit(this);
    searchButton = new QPushButton("Search", this);

    eventLabel = new QLabel(this);
    eventLabel->setWordWrap(true);

    yesButton = new QPushButton("Yes", this);
    noButton = new QPushButton("No, show next event", this);
    backButton = new QPushButton("Back to main menu", this);

    auto* topLayout = new QHBoxLayout();
    topLayout->addWidget(monthLabel);
    topLayout->addWidget(monthEdit);
    topLayout->addWidget(searchButton);

    layout->addLayout(topLayout);
    layout->addWidget(eventLabel);

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(yesButton);
    buttonLayout->addWidget(noButton);
    buttonLayout->addWidget(backButton);
    layout->addLayout(buttonLayout);

    connect(searchButton, &QPushButton::clicked, this, &ShowEventsByMonth::onSearchClicked);
    connect(yesButton, &QPushButton::clicked, this, &ShowEventsByMonth::onYesClicked);
    connect(noButton, &QPushButton::clicked, this, &ShowEventsByMonth::onNoClicked);
    connect(backButton, &QPushButton::clicked, this, &ShowEventsByMonth::onBackClicked);

    yesButton->setEnabled(false);
    noButton->setEnabled(false);
}

void ShowEventsByMonth::onSearchClicked()
{
    bool ok = false;
    int month = monthEdit->text().toInt(&ok);
    if (!ok || month < 1 || month > 12) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid month number (1-12).");
        return;
    }

    events = service.getEventsByMonth(month);
    currentIndex = 0;

    if (events.empty()) {
        eventLabel->setText("No events found for the given month.");
        yesButton->setEnabled(false);
        noButton->setEnabled(false);
    }
    else {
        yesButton->setEnabled(true);
        noButton->setEnabled(true);
        showCurrentEvent();
    }
}

void ShowEventsByMonth::showCurrentEvent()
{
    if (events.empty()) {
        eventLabel->setText("No events to show.");
        return;
    }
    const Event& event = events[currentIndex];
    eventLabel->setText(QString::fromStdString(event.toString()));
     QDesktopServices::openUrl(QUrl(QString::fromStdString(event.getLink())));
}

void ShowEventsByMonth::onYesClicked()
{
    if (events.empty()) return;
    const Event& event = events[currentIndex];
    try {
        if (service.addUserEvent(event)) {
            QMessageBox::information(this, "Success", "You are attending this event!");
        }
        else {
            QMessageBox::warning(this, "Error", "Could not add event to your list.");
        }
    }
    catch (const std::exception& ex) {
        QMessageBox::warning(this, "Error", ex.what());
    }
    currentIndex = (currentIndex + 1) % events.size();
    showCurrentEvent();
}

void ShowEventsByMonth::onNoClicked()
{
    if (events.empty()) return;
    currentIndex = (currentIndex + 1) % events.size();
    showCurrentEvent();
}

void ShowEventsByMonth::onBackClicked()
{
    close();
}

