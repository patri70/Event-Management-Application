#pragma once
#include <string>

class DateAndTime
{
private:
	int day, month, year, hour, minute;

public:
	DateAndTime(int year = 0, int month = 0, int day = 0, int hour = 0, int minute = 0);


	int getYear() const noexcept { return this->year; }
	int getMonth() const noexcept { return this->month; }
	int getDay() const noexcept { return this->day; }
	int getHour() const noexcept { return this->hour; }
	int getMinute() const noexcept { return this->minute; }

	bool operator==(const DateAndTime& other) const {
		return this->day == other.day &&
			this->month == other.month &&
			this->year == other.year &&
			this->hour == other.hour &&
			this->minute == other.minute;
	}

	bool operator>(const DateAndTime& other) const {
		if (this->year > other.year) return true;
		if (this->year == other.year && this->month > other.month) return true;
		if (this->year == other.year && this->month == other.month && this->day > other.day) return true;
		if (this->year == other.year && this->month == other.month && this->day == other.day && this->hour > other.hour) return true;
		if (this->year == other.year && this->month == other.month && this->day == other.day && this->hour == other.hour && this->minute > other.minute) return true;
		return false;
	}

	std::string toStringDate() const {
		char buffer[20];
		snprintf(buffer, sizeof(buffer), "%02d.%02d.%04d %02d:%02d",
			day, month, year, hour, minute);
		return std::string(buffer);
	}
};

class Event
{
private:
	std::string title;
	std::string description;
	std::string link;
	int nr_people;
	DateAndTime date;

public:
	Event(const std::string& title,
		const std::string& description,
		const std::string& link,
		int nr_people,
		DateAndTime date);

	std::string getTitle() const { return this->title; }
	std::string getDescription() const { return this->description; }
	std::string getLink() const { return this->link; }
	int getNrPeople() const noexcept { return this->nr_people; }
	DateAndTime getDate() const noexcept { return this->date; }
	Event();
	void play() const;

	bool operator==(const Event& other) const {
		return this->title == other.title &&
			this->description == other.description &&
			this->link == other.link &&
			this->nr_people == other.nr_people &&
			this->date == other.date;
	}

	void incrementNrPeople() { this->nr_people++; }
	void decrementNrPeople() { this->nr_people--; }

	std::string toString() const;

	friend std::ostream& operator<<(std::ostream& out, const Event& event);
	friend std::istream& operator>>(std::istream& in, Event& event);

	std::string toCSV() const;
	std::string toHTMLRow() const;
};

