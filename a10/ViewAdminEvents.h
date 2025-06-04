#pragma once
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include "Service.h"
#include "ui_GUI.h"
#include "AdminEventsTable.h"

class ViewAdminEvents : public QWidget
{
	Q_OBJECT

private:
	QComboBox* viewModeComboBox;
	Service& service;
	QTableView* eventsTable;
	QPushButton* closeButton;
	AdminEventsTable* model;

	void completeTable();
	void onViewModeChanged(const QString& mode);

public:
	explicit ViewAdminEvents(Service& service, QWidget* parent = nullptr);
	~ViewAdminEvents() override = default;
};

