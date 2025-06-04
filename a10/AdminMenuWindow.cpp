#include "AdminMenuWindow.h"
#include "ViewAdminEvents.h"


AdminMenuWindow::AdminMenuWindow(Service& service, QWidget* parent) : QWidget(parent), service(service)
{
	setWindowTitle("Administrator");
	this->resize(800, 600);	
	setAdmin();
}

void AdminMenuWindow::setAdmin()
{
	auto* layoutAdmin = new QVBoxLayout(this);

	label = new QLabel("Administrator Menu", this);
	QFont font = label->font();
	font.setPointSize(20);
	font.setBold(true);
	label->setFont(font);
	label->setAlignment(Qt::AlignCenter);
	layoutAdmin->addSpacing(40);
	layoutAdmin->addWidget(label);
	layoutAdmin->addSpacing(60);
	addButton = new QPushButton("Add Event", this);
	removeButton = new QPushButton("Remove Event", this);
	updateButton = new QPushButton("Update Event", this);
	listButton = new QPushButton("List Events", this);
	closeButton = new QPushButton("Close", this);
	undoButton = new QPushButton("Undo", this);
	redoButton = new QPushButton("Redo", this);

	QSize buttonSize(300, 80);
	addButton->setFixedSize(buttonSize);
	removeButton->setFixedSize(buttonSize);
	updateButton->setFixedSize(buttonSize);
	listButton->setFixedSize(buttonSize);

	QFont buttonFont = addButton->font();
	buttonFont.setBold(true);
	buttonFont.setPointSize(14);
	addButton->setFont(buttonFont);
	removeButton->setFont(buttonFont);
	updateButton->setFont(buttonFont);
	listButton->setFont(buttonFont);
	closeButton->setFont(buttonFont);
	undoButton->setFont(buttonFont);
	redoButton->setFont(buttonFont);

	auto* buttonLayout = new QVBoxLayout();
	buttonLayout->setAlignment(Qt::AlignCenter);
	buttonLayout->setSpacing(20);
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(removeButton);
	buttonLayout->addWidget(updateButton);
	buttonLayout->addWidget(listButton);
	buttonLayout->setAlignment(Qt::AlignCenter);
	layoutAdmin->addLayout(buttonLayout);
	layoutAdmin->addSpacing(50);
	layoutAdmin->addStretch();

	undoButton->setFixedSize(100,30);
	redoButton->setFixedSize(100,30);
	closeButton->setFixedSize(100, 30);
	layoutAdmin->addWidget(undoButton, 0, Qt::AlignLeft);
	layoutAdmin->addWidget(redoButton, 0, Qt::AlignRight);
	layoutAdmin->addWidget(closeButton, 0, Qt::AlignHCenter);

	connect(listButton, &QPushButton::clicked, this, &AdminMenuWindow::viewEvents);
	connect(addButton, &QPushButton::clicked, this, &AdminMenuWindow::addEvent);
	connect(updateButton, &QPushButton::clicked, this, &AdminMenuWindow::updateEventAdmin);
	connect(removeButton, &QPushButton::clicked, this, &AdminMenuWindow::removeEventAdmin);
	connect(undoButton, &QPushButton::clicked, this, &AdminMenuWindow::undo);
	connect(redoButton, &QPushButton::clicked, this, &AdminMenuWindow::redo);
	connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
	setLayout(layoutAdmin);

	QShortcut* undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
	QShortcut* redoShortcut = new QShortcut(QKeySequence("Ctrl+Y"), this);
	connect(undoShortcut, &QShortcut::activated, this, &AdminMenuWindow::undo);
	connect(redoShortcut, &QShortcut::activated, this, &AdminMenuWindow::redo);

}

void AdminMenuWindow::viewEvents()
{
	auto* viewAdminEvents = new ViewAdminEvents(service);
	viewAdminEvents->show();
}

void AdminMenuWindow::addEvent()
{
	auto* addEventWindow = new AddEvent(service, commandManager);
	addEventWindow->show();
}

