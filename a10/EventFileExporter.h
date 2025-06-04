#pragma once
#include "Event.h"
#include <vector>
#include <string>


class EventFileExporter
{
protected:
	std::string filename;
    std::vector<Event> events;
public:
    EventFileExporter(const std::string& filename);
    virtual ~EventFileExporter() {};

    virtual void writeToFile() = 0;
    virtual void displayFile() = 0;

    void setEvents(const std::vector<Event>& events) {
        this->events = events;
    }

};

class CSVEventExporter : public EventFileExporter {
public:
    CSVEventExporter(const std::string& filename);
    void writeToFile() override;
    void displayFile() override;
};

class HTMLEventExporter : public EventFileExporter {
public:
    HTMLEventExporter(const std::string& filename);
    void writeToFile() override;
    void displayFile() override;
};
