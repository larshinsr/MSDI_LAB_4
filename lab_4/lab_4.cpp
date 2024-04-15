#include <iostream>
#include <bitset>
#include <vector>
#include "DES.h"

using namespace std;

string toUpperString(string str);
vector<string> splitString(const string& str);

int main()
{
    cout << "Enter text:";
    string text;
    getline(cin, text);
    text = toUpperString(text);
    vector<string> textBlocks = splitString(text);

    string key;
    do
    {
        cout << "Enter key:";
        cin >> key;
        if (key.size() <= 8) break;
    } while (true);
    key = toUpperString(key);
    bitset<64> bitTestKey = convertStringToBitset(key);
    vector<bitset<48>> vecBitTestKey = keyGeneration(bitTestKey);

    string decryptedText = "";
    string encryptedText = "";
    bitset<64> IV = convertStringToBitset("initialV"); // Значение IV для начала процесса CFB

    for (int i = 0; i < textBlocks.size(); i++)
    {
        bitset<64> bitBlock = convertStringToBitset(textBlocks[i]);

        // Шифрование в режиме CFB
        bitset<64> encryptedBlock = encrypt_DES_CFB(bitBlock, IV, vecBitTestKey);
        IV = encryptedBlock; // Используем зашифрованный блок в качестве нового IV для следующего блока

        // Расшифрование в режиме CFB
        bitset<64> decryptedBlock = decrypt_DES_CFB(encryptedBlock, IV, vecBitTestKey);

        encryptedText += bitsetToString(encryptedBlock); // Преобразуем зашифрованный блок в строку
        decryptedText += bitsetToString(decryptedBlock); // Преобразуем расшифрованный блок в строку
    }

    cout << "Encrypted text: " << encryptedText << endl << endl;
    cout << "Decrypted text: " << decryptedText << endl << endl;
}

string toUpperString(string str)
{
    string newStr = "";
    for (char c : str)
    {
        c = toupper(c);
        newStr.push_back(c);
    }
    return newStr;
}

vector<string> splitString(const string& str) {
    vector<string> result;

    for (size_t i = 0; i < str.size(); i += 8) {
        result.push_back(str.substr(i, 8));
    }

    return result;
}
