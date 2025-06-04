#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "Undo_Redo.h"
#include "Service.h"

class ShowEventsByMonth : public QWidget
{
    Q_OBJECT

public:
    explicit ShowEventsByMonth(Service& service, CommandManager& commandManager, QWidget* parent = nullptr);

private slots:
    void onSearchClicked();
    void onYesClicked();
    void onNoClicked();
    void onBackClicked();

private:
    void showCurrentEvent();

    Service& service;
    CommandManager& commandManager;
    std::vector<Event> events;
    int currentIndex;

    QLineEdit* monthEdit;
    QLabel* eventLabel;
    QPushButton* searchButton;
    QPushButton* yesButton;
    QPushButton* noButton;
    QPushButton* backButton;
};


