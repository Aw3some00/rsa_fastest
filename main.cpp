#include <iostream>
#include "cryptomath.h"
#include <vector>

using std::cin;
using std::cout;
using std::vector;

LargeNumber secret(LargeNumber& prime1, LargeNumber& prime2, LargeNumber e, LargeNumber& coef) {
    return (coef * (prime1 - LargeNumber("1")) * (prime2 - LargeNumber("1")) + LargeNumber("1")) / e;
}

vector<LargeNumber> encryptMessage(const std::string& message, LargeNumber e, LargeNumber m) {
    vector<LargeNumber> encrypted;
    for (char character : message) {
        LargeNumber ascii(std::to_string(static_cast<int>(character)));
        LargeNumber encryptedValue = LargeNumber::Power(ascii, e, m);
        encrypted.push_back(encryptedValue);
    }
    return encrypted;
}

std::string decryptMessage(const vector<LargeNumber>& encryptedChars, LargeNumber d, LargeNumber m) {
    std::string decrypted;

    for (const LargeNumber& encryptedChar : encryptedChars) {
        LargeNumber decryptedValue = LargeNumber::Power(encryptedChar, d, m);
        decrypted += decryptedValue.ConvertToChar();
    }

    return decrypted;
}

int main() {
    LargeNumber e, k;
    LargeNumber prime1, prime2, multiplier;


    cout << "Enter prime1: ";
    cin >> prime1;
    cout << "Enter prime2: ";
    cin >> prime2;
    cout << "Enter e: ";
    cin >> e;
    cout << "Enter k: ";
    cin >> k;


    multiplier = prime1 * prime2;




    std::string message;
    cout << " message: ";
    cin.ignore();
    std::getline(cin, message);


    vector<LargeNumber> encryptedMessage = encryptMessage(message, e, multiplier);


    std::cout << "Encrypted message: \n";
    for (const auto& encrypted : encryptedMessage) {
        encrypted.Show();

    }
    std::cout << "\n";


    std::string desMessage = decryptMessage(encryptedMessage,  secret(prime1, prime2, e, k), multiplier);
    std::cout << "Decrypted message: " << desMessage << std::endl;

    return 0;
}
