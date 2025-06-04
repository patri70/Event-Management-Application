#pragma once
#include <QAbstractTableModel>
#include <vector>
#include "Event.h"

class AdminEventsTable : public QAbstractTableModel
{
    Q_OBJECT
        std::vector<Event> events;
    bool detailed;

public:
    AdminEventsTable(const std::vector<Event>& events, bool detailed = true, QObject* parent = nullptr)
        : QAbstractTableModel(parent), events(events), detailed(detailed) {
    }

    void setEvents(const std::vector<Event>& newEvents) {
        beginResetModel();
        events = newEvents;
        endResetModel();
    }

    void setDetailed(bool isDetailed) {
        if (detailed != isDetailed) {
            beginResetModel();
            detailed = isDetailed;
            endResetModel();
        }
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return static_cast<int>(events.size());
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return detailed ? 5 : 2;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

        const Event& event = events[index.row()];
        if (detailed) {
            switch (index.column()) {
            case 0: return QString::fromStdString(event.getTitle());
            case 1: return QString::fromStdString(event.getDescription());
            case 2: return QString::fromStdString(event.getLink());
            case 3: return QString::fromStdString(event.getDate().toStringDate());
            case 4: return event.getNrPeople();
            default: return QVariant();
            }
        }
        else {
            switch (index.column()) {
            case 0: return QString::fromStdString(event.getTitle());
            case 1: return QString::fromStdString(event.getDate().toStringDate());
            default: return QVariant();
            }
        }
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
            return QVariant();
        if (detailed) {
            switch (section) {
            case 0: return "Title";
            case 1: return "Description";
            case 2: return "Link";
            case 3: return "Date";
            case 4: return "Number of people";
            default: return QVariant();
            }
        }
        else {
            switch (section) {
            case 0: return "Title";
            case 1: return "Date";
            default: return QVariant();
            }
        }
    }
};
