#include "UserMenu.h"
#include "ShowEventsByMonth.h"
#include "EventFileExporter.h"

UserMenu::UserMenu(Service& service, QWidget* parent) : QWidget(parent), service(service)
{
	setWindowTitle("User Menu");
	this->resize(800, 600);
	setUserMenu();
}

void UserMenu::setUserMenu()
{
	auto* layoutUser = new QVBoxLayout(this);

	label = new QLabel("User Menu", this);
	QFont font = label->font();
	font.setPointSize(22);
	font.setBold(true);
	label->setFont(font);
	label->setAlignment(Qt::AlignCenter);
	layoutUser->addSpacing(40);
	layoutUser->addWidget(label);
	layoutUser->addSpacing(60);

	viewByMonthButton = new QPushButton("View Events by Month", this);
	removeEventButton = new QPushButton("Remove Event", this);
	viewParticipatingEventsButton = new QPushButton("View Participating Events", this);
	saveEventsButton = new QPushButton("Save Events to File", this);
	displayFileEventsButton = new QPushButton("Display Saved Events List", this);
	closeButton = new QPushButton("Close", this);
	undoButton = new QPushButton("Undo", this);
	redoButton = new QPushButton("Redo", this);

	QFont buttonFont = viewByMonthButton->font();
	buttonFont.setBold(true);
	buttonFont.setPointSize(14);
	viewByMonthButton->setFont(buttonFont);
	removeEventButton->setFont(buttonFont);
	viewParticipatingEventsButton->setFont(buttonFont);
	saveEventsButton->setFont(buttonFont);
	displayFileEventsButton->setFont(buttonFont);
	closeButton->setFont(buttonFont);
	undoButton->setFont(buttonFont);
	redoButton->setFont(buttonFont);

	QSize buttonSize(400, 80);
	viewByMonthButton->setFixedSize(buttonSize);
	removeEventButton->setFixedSize(buttonSize);
	viewParticipatingEventsButton->setFixedSize(buttonSize);
	saveEventsButton->setFixedSize(buttonSize);
	displayFileEventsButton->setFixedSize(buttonSize);
	closeButton->setFixedSize(buttonSize);

	auto* buttonLayout = new QVBoxLayout();
	buttonLayout->setAlignment(Qt::AlignCenter);
	buttonLayout->setSpacing(20);
	buttonLayout->addWidget(viewByMonthButton);
	buttonLayout->addWidget(removeEventButton);
	buttonLayout->addWidget(viewParticipatingEventsButton);
	buttonLayout->addWidget(saveEventsButton);
	buttonLayout->addWidget(displayFileEventsButton);
	buttonLayout->setAlignment(Qt::AlignCenter);
	layoutUser->addLayout(buttonLayout);
	layoutUser->addSpacing(30);
	layoutUser->addStretch();

	undoButton->setFixedSize(100, 30);
	redoButton->setFixedSize(100, 30);
	closeButton->setFixedSize(100, 30);
	layoutUser->addWidget(undoButton, 0, Qt::AlignLeft);
	layoutUser->addWidget(redoButton, 0, Qt::AlignRight);
	layoutUser->addWidget(closeButton, 0, Qt::AlignHCenter);

	connect(undoButton, &QPushButton::clicked, this, &UserMenu::undo);
	connect(redoButton, &QPushButton::clicked, this, &UserMenu::redo);
	connect(viewByMonthButton, &QPushButton::clicked, this, &UserMenu::showEventsMonth);
	connect(removeEventButton, &QPushButton::clicked, this, &UserMenu::removeEventUser);
	connect(viewParticipatingEventsButton, &QPushButton::clicked, this, &UserMenu::listEventsUser);
	connect(saveEventsButton, &QPushButton::clicked, this, &UserMenu::saveEventsToFile);
	connect(displayFileEventsButton, &QPushButton::clicked, this, &UserMenu::displayEventsToFile);
	connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
	setLayout(layoutUser);

	QShortcut* undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
	QShortcut* redoShortcut = new QShortcut(QKeySequence("Ctrl+Y"), this);
	connect(undoShortcut, &QShortcut::activated, this, &UserMenu::undo);
	connect(redoShortcut, &QShortcut::activated, this, &UserMenu::redo);
}

void UserMenu::showEventsMonth()
{
	auto* showEvents = new ShowEventsByMonth(service, commandManager);
	showEvents->show();
}

void UserMenu::listEventsUser()
{
	auto userEvents = service.getUserEvents();
	if (userEvents.empty()) {
		QMessageBox::information(this, "Participating Events", "You are not participating in any events.");
		return;
	}

	QString eventsText;
	for (const auto& event : userEvents) {
		eventsText += QString::fromStdString(event.toString()) + "\n\n";
	}

	QMessageBox::information(this, "Participating Events", eventsText);
}

