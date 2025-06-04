#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include "Undo_Redo.h"
#include <QDateTimeEdit>
#include "Service.h"


class AddEvent : public QWidget
{
    Q_OBJECT

public:
    explicit AddEvent(Service& service, CommandManager& commandManager, QWidget* parent = nullptr);

private slots:
    void onAddClicked();
    void onCancelClicked();

private:
    Service& service;
    CommandManager& commandManager;
    QLineEdit* titleEdit;
    QTextEdit* descriptionEdit;
    QLineEdit* linkEdit;
    QLineEdit* nrPeopleEdit;
    QDateTimeEdit* dateTimeEdit;
    QPushButton* addButton;
    QPushButton* cancelButton;
};

