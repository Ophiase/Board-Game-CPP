#include "utils/Optional.hpp"

template <typename T>
Optional<T>& Optional<T>::operator=(const T& val) {
    if (hasValue) {
        storage.value = val;
    } else {
        new (&storage.value) T(val);
        _hasValue = true;
    }

    return *this;
}

template <typename T>
bool Optional<T>::hasValue() const {
    return _hasValue;
}

template <typename T>
const T& Optional<T>::value() const {
    if (!hasValue) {
        throw std::runtime_error("Optional does not contain a value");
    }
    return storage.value;
}

template <typename T>
void Optional<T>::reset() {
    if (_hasValue) {
        storage.empty = nullptr;
        _hasValue = false;
    }
}