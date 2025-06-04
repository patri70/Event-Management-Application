#pragma once
#include "Event.h"
#include <vector>


class Repository
{
private:
	std::vector<Event> events;
	std::vector<Event> userEvents;

	std::string filename;

	void loadFromFile();
	void saveToFile() const;

public:
	Repository(const std::string& filename); 

	bool addEvent(const Event& event);
	bool removeEvent(const Event& event);
	bool updateEvent(const std::string& eventTitle, const Event& newEvent);
	

	std::vector<Event> getEventsByMonth(int month) const;
	std::vector<Event> getAllEvents() const;

	bool addUserEvent(const Event& event);
	bool removeUserEvent(const Event& event);
	std::vector<Event> getUserEvents() const;
};

