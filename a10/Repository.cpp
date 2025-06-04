#include "Repository.h"
#include "EventValidator.h"
#include "Exception.h"
#include <algorithm>
#include <iterator>
#include <fstream>
#include <stdexcept>
#include <iostream>

Repository::Repository(const std::string& filename) : filename(filename) {
	loadFromFile();
}

void Repository::loadFromFile() {
	std::ifstream inFile(filename);
	if (!inFile.is_open()) {
		std::ofstream outFile(filename);
		outFile.close();
		return;
	}

	Event event;
	while (inFile >> event) {
		events.push_back(event);
	}

	inFile.close();
}

void Repository::saveToFile() const {
	std::ofstream outFile(filename);
	if (!outFile.is_open()) {
		throw std::runtime_error("Could not open file for writing: " + filename);
	}

	for (const auto& event : events) {
		outFile << event;
	}
	outFile.close();
}

bool Repository::addEvent(const Event& event) {
	EventValidator::validate(event);

	if (std::find(events.begin(), events.end(), event) != events.end())
		throw std::runtime_error("Event already exists.");
	events.push_back(event);
	saveToFile();
	return true;
}

bool Repository::removeEvent(const Event& event) {
	auto it = std::find(events.begin(), events.end(), event);
	if (it != events.end()) {
		events.erase(it);  
		saveToFile();      
		return true;
	}
	throw std::runtime_error("Event not found.");
}

bool Repository::updateEvent(const std::string& eventTitle, const Event& newEvent)
{
	EventValidator::validate(newEvent);

	auto found = std::find_if(events.begin(), events.end(), [&](const Event& e) { return e.getTitle() == eventTitle; });
	if (found != events.end()) {
		*found = newEvent;
		saveToFile();
		return true;
	}
	throw std::runtime_error("Old event not found.");
}


std::vector<Event> Repository::getAllEvents() const
{
	return events;
}

std::vector<Event> Repository::getEventsByMonth(int month) const
{
	std::vector<Event> filteredEvents;
	
	std::copy_if(events.begin(), events.end(), std::back_inserter(filteredEvents),
		[month](const Event& event) {
			return event.getDate().getMonth() == month;
		});

	std::sort(filteredEvents.begin(), filteredEvents.end(),
		[](const Event& event1, const Event& event2) {
			return event2.getDate() > event1.getDate();
		});

	return filteredEvents;
}

bool Repository::addUserEvent(const Event& event)
{
	EventValidator::validate(event);
	auto found = std::find(events.begin(), events.end(), event);
	if (found != events.end()) {
		found->incrementNrPeople(); 
		userEvents.push_back(*found);
		saveToFile();
		return true;
	}
	return false;
}

bool Repository::removeUserEvent(const Event& event)
{
	auto found = std::find(userEvents.begin(), userEvents.end(), event);
	if (found != userEvents.end()) {
		auto mainEvent = std::find(events.begin(), events.end(), event);
		if (mainEvent != events.end()) {
			mainEvent->decrementNrPeople();
		}
		userEvents.erase(found);
		saveToFile();
		return true;
	}
	throw std::runtime_error("Event not found.");
}

std::vector<Event> Repository::getUserEvents() const
{
	return userEvents;
}



