#include <iostream>
#include <fstream>
#include <bitset>
#include <windows.h>
#include <string>

using namespace std;

int parityBit(int value, int bits) {
    int count = 0;
    for (int i = 0; i < bits; ++i) {
        if (value & (1 << i)) {
            ++count;
        }
    }
    return count % 2;
}

int main() {
    SetConsoleOutputCP(1251);

    string encryptedFilePath = "C:\\Users\\westt\\source\\repos\\LAB2Z3SHUFR\\LAB2Z3SHUFR\\encrypted.bin";
    string decryptedFilePath = "C:\\Users\\westt\\source\\repos\\LAB2Z3SHUFR\\LAB2Z3SHUFR\\decrypted.txt";

    ifstream inFile(encryptedFilePath, ios::binary);
    if (!inFile) {
        cerr << "Не вдалося відкрити файл " << encryptedFilePath << " для читання!" << endl;
        return 1;
    }

    ofstream outFile(decryptedFilePath);
    if (!outFile) {
        cerr << "Не вдалося відкрити файл " << decryptedFilePath << " для запису!" << endl;
        return 1;
    }

    unsigned short encryptedChar;
    for (int i = 0; i < 8; ++i) {  // 8 рядків
        for (int j = 0; j < 8; ++j) {  // 8 символів у рядку
            inFile.read(reinterpret_cast<char*>(&encryptedChar), sizeof(encryptedChar));

            if (inFile.eof()) {
                cerr << "Несподіваний кінець файлу!" << endl;
                break;
            }

            // Витягуємо номер рядка (біти 0-2)
            int row = (encryptedChar & 0x7); // 0x7 = 0000 0111 (3 біти)

            // Витягуємо позицію символу в рядку (біти 3-5)
            int pos = (encryptedChar >> 3) & 0x7; // Зсуваємо на 3 біти і беремо 3 біти

            // Витягуємо біт парності для перших полів (біт 6)
            int parityFirstFields = (encryptedChar >> 6) & 0x1;

            // Витягуємо ASCII-код символу (біти 7-14)
            char decodedChar = static_cast<char>((encryptedChar >> 7) & 0xFF);  // Зсуваємо на 7 і беремо 8 біт

            // Витягуємо біт парності для ASCII-коду (біт 15)
            int parityAscii = (encryptedChar >> 15) & 0x1;

            // Перевіряємо біт парності для перших полів
            if (parityBit(row + (pos << 3), 6) != parityFirstFields) {
                cerr << "Помилка парності для рядка " << row << " і позиції " << pos << "!" << endl;
            }

            // Перевіряємо біт парності для ASCII-коду
            if (parityBit(decodedChar, 8) != parityAscii) {
                cerr << "Помилка парності для символу ASCII " << decodedChar << "!" << endl;
            }

            // Виводимо розшифрований символ в консоль
            cout << decodedChar;

            // Записуємо у файл
            outFile << decodedChar;
        }
        cout << endl;
        outFile << endl;
    }

    inFile.close();
    outFile.close();

    cout << "Розшифрування завершено, результат виведено у консоль і файл " << decryptedFilePath << "." << endl;

    return 0;
}
