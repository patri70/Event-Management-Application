#pragma once
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class RepositoryException : public std::runtime_error {
public:
    explicit RepositoryException(const std::string& message) : std::runtime_error(message) {}
};

class ValidationException : public std::runtime_error {
public:
    explicit ValidationException(const std::string& message) : std::runtime_error(message) {}
};

#endif

