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
	vector<string> asd = splitString(text);
	string key;
	do
	{
		cout << "Enter key:";
		cin >> key;
		if (key.size() <= 8) break;
	} while (true);

	string IV;
	do
	{
		cout << "Enter key:";
		cin >> key;
		if (key.size() <= 8) break;
	} while (true);

	key = toUpperString(key);
	bitset<64> bitTestKey = convertStringToBitset(key);
	bitset<64> bitTestIV = convertStringToBitset(IV);
	bitset<64> tempBitTestIV = bitTestIV;
	vector<bitset<48>> vecBitTestKey = keyGeneration(bitTestKey);
	string decryptedText = "";
	string cryptedText = "";
	vector<bitset<64>> splitedCryptedText;
	vector<bitset<64>> splitedDecryptedText;

	for (int i = 0; i < asd.size(); i++)
	{
		bitset<64> bitTestPart = convertStringToBitset(asd[i]);
		bitset<64> Crypt = encrypt(bitTestIV, vecBitTestKey);
		bitset<64> cryptedTextBitPart = bitTestPart ^ Crypt;
		bitTestIV = cryptedTextBitPart;
		cryptedText += bitsetToString(cryptedTextBitPart);
		splitedCryptedText.push_back(cryptedTextBitPart);
	}
	
	for (auto cryptedText : splitedCryptedText) {
		bitset<64> Crypt = encrypt(tempBitTestIV, vecBitTestKey);
		bitset<64> decryptedTextBitPart = cryptedText ^ Crypt;
		tempBitTestIV = cryptedText;
		splitedDecryptedText.push_back(decryptedTextBitPart);
		decryptedText += bitsetToString(decryptedTextBitPart);
	}
	/*for (auto str : splitedCryptedText) {
		cout<<bitsetToString(str);
	}
	cout<<endl;
	for (auto str : splitedDecryptedText) {
		cout<<bitsetToString(str);
	}*/
	cout << cryptedText << endl;
	cout << decryptedText << endl;

	//cout << "decrypted text - " << deryptedText << endl << endl;

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
