#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QInputDialog>
#include "ui_GUI.h"
#include "Service.h"
#include "Undo_Redo.h"
#include <QShortcut>
#include <QMessageBox>
#include <QFileDialog>
#include <QComboBox>

class UserMenu : public QWidget
{
	Q_OBJECT

private:
	Service& service;
	CommandManager commandManager;
	QPushButton* viewByMonthButton;
	QPushButton* removeEventButton;
	QPushButton* viewParticipatingEventsButton;
	QPushButton* saveEventsButton;
	QPushButton* displayFileEventsButton;
	QPushButton* undoButton;
	QPushButton* redoButton;
	QPushButton* closeButton;
	QLabel* label;

	void setUserMenu();

private slots:
	void showEventsMonth();
	void removeEventUser();
	void listEventsUser();
	void saveEventsToFile();
	void displayEventsToFile();
	void undo();
	void redo();

public:
	explicit UserMenu(Service& service, QWidget* parent = nullptr);
	~UserMenu() override = default;
};

