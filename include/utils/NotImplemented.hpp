#pragma once

#include <string>
#include <stdexcept>

class NotImplemented {
public:
    NotImplemented(const std::string& message = "Function not implemented") : message(message) {}
    void operator()() const;
private:
    std::string message;
};