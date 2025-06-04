#include "ViewAdminEvents.h"
#include <QHeaderView>

ViewAdminEvents::ViewAdminEvents(Service& service, QWidget* parent)
    : QWidget(parent), service(service)
{
    this->resize(1400, 500);
    setWindowTitle("All events");

    auto* layout = new QVBoxLayout(this);

    eventsTable = new QTableView(this);
    model = new AdminEventsTable(service.getAllEvents(), true, this);
    eventsTable->setModel(model);
    eventsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(eventsTable);

    closeButton = new QPushButton("Close", this);
    closeButton->setFixedSize(100, 30);
    layout->addWidget(closeButton, 0, Qt::AlignHCenter);

    viewModeComboBox = new QComboBox(this);
    viewModeComboBox->addItem("Detailed");
    viewModeComboBox->addItem("Short");
    layout->addWidget(viewModeComboBox);

    connect(viewModeComboBox, &QComboBox::currentTextChanged, this, &ViewAdminEvents::onViewModeChanged);
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);

    completeTable();
}

void ViewAdminEvents::completeTable() {
    QString mode = viewModeComboBox->currentText();
    bool detailed = (mode == "Detailed");
    model->setDetailed(detailed);
    model->setEvents(service.getAllEvents());
}

void ViewAdminEvents::onViewModeChanged(const QString& mode) {
    Q_UNUSED(mode);
    completeTable();
}
