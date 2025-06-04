#pragma once
#include "ui_GUI.h"
#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "Service.h"

class GUI : public QWidget
{
	Q_OBJECT

private:
	Service& service;
	QTableWidget* eventsTable;
	QPushButton* closeButton;
	QPushButton* adminButton;	
	QPushButton* userButton;
	QSpacerItem* spacer;

	void setupUI();

private slots:
	void adminMenu();
	void userMenu();

public: 
	explicit GUI(Service& service, QWidget* parent = nullptr);
	  ~GUI();
};

