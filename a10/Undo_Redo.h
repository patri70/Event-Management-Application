#pragma once
#include "Event.h"
#include "Service.h"
#include <memory>
#include <stack>

class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;
};

class AddCommand : public Command {
private:
    Event event;
    Service& service;

public:
    AddCommand(Service& service, const Event& event);
    void execute() override;
    void undo() override;
};

class RemoveCommand : public Command {
private:
    Event removedEvent;
    Service& service;

public:
    RemoveCommand(Service& service, const Event& event);
    void execute() override;
    void undo() override;
};

class UpdateCommand : public Command {
private:
    Event oldEvent, newEvent;
    Service& service;

public:
    UpdateCommand(Service& service, const Event& oldEvent, const Event& newEvent);
    void execute() override;
    void undo() override;
};

class CommandManager {
private:
    std::stack<std::unique_ptr<Command>> undoStack;
    std::stack<std::unique_ptr<Command>> redoStack;

public:
    bool executeCommand(std::unique_ptr<Command> cmd);
    void undo();
    void redo();
};

class AddUserCommand : public Command {
private:
    Event event;
    Service& service;

public:
    AddUserCommand(Service& service, const Event& event);
    void execute() override;
    void undo() override;
};

class RemoveUserCommand : public Command {
private:
    Event removedEvent;
    Service& service;

public:
    RemoveUserCommand(Service& service, const Event& event);
    void execute() override;
    void undo() override;
};

