#include "Undo_Redo.h"

AddCommand::AddCommand(Service& service, const Event& event)
    : service(service), event(event) {
}

void AddCommand::execute() {
    service.addEvent(event);
}

void AddCommand::undo() {
    service.removeEvent(event);
}

RemoveCommand::RemoveCommand(Service& service, const Event& event)
    : service(service), removedEvent(event) {
}

void RemoveCommand::execute() {
    service.removeEvent(removedEvent);
}

void RemoveCommand::undo() {
    service.addEvent(removedEvent);
}

AddUserCommand::AddUserCommand(Service& service, const Event& event)
    : service(service), event(event) {
}

void AddUserCommand::execute() {
    service.addUserEvent(event);
}

void AddUserCommand::undo() {
    service.removeUserEvent(event);
}

RemoveUserCommand::RemoveUserCommand(Service& service, const Event& event)
    : service(service), removedEvent(event) {
}

void RemoveUserCommand::execute() {
    service.removeUserEvent(removedEvent);
}

void RemoveUserCommand::undo() {
    service.addUserEvent(removedEvent);
}

UpdateCommand::UpdateCommand(Service& service, const Event& oldEvent, const Event& newEvent)
    : service(service), oldEvent(oldEvent), newEvent(newEvent) {
}

void UpdateCommand::execute() {
    service.updateEvent(oldEvent.getTitle(), newEvent);
}

void UpdateCommand::undo() {
    service.updateEvent(newEvent.getTitle(), oldEvent);
}

bool CommandManager::executeCommand(std::unique_ptr<Command> cmd) {
    cmd->execute();
    undoStack.push(std::move(cmd));
    while (!redoStack.empty()) redoStack.pop();
    return true;
}

void CommandManager::undo() {
    if (!undoStack.empty()) {
        auto cmd = std::move(undoStack.top());
        undoStack.pop();
        cmd->undo();
        redoStack.push(std::move(cmd));
    }
}

void CommandManager::redo() {
    if (!redoStack.empty()) {
        auto cmd = std::move(redoStack.top());
        redoStack.pop();
        cmd->execute();
        undoStack.push(std::move(cmd));
    }
}

