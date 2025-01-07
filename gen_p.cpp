#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

// Hàm cộng hai số lớn
void bigAdd(vector<int>& result, const vector<int>& num1, const vector<int>& num2) {
    int carry = 0;
    int size = max(num1.size(), num2.size()) + 1;
    result.resize(size, 0);
    
    for (int i = 0; i < size; ++i) {
        int sum = carry;
        if (i < num1.size()) sum += num1[i];
        if (i < num2.size()) sum += num2[i];
        result[i] = sum % 10;
        carry = sum / 10;
    }

    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();  // Xóa phần tử 0 thừa ở cuối
    }
}

// Hàm nhân số lớn với một số nguyên
void bigMultiply(vector<int>& result, const vector<int>& num, int multiplier) {
    int carry = 0;
    result.resize(num.size() + 1, 0);
    
    for (int i = 0; i < num.size(); ++i) {
        int product = num[i] * multiplier + carry;
        result[i] = product % 10;
        carry = product / 10;
    }

    if (carry) {
        result[num.size()] = carry;
    }

    // Xóa phần tử 0 thừa
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
}

// Hàm kiểm tra xem số lớn có phải là số nguyên tố không
bool isPrime(const vector<int>& num) {
    if (num.size() == 1 && num[0] == 1) return false;  // 1 không phải là số nguyên tố
    if (num.size() == 1 && num[0] == 2) return true;   // 2 là số nguyên tố
    if (num[0] % 2 == 0) return false;  // Loại bỏ số chẵn ngoài 2

    // Kiểm tra chia hết từ 2 đến căn bậc 2 của số
    vector<int> divisor;
    for (int d = 3; d * d <= num[0]; d += 2) {
        // Nếu chia hết thì không phải nguyên tố
        if (num[0] % d == 0) return false;
    }
    
    return true;
}

// Hàm sinh một số ngẫu nhiên có độ dài 8192 bit
vector<int> generateLargeNumber(int bits) {
    vector<int> num;
    int digits = (bits + 2) / 3;  // Mỗi chữ số thập phân chiếm khoảng 3 bit
    srand(time(0));

    for (int i = 0; i < digits; ++i) {
        num.push_back(rand() % 10);
    }
    return num;
}

int main() {
    // Sinh một số ngẫu nhiên 8192 bit
    int bits = 8192;
    bool check = false;
    while {!check} {
        vector<int> number = generateLargeNumber(bits);

        // Kiểm tra số nguyên tố
        if (isPrime(number)) {
            cout << "Generated number: ";
            for (int i = number.size() - 1; i >= 0; --i) {
                cout << number[i];
            }
            cout << endl;
            cout << "The number is prime." << endl;
            check = True;
        } else {
            cout << "The number is not prime." << endl;
            continue;
        }
    }
    return 0;
}