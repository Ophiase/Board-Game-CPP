#include "engine/Player.hpp"

bool Player::operator==(PlayerId otherId) const {
    return this->id == otherId;
}

bool Player::operator==(Player other) const {
    return 
        (this->id == other.id) &&
        (this->name == other.name) &&
        (this->isAI == other.isAI);
}

bool Player::operator!=(PlayerId otherId) const {
    return !((*this) == otherId);
}

bool Player::operator!=(Player other) const {
    return !((*this) == other);
}
