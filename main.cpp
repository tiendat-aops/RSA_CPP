#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <sstream>
using namespace std;

class BigNumber {
private:
    vector<int> digits; // Store digits in reverse order for easier calculations
    bool isNegative;

    void removeLeadingZeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
        if (digits.size() == 1 && digits[0] == 0) {
            isNegative = false; // Zero is not negative
        }
    }

public:
    // Constructors
    BigNumber() : isNegative(false) {}

    BigNumber(const string &number) {
        isNegative = (number[0] == '-');
        for (int i = number.size() - 1; i >= (isNegative ? 1 : 0); --i) {
            digits.push_back(number[i] - '0');
        }
        removeLeadingZeros();
    }

    // Add this function inside the BigNumber class
    string getValue() const {
        if (digits.empty()) {
            return "0"; // Handle empty BigNumber case
        }

        std::ostringstream result;

        if (isNegative) {
            result << "-";
        }

        for (size_t i = digits.size(); i-- > 0;) { // Reverse order
            result << digits[i];
        }

        return result.str();
    }

    // Operator overloading for addition
    BigNumber operator+(const BigNumber &other) const {
        if (isNegative == other.isNegative) {
            BigNumber result;
            result.isNegative = isNegative;

            int carry = 0;
            size_t maxLength = max(digits.size(), other.digits.size());

            for (size_t i = 0; i < maxLength || carry; ++i) {
                int sum = carry;
                if (i < digits.size()) sum += digits[i];
                if (i < other.digits.size()) sum += other.digits[i];
                result.digits.push_back(sum % 10);
                carry = sum / 10;
            }

            result.removeLeadingZeros();
            return result;
        }
        // Handle addition when signs are different
        return *this - (-other);
    }

    // Operator overloading for subtraction
    BigNumber operator-(const BigNumber &other) const {
        if (isNegative != other.isNegative) {
            return *this + (-other);
        }

        if (abs() < other.abs()) {
            return -(other - *this);
        }

        BigNumber result;
        result.isNegative = isNegative;

        int borrow = 0;
        for (size_t i = 0; i < digits.size() || borrow; ++i) {
            int diff = digits[i] - borrow;
            if (i < other.digits.size()) diff -= other.digits[i];
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.digits.push_back(diff);
        }

        result.removeLeadingZeros();
        return result;
    }

    // Operator overloading for multiplication
    BigNumber operator*(const BigNumber &other) const {
        BigNumber result;
        result.digits.resize(digits.size() + other.digits.size(), 0);
        result.isNegative = (isNegative != other.isNegative);

        for (size_t i = 0; i < digits.size(); ++i) {
            int carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; ++j) {
                long long current = result.digits[i + j] + carry;
                if (j < other.digits.size()) {
                    current += 1LL * digits[i] * other.digits[j];
                }
                result.digits[i + j] = current % 10;
                carry = current / 10;
            }
        }

        result.removeLeadingZeros();
        return result;
    }

    // Operator overloading for division
    BigNumber operator/(const BigNumber &other) const {
        if (other.isZero()) {
            throw runtime_error("Division by zero");
        }

        BigNumber dividend = abs();
        BigNumber divisor = other.abs();
        BigNumber result;
        result.digits.resize(dividend.digits.size(), 0);

        BigNumber current;
        for (size_t i = dividend.digits.size(); i-- > 0;) {
            current.digits.insert(current.digits.begin(), dividend.digits[i]);
            current.removeLeadingZeros();

            int quotient = 0;
            while (!(current < divisor)) { // Use >= implicitly
                current = current - divisor;
                ++quotient;
            }
            result.digits[i] = quotient;
        }

        result.isNegative = (isNegative != other.isNegative);
        result.removeLeadingZeros();
        return result;
    }

    // Operator overloading for modulo
    BigNumber operator%(const BigNumber &other) const {
        if (other.isZero()) {
            throw runtime_error("Modulo by zero");
        }

        BigNumber dividend = abs();
        BigNumber divisor = other.abs();
        BigNumber current;

        for (size_t i = dividend.digits.size(); i-- > 0;) {
            current.digits.insert(current.digits.begin(), dividend.digits[i]);
            current.removeLeadingZeros();

            while (!(current < divisor)) {
                current = current - divisor;
            }
        }

        current.isNegative = isNegative;
        current.removeLeadingZeros();
        return current;
    }

    // Function to convert BigNumber to binary
    string toBinary() const {
        if (isZero()) return "0";

        BigNumber number = abs();
        string binary = "";
        BigNumber two("2");

        while (!number.isZero()) {
            BigNumber remainder = number % two;
            binary += (remainder.digits[0] + '0');
            number = number / two;
        }

        if (isNegative) {
            binary += "-";
        }

        reverse(binary.begin(), binary.end());
        return binary;
    }

    // Function to calculate modular inverse using Extended Euclidean Algorithm
    BigNumber modularInverse(const BigNumber &mod) const {
        if (mod.isZero()) {
            throw runtime_error("Modulo by zero");
        }

        BigNumber a = abs();
        BigNumber m = mod.abs();
        BigNumber x0("0"), x1("1"), y0("1"), y1("0");

        while (!m.isZero()) {
            BigNumber q = a / m;
            BigNumber r = a % m;

            BigNumber tempX = x1 - q * x0;
            BigNumber tempY = y1 - q * y0;

            a = m;
            m = r;
            x1 = x0;
            x0 = tempX;
            y1 = y0;
            y0 = tempY;
        }

        if (!(a == BigNumber("1"))) {
            throw runtime_error("Modular inverse does not exist");
        }

        if (x1.isNegative) {
            x1 = x1 + mod.abs();
        }

        return x1;
    }


    // Function to perform modular exponentiation (a^b mod n)
    BigNumber modularExponentiation(const BigNumber &b, const BigNumber &n) const {
        BigNumber a = *this;
        string bBinary = b.toBinary();
        BigNumber result("1");
        BigNumber base = a % n;

        for (int i = bBinary.size() - 1; i >= 0; --i) {
            if (bBinary[i] == '1') {
                result = (result * base) % n;
            }
            base = (base * base) % n;
        }

        return result;
    }

    // Negation operator
    BigNumber operator-() const {
        BigNumber result = *this;
        if (!result.isZero()) {
            result.isNegative = !isNegative;
        }
        return result;
    }

    // Absolute value
    BigNumber abs() const {
        BigNumber result = *this;
        result.isNegative = false;
        return result;
    }

    // Comparison operators
    bool operator<(const BigNumber &other) const {
        if (isNegative != other.isNegative) {
            return isNegative;
        }

        if (digits.size() != other.digits.size()) {
            return isNegative ? digits.size() > other.digits.size() : digits.size() < other.digits.size();
        }

        for (size_t i = digits.size(); i-- > 0;) {
            if (digits[i] != other.digits[i]) {
                return isNegative ? digits[i] > other.digits[i] : digits[i] < other.digits[i];
            }
        }

        return false;
    }

    bool operator==(const BigNumber &other) const {
        return isNegative == other.isNegative && digits == other.digits;
    }

    bool operator>=(const BigNumber &other) const {
        return !(*this < other);
    }

    bool isZero() const {
        return digits.size() == 1 && digits[0] == 0;
    }

    // Print the number
    friend ostream &operator<<(ostream &os, const BigNumber &number) {
        if (number.isNegative) os << "-";
        for (size_t i = number.digits.size(); i-- > 0;) {
            os << number.digits[i];
        }
        return os;
    }

    // RSA encryption and decryption
    tuple<BigNumber, BigNumber, BigNumber> RSA(const BigNumber &p, const BigNumber &q) const {
        BigNumber one("1");
        BigNumber e = *this;
        BigNumber n = p * q;
        cout << "n: " << n << endl;

        BigNumber phi_n = (p - one) * (q - one);
        cout << "phi_n: " << phi_n << endl;

        BigNumber d = e.modularInverse(phi_n);
        cout << "d: " << d << endl;

        return make_tuple(n, phi_n, d);
    }
};

