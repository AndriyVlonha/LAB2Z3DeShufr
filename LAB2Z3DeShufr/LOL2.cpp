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
        cerr << "�� ������� ������� ���� " << encryptedFilePath << " ��� �������!" << endl;
        return 1;
    }

    ofstream outFile(decryptedFilePath);
    if (!outFile) {
        cerr << "�� ������� ������� ���� " << decryptedFilePath << " ��� ������!" << endl;
        return 1;
    }

    unsigned short encryptedChar;
    for (int i = 0; i < 8; ++i) {  // 8 �����
        for (int j = 0; j < 8; ++j) {  // 8 ������� � �����
            inFile.read(reinterpret_cast<char*>(&encryptedChar), sizeof(encryptedChar));

            if (inFile.eof()) {
                cerr << "����������� ����� �����!" << endl;
                break;
            }

            // �������� ����� ����� (��� 0-2)
            int row = (encryptedChar & 0x7); // 0x7 = 0000 0111 (3 ���)

            // �������� ������� ������� � ����� (��� 3-5)
            int pos = (encryptedChar >> 3) & 0x7; // ������� �� 3 ��� � ������ 3 ���

            // �������� �� ������� ��� ������ ���� (�� 6)
            int parityFirstFields = (encryptedChar >> 6) & 0x1;

            // �������� ASCII-��� ������� (��� 7-14)
            char decodedChar = static_cast<char>((encryptedChar >> 7) & 0xFF);  // ������� �� 7 � ������ 8 ��

            // �������� �� ������� ��� ASCII-���� (�� 15)
            int parityAscii = (encryptedChar >> 15) & 0x1;

            // ���������� �� ������� ��� ������ ����
            if (parityBit(row + (pos << 3), 6) != parityFirstFields) {
                cerr << "������� ������� ��� ����� " << row << " � ������� " << pos << "!" << endl;
            }

            // ���������� �� ������� ��� ASCII-����
            if (parityBit(decodedChar, 8) != parityAscii) {
                cerr << "������� ������� ��� ������� ASCII " << decodedChar << "!" << endl;
            }

            // �������� ������������� ������ � �������
            cout << decodedChar;

            // �������� � ����
            outFile << decodedChar;
        }
        cout << endl;
        outFile << endl;
    }

    inFile.close();
    outFile.close();

    cout << "������������� ���������, ��������� �������� � ������� � ���� " << decryptedFilePath << "." << endl;

    return 0;
}
