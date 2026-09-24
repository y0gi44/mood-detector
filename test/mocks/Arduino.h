#ifndef TEST_ARDUINO_H
#define TEST_ARDUINO_H

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <string>

using byte = unsigned char;

inline bool isDigit(char value) {
    return value >= '0' && value <= '9';
}

class String {
public:
    String() = default;
    String(const char* value) : value_(value == nullptr ? "" : value) {}
    String(const std::string& value) : value_(value) {}
    String(char value) : value_(1, value) {}
    String(int value) : value_(std::to_string(value)) {}
    String(long value) : value_(std::to_string(value)) {}

    unsigned int length() const { return static_cast<unsigned int>(value_.length()); }
    char charAt(unsigned int index) const { return index < value_.length() ? value_[index] : '\0'; }
    long toInt() const {
        char* end = nullptr;
        const long result = std::strtol(value_.c_str(), &end, 10);
        return end == value_.c_str() ? 0 : result;
    }
    void toCharArray(char* destination, unsigned int size) const {
        if (size == 0) {
            return;
        }
        const unsigned int count = length() < size - 1 ? length() : size - 1;
        value_.copy(destination, count);
        destination[count] = '\0';
    }

    String& operator+=(const String& other) {
        value_ += other.value_;
        return *this;
    }

    const std::string& stdString() const { return value_; }

private:
    std::string value_;
};

inline String operator+(const String& left, const String& right) {
    return String(left.stdString() + right.stdString());
}

inline String operator+(const String& left, const char* right) {
    return String(left.stdString() + (right == nullptr ? "" : right));
}

inline String operator+(const char* left, const String& right) {
    return String((left == nullptr ? "" : left) + right.stdString());
}

#endif