void UserMenu::removeEventUser()
{
	auto userEvents = service.getUserEvents();
	if (userEvents.empty()) {
		QMessageBox::information(this, "Remove Event", "You are not participating in any events.");
		return;
	}

	QStringList eventTitles;
	for (const auto& event : userEvents) {
		eventTitles << QString::fromStdString(event.getTitle());
	}

	bool ok;
	QString selectedEventTitle = QInputDialog::getItem(this, "Remove Event", "Select an event to remove:", eventTitles, 0, false, &ok);
	if (ok && !selectedEventTitle.isEmpty()) {
		
		auto it = std::find_if(userEvents.begin(), userEvents.end(), [&](const Event& event) {
			return event.getTitle() == selectedEventTitle.toStdString();
			});

		if (it != userEvents.end()) {
			commandManager.executeCommand(std::make_unique<RemoveUserCommand>(service, *it));
			QMessageBox::information(this, "Remove Event", "Event removed successfully.");
		}
		else {
			QMessageBox::warning(this, "Remove Event", "Selected event not found.");
		}
	}
}

void UserMenu::saveEventsToFile()
{
	QDialog dialog(this);
	QVBoxLayout layout(&dialog);

	QLabel formatLabel("Select export format:");
	QComboBox formatCombo;
	formatCombo.addItem("CSV (.csv)", "csv");
	formatCombo.addItem("HTML (.html)", "html");
	layout.addWidget(&formatLabel);
	layout.addWidget(&formatCombo);

	QLabel fileLabel("Filename (without extension):");
	QLineEdit filenameEdit;
	layout.addWidget(&fileLabel);
	layout.addWidget(&filenameEdit);

	QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	layout.addWidget(&buttons);

	QObject::connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	QObject::connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	if (dialog.exec() != QDialog::Accepted) {
		return;
	}

	
	QString filename = filenameEdit.text().trimmed();
	if (filename.isEmpty()) {
		QMessageBox::warning(this, "Error", "Filename cannot be empty!");
		return;
	}

	QString extension = formatCombo.currentData().toString();
	QString fullPath = filename + "." + extension;

	try {
		
		auto events = service.getAllEvents();

		if (events.empty()) {
			QMessageBox::information(this, "Export", "No events to export.");
			return;
		}

		std::unique_ptr<EventFileExporter> exporter;
		if (extension == "csv") {
			exporter = std::make_unique<CSVEventExporter>(fullPath.toStdString());
		}
		else {
			exporter = std::make_unique<HTMLEventExporter>(fullPath.toStdString());
		}

		exporter->setEvents(events);
		exporter->writeToFile();

		QMessageBox::information(this, "Success",
			QString("Events exported successfully to:\n%1").arg(fullPath));

		QMessageBox::StandardButton openFile = QMessageBox::question(this, "Open File",
			"Do you want to open the exported file?",
			QMessageBox::Yes | QMessageBox::No);

		if (openFile == QMessageBox::Yes) {
			exporter->displayFile();
		}
	}
	catch (const std::exception& e) {
		QMessageBox::critical(this, "Error",
			QString("Failed to export events:\n%1").arg(e.what()));
	}
}

void UserMenu::displayEventsToFile()
{
	QDialog dialog(this);
	QVBoxLayout layout(&dialog);

	QLabel formatLabel("Select file format:");
	QComboBox formatCombo;
	formatCombo.addItem("CSV (.csv)", "csv");
	formatCombo.addItem("HTML (.html)", "html");
	layout.addWidget(&formatLabel);
	layout.addWidget(&formatCombo);

	QLabel fileLabel("Filename (without extension):");
	QLineEdit filenameEdit;
	layout.addWidget(&fileLabel);
	layout.addWidget(&filenameEdit);

	QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	layout.addWidget(&buttons);

	QObject::connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	QObject::connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	if (dialog.exec() != QDialog::Accepted) {
		return;
	}

	QString filename = filenameEdit.text().trimmed();
	if (filename.isEmpty()) {
		QMessageBox::warning(this, "Error", "Filename cannot be empty!");
		return;
	}

	QString extension = formatCombo.currentData().toString();
	QString fullPath = filename + "." + extension;

	try {
		if (!QFile::exists(fullPath)) {
			QMessageBox::warning(this, "Error",
				QString("File not found:\n%1").arg(fullPath));
			return;
		}

		std::unique_ptr<EventFileExporter> exporter;
		if (extension == "csv") {
			exporter = std::make_unique<CSVEventExporter>(fullPath.toStdString());
		}
		else {
			exporter = std::make_unique<HTMLEventExporter>(fullPath.toStdString());
		}

		exporter->displayFile();
	}
	catch (const std::exception& e) {
		QMessageBox::critical(this, "Error",
			QString("Failed to open file:\n%1").arg(e.what()));
	}
}

void UserMenu::undo()
{
	commandManager.undo();
}

void UserMenu::redo()
{
	commandManager.redo();
}