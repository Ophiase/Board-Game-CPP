#pragma once

#include <stdexcept>

template <typename T>
class Optional {
private:
    union Storage {
        T value;
        std::nullptr_t empty;
    };

    Storage storage{nullptr};
    bool _hasValue{false};

public:
    Optional() {};
    Optional(const T& val) : 
        storage{val}, _hasValue{true} {};
    ~Optional();
    Optional& operator=(const T& val);
    
    bool hasValue() const;
    const T& value() const;

    void reset();

    /*
        Only case where optional are not equal is when they both have
        a value, but thoses values are differents.
    */
    friend bool operator==(const Optional<T>& lhs, const Optional<T>& rhs) {
        return ! (
            lhs.hasValue() && rhs.hasValue() && (lhs.value() != rhs.value()) 
            ); 
    }
};