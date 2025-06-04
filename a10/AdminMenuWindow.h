#pragma once
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QInputDialog>
#include <QShortcut>
#include <QMessageBox>
#include "Service.h"
#include "AddEvent.h"
#include "ui_GUI.h"

class AdminMenuWindow : public QWidget
{
	Q_OBJECT

private:
	Service& service;
	CommandManager commandManager;
	QTableWidget* eventsTable;
	QPushButton* closeButton;
	QPushButton* addButton;
	QPushButton* removeButton;
	QPushButton* updateButton;
	QPushButton* listButton;
	QPushButton* undoButton;
	QPushButton* redoButton;
	QLabel* label;

	void setAdmin();

private slots:
	void viewEvents();
	void addEvent();
	void removeEventAdmin();
	void updateEventAdmin();
	void undo();
	void redo();

public:
	explicit AdminMenuWindow(Service& service, QWidget* parent = nullptr);
	~AdminMenuWindow() override = default;

};

