#ifndef CRYPTOMATH_H
#define CRYPTOMATH_H
#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>

using std::cout;
using std::string;
using std::endl;

class LargeNumber {
private:
    string value;

public:
    LargeNumber() : value("0") {}

    LargeNumber(const string& val) : value(val) {}

    void Show() const {
        cout << value;
    }

    bool isLessThan(const string& str1, const string& str2) const {
        if (str1.length() < str2.length()) return true;
        if (str1.length() > str2.length()) return false;
        return str1 < str2;
    }

    LargeNumber operator+(const LargeNumber& other) const {
        string result;
        int carry = 0;
        string first = value;
        string second = other.value;
        std::reverse(first.begin(), first.end());
        std::reverse(second.begin(), second.end());

        int maxLength = std::max(first.size(), second.size());
        first.resize(maxLength, '0');
        second.resize(maxLength, '0');

        for (int i = 0; i < maxLength; i++) {
            int digitSum = (first[i] - '0') + (second[i] - '0') + carry;
            carry = digitSum / 10;
            result.push_back((digitSum % 10) + '0');
        }

        if (carry) {
            result.push_back(carry + '0');
        }
        std::reverse(result.begin(), result.end());

        return LargeNumber(result);
    }

    string Subtract(const string& str1, const string& str2) const {
        string first = str1;
        string second = str2;
        string result;

        bool isNegative = false;

        if (isLessThan(first, second)) {
            string temp = first;
            first = second;
            second = temp;
            isNegative = true;
        }

        std::reverse(first.begin(), first.end());
        std::reverse(second.begin(), second.end());

        second.resize(first.size(), '0');

        int borrow = 0;
        for (size_t i = 0; i < first.size(); i++) {
            int sub = (first[i] - '0') - (second[i] - '0') - borrow;
            if (sub < 0) {
                sub += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.push_back(sub + '0');
        }

        while (result.size() > 1 && result.back() == '0') {
            result.pop_back();
        }

        std::reverse(result.begin(), result.end());

        if (isNegative) {
            result = "-" + result;
        }

        return result.empty() ? "0" : result;
    }

    LargeNumber operator-(const LargeNumber& other) const {
        string first = value;
        string second = other.value;

        if (isLessThan(first, second)) {
            return LargeNumber("0");
        }

        string result;
        int borrow = 0;

        std::reverse(first.begin(), first.end());
        std::reverse(second.begin(), second.end());

        second.resize(first.size(), '0');

        for (size_t i = 0; i < first.size(); i++) {
            int sub = (first[i] - '0') - (second[i] - '0') - borrow;
            if (sub < 0) {
                sub += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.push_back(sub + '0');
        }

        while (result.size() > 1 && result.back() == '0') {
            result.pop_back();
        }

        std::reverse(result.begin(), result.end());
        return LargeNumber(result);
    }

    friend LargeNumber operator%(const LargeNumber& dividend, const LargeNumber& divisor) {
        LargeNumber dividendCopy = dividend;
        LargeNumber divisorCopy = divisor;
        LargeNumber quotient = dividendCopy / divisorCopy;
        LargeNumber tempResult = quotient * divisorCopy;
        tempResult = dividendCopy - tempResult;
        return LargeNumber(tempResult);
    }

    bool operator!=(const LargeNumber& other) const {
        return value != other.value;
    }

    bool operator==(const LargeNumber& other) const {
        return value == other.value;
    }

    bool operator>(const LargeNumber& other) const {
        return value > other.value;
    }

    static LargeNumber GCD(LargeNumber a, LargeNumber b) {
        while (b != LargeNumber("0")) {
            LargeNumber temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    LargeNumber operator/(const LargeNumber& divisor) const {
        if (divisor == LargeNumber("0")) {
            throw std::invalid_argument("Division by zero error");
        }

        string quotient = "";
        string remainder = "";

        for (char digit : value) {
            remainder = (remainder == "0") ? string(1, digit) : remainder + digit;
            int count = 0;

            if (isLessThan(remainder, divisor.value)) {
                if (!quotient.empty()) {
                    quotient += '0';
                }
                continue;
            }

            while (!isLessThan(remainder, divisor.value)) {
                remainder = Subtract(remainder, divisor.value);
                count++;
            }

            quotient += std::to_string(count);
        }

        while (quotient.length() > 1 && quotient[0] == '0') {
            quotient.erase(0, 1);
        }

        return LargeNumber(quotient.empty() ? "0" : quotient);
    }

    LargeNumber operator*(const LargeNumber& other) const {
        string first = value;
        string second = other.value;
        int length1 = first.size();
        int length2 = second.size();

        string result(length1 + length2, '0');

        for (int i = length1 - 1; i >= 0; --i) {
            int carry = 0;
            int digit1 = first[i] - '0';

            for (int j = length2 - 1; j >= 0; --j) {
                int digit2 = second[j] - '0';
                int sum = (digit1 * digit2) + (result[i + j + 1] - '0') + carry;
                carry = sum / 10;
                result[i + j + 1] = (sum % 10) + '0';
            }
            result[i] += carry;
        }

        size_t start = result.find_first_not_of('0');
        if (start != string::npos) {
            return LargeNumber(result.substr(start));
        } else {
            return LargeNumber("0");
        }
    }

    static LargeNumber Power(LargeNumber base, LargeNumber exponent, LargeNumber modulus) {
        LargeNumber result("1");
        while (exponent > LargeNumber("0")) {
            if (exponent.value.back() % 2 == 1) {
                result = (result * base) % modulus;
            }
            base = (base * base) % modulus;
            exponent = exponent / LargeNumber("2");
        }
        return result;
    }

    char ConvertToChar() const {
        int asciiValue = std::stoi(value);
        return static_cast<char>(asciiValue);
    }
    friend std::istream& operator>>(std::istream& is, LargeNumber& largeNum) {
        is >> largeNum.value;
        return is;
    }

};

#endif // CRYPTOMATH_H
