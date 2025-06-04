#include "Service.h"

bool Service::addEvent(const Event& event)
{
	return this->repo.addEvent(event);
}

bool Service::removeEvent(const Event& event)
{
	return this->repo.removeEvent(event);
}

bool Service::updateEvent(const std::string& eventTitle, const Event& newEvent)
{
	return this->repo.updateEvent(eventTitle, newEvent);
}

std::vector<Event> Service::getAllEvents() const
{
	return this->repo.getAllEvents();
}

std::vector<Event> Service::getEventsByMonth(int month) const
{
	return this->repo.getEventsByMonth(month);
}

bool Service::addUserEvent(const Event& event)
{
	return this->repo.addUserEvent(event);
}

bool Service::removeUserEvent(const Event& event)
{
	return this->repo.removeUserEvent(event);
}

std::vector<Event> Service::getUserEvents() const
{
	return this->repo.getUserEvents();
}