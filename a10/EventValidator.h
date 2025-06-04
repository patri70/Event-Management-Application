#pragma once
#ifndef EVENTVALIDATOR_H
#define EVENTVALIDATOR_H

#include "Event.h"
#include "Exception.h"

class EventValidator {
public:
    static void validate(const Event& event) {
        if (event.getTitle().empty()) {
            throw ValidationException("Event title cannot be empty.");
        }
        if (event.getDescription().empty()) {
            throw ValidationException("Event description cannot be empty.");
        }
        if (event.getLink().empty()) {
            throw ValidationException("Event link cannot be empty.");
        }
        if (event.getNrPeople() < 0) {
            throw ValidationException("Number of people cannot be negative.");
        }
        
    }
};

#endif 