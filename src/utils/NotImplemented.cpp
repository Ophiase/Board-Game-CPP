#include "utils/NotImplemented.hpp"

void NotImplemented::operator()() const {
        throw std::runtime_error(message);
};