void AdminMenuWindow::removeEventAdmin()
{
	auto allEvents = service.getAllEvents();
	if (allEvents.empty()) {
		QMessageBox::information(this, "Remove Event", "There are no events to remove.");
		return;
	}

	QStringList eventTitles;
	for (const auto& event : allEvents) {
		eventTitles << QString::fromStdString(event.getTitle());
	}

	bool ok;
	QString selectedEventTitle = QInputDialog::getItem(this, "Remove Event", "Select an event to remove:", eventTitles, 0, false, &ok);
	if (ok && !selectedEventTitle.isEmpty()) {
		auto it = std::find_if(allEvents.begin(), allEvents.end(), [&](const Event& event) {
			return event.getTitle() == selectedEventTitle.toStdString();
			});

		if (it != allEvents.end()) {
			commandManager.executeCommand(std::make_unique<RemoveCommand>(service, *it));
			QMessageBox::information(this, "Remove Event", "Event removed successfully.");
		}
		else {
			QMessageBox::warning(this, "Remove Event", "Selected event not found.");
		}
	}
}

void AdminMenuWindow::updateEventAdmin()
{
	auto allEvents = service.getAllEvents();
	if (allEvents.empty()) {
		QMessageBox::information(this, "Update Event", "No events found.");
		return;
	}

	QStringList titles;
	for (const auto& e : allEvents) {
		titles << QString::fromStdString(e.getTitle());
	}

	bool ok;
	QString oldTitle = QInputDialog::getItem(
		this, "Update Event", "Select event:", titles, 0, false, &ok
	);

	if (!ok || oldTitle.isEmpty()) return; 

	
	auto it = std::find_if(allEvents.begin(), allEvents.end(),
		[&](const Event& e) { return e.getTitle() == oldTitle.toStdString(); });

	if (it == allEvents.end()) {
		QMessageBox::warning(this, "Error", "Event not found.");
		return;
	}

	QString newTitle = QInputDialog::getText(
		this, "Update Event", "New Title:", QLineEdit::Normal,
		QString::fromStdString(it->getTitle()), &ok
	);

	if (!ok) return; 

	QString newDescription = QInputDialog::getText(
		this, "Update Event", "New Description:", QLineEdit::Normal,
		QString::fromStdString(it->getDescription()), &ok
	);

	QString newLink = QInputDialog::getText(
		this, "Update Event", "New Link:", QLineEdit::Normal,
		QString::fromStdString(it->getLink()), &ok
	);

	QString newNrPeople = QInputDialog::getText(
		this, "Update Event", "New Number Of People:", QLineEdit::Normal,
		QString::fromStdString(std::to_string(it->getNrPeople())), &ok
	);

	QString newDate = QInputDialog::getText(
		this, "Update Event", "New Date (dd.MM.yyyy HH:mm):", QLineEdit::Normal,
		QString::fromStdString(it->getDate().toStringDate()), &ok
	);


	if (!ok) return;

	int nrPeople = newNrPeople.toInt(&ok);
	if (!ok) {
		QMessageBox::warning(this, "Error", "Invalid number of people.");
		return;
	}

	
	QDateTime dateTime = QDateTime::fromString(newDate, "dd.MM.yyyy HH:mm");
	if (!dateTime.isValid()) {
		QMessageBox::warning(this, "Error", "Invalid date/time format. Please use dd.MM.yyyy HH:mm");
		return;
	}

	DateAndTime date(
		dateTime.date().year(),
		dateTime.date().month(),
		dateTime.date().day(),
		dateTime.time().hour(),
		dateTime.time().minute()
	);
	
	
	Event updatedEvent(
		newTitle.toStdString(),  
		newDescription.toStdString(),
		newLink.toStdString(),   
		nrPeople,  
		date  
	);

	
	try {
		commandManager.executeCommand(std::make_unique<UpdateCommand>(service, *it, updatedEvent));
		QMessageBox::information(this, "Success", "Event updated!");
	}
	catch (const std::exception& e) {
		QMessageBox::critical(this, "Error", e.what());
	}
}

void AdminMenuWindow::undo() {
	commandManager.undo();
}

void AdminMenuWindow::redo() {
	commandManager.redo();
}
