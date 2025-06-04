#include "Event.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>

DateAndTime::DateAndTime(int year, int month, int day, int hour, int minute)
{
	this->day = day;
	this->month = month;
	this->year = year;
	this->hour = hour;
	this->minute = minute;
}

Event::Event(const std::string& title, const std::string& description, const std::string& link, int nr_people, DateAndTime date)
{
	this->title = title;
	this->description = description;
	this->link = link;
	this->nr_people = nr_people;
	this->date = date;
}

Event::Event()
{
	this->title = "";
	this->description = "";
	this->link = "";
	this->nr_people = 0;
	this->date = DateAndTime(0, 0, 0, 0, 0);
}

void Event::play() const {
	ShellExecuteA(NULL, "open", this->getLink().c_str(), NULL, NULL, SW_SHOWNORMAL);
}

std::string Event::toString() const {
	const auto& d = this->date;
	std::string result = "Title: " + this->title +
		" | Description: " + this->description +
		" | Link: " + this->link +
		" | Number of people: " + std::to_string(this->nr_people) +
		" | Date: " + std::to_string(d.getDay()) + "." +
		std::to_string(d.getMonth()) + "." + std::to_string(d.getYear()) +
		" " + std::to_string(d.getHour()) + ":" + 
		(d.getMinute() < 10 ? "0" : "") + std::to_string(d.getMinute());
    return result;
}

std::ostream& operator<<(std::ostream& out, const DateAndTime& dt) {
	out << dt.getYear() << " " << dt.getMonth() << " "  << dt.getDay() << " "
		<< dt.getHour() << " " << dt.getMinute();
	return out;
}

std::istream& operator>>(std::istream& in, DateAndTime& dt) {
	int y, m, d, h, min;
	in >> y >> m >> d >> h >> min;
	dt = DateAndTime(y, m, d, h, min);
	return in;
}

std::ostream& operator<<(std::ostream& out, const Event& e) {
	out << e.getTitle() << "|"
		<< e.getDescription() << "|" 
		<< e.getLink() <<  "|" 
		<< e.getNrPeople() <<  "|" 
		<< e.getDate() << '\n';
	return out;
}

std::istream& operator>>(std::istream& in, Event& e) {
	std::string line;
	std::getline(in, line);

	std::istringstream iss(line);
	std::string title, description, link;
	int nr_people;
	DateAndTime date;

	std::getline(iss, title, '|');
	std::getline(iss, description, '|');
	std::getline(iss, link, '|');
	iss >> nr_people;
	iss.ignore(1, '|'); // Ignore the delimiter before the date
	iss >> date;

	e = Event(title, description, link, nr_people, date);
	return in;
}

std::string Event::toCSV() const {
	std::stringstream ss;
	ss << "\"" << title << "\";"
		<< "\"" << description << "\";"
		<< "\"" << link << "\";"
		<< nr_people << ";"
		<< date.getDay() << "/" << date.getMonth() << "/" << date.getYear() << " "
		<< date.getHour() << ":" << (date.getMinute() < 10 ? "0" : "") << date.getMinute();
	return ss.str();
}

std::string Event::toHTMLRow() const {
	std::stringstream ss;
	ss << "<tr>"
		<< "<td>" << title << "</td>"
		<< "<td>" << description << "</td>"
		<< "<td><a href='" << link << "'>" << link << "</a></td>"
		<< "<td>" << nr_people << "</td>"
		<< "<td>" << date.getDay() << "." << date.getMonth() << "." << date.getYear()
		<< " " << date.getHour() << ":" << (date.getMinute() < 10 ? "0" : "") << date.getMinute()
		<< "</td>"
		<< "</tr>";
	return ss.str();
}