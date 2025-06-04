#include "GUI.h"
#include <QString>
#include "AdminMenuWindow.h"
#include "UserMenu.h"
#include <QFont>

GUI::GUI(Service& service, QWidget* parent) : QWidget(parent), service(service)
{
	setupUI();
}

GUI::~GUI()
{
}

void GUI::setupUI()
{
	this->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fbc2eb, stop:1 #a6c1ee);");

	auto* mainLayout = new QVBoxLayout(this);
	QLabel* titleLabel = new QLabel("Event Management System");
	QFont titleFont;
	titleFont.setPointSize(20);
	titleFont.setBold(true);
	titleLabel->setFont(titleFont);
	titleLabel->setAlignment(Qt::AlignCenter);
	mainLayout->addWidget(titleLabel);

	auto* buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch();

	adminButton = new QPushButton("Admin Menu", this);
	adminButton->setFixedSize(200, 55);
	connect(adminButton, &QPushButton::clicked, this, &GUI::adminMenu);

	QFont buttonFont = adminButton->font();
	buttonFont.setPointSize(16);
	buttonFont.setBold(true);
	adminButton->setFont(buttonFont);
	buttonLayout->addWidget(adminButton);

	buttonLayout->addSpacing(40);

	userButton = new QPushButton("User Menu", this);
	userButton->setFixedSize(200, 55);
	connect(userButton, &QPushButton::clicked, this, &GUI::userMenu);
	userButton->setFont(buttonFont);
	buttonLayout->addWidget(userButton);

	buttonLayout->addStretch();

	mainLayout->addSpacing(150);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addStretch();

	closeButton = new QPushButton("Close", this);
	closeButton->setFixedSize(100, 30);
	connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
	mainLayout->addWidget(closeButton, 0, Qt::AlignHCenter);
	setLayout(mainLayout);

}

void GUI::adminMenu()
{
	AdminMenuWindow* adminWindow = new AdminMenuWindow(service);
	adminWindow->show();
}

void GUI::userMenu()
{
	UserMenu* userWindow = new UserMenu(service);
	userWindow->show();
}