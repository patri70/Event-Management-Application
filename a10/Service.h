#pragma once
#include "Repository.h"

class Service
{
private:
    Repository repo;

public:
    Service() = default;
    explicit Service(const Repository& repo) : repo(repo) {}

    bool addEvent(const Event& event);
    bool removeEvent(const Event& event);
    bool updateEvent(const std::string& eventTitle, const Event& newEvent);
   // int GetEventPosition(const Event& event);
    std::vector<Event> getAllEvents() const;

    std::vector<Event> getEventsByMonth(int month) const;
    bool addUserEvent(const Event& event);
    bool removeUserEvent(const Event& event);
    std::vector<Event> getUserEvents() const;
};