string load_prime(string fileName) {
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cerr << "Can not open file: " << fileName << endl;
        return "";
    }

    string primeNumber;
    getline(inputFile, primeNumber);

    inputFile.close();

    return primeNumber;
}

// int main() {
//     // BigNumber num1("3674911");
//     // BigNumber num2("6007800");

//     // cout << "num1: " << num1 << endl;
//     // cout << "num2: " << num2 << endl;

//     // BigNumber sum = num1 + num2;
//     // cout << "Sum: " << sum << endl;

//     // BigNumber diff = num2 - num1;
//     // cout << "Difference: " << diff << endl;

//     // BigNumber prod = num1 * num2;
//     // cout << "Product: " << prod << endl;

//     // BigNumber quotient = num2 / num1;
//     // cout << "Quotient: " << quotient << endl;

//     // BigNumber mod = num2 % num1;
//     // cout << "Modulo: " << mod << endl;

//     // cout << "Binary of num1: " << num1.toBinary() << endl;

//     // BigNumber modInverse = num1.modularInverse(num2);
//     // cout << "Modular Inverse of num1 mod num2: " << modInverse << endl;

//     // BigNumber x("5234673");
//     // BigNumber e("3674911");
//     // BigNumber n("6012707");
//     // BigNumber modExp = x.modularExponentiation(e,n);
//     // cout << "Modular Exponentiation of num1^num2 mod 1000000007: " << modExp << endl;

//     string p_str = load_prime("prime_number_p_512.txt");
//     string q_str = load_prime("prime_number_q_512.txt");
//     string e_str = load_prime("prime_number_e_1024.txt");

//     BigNumber p(p_str);
//     BigNumber q(q_str);
//     BigNumber e(e_str);
//     BigNumber message("123");

//     cout << "Message: " << message << endl;

//     BigNumber result = message.RSA(p, q, e);
//     cout << "RSA result: " << result << endl;
// }