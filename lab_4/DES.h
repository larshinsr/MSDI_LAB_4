#pragma once
#include <map>
#include <bitset>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm> 
#include "Tables.h"
using namespace std;


bitset<64> convertStringToBitset(string input) {			//Преобразует строку символов в 64-битный bitset.
	bitset<64> output;
	for (int i = 0; i < input.size(); i++) {
		output <<= 8;
		output |= bitset<64>(input[i]);
	}
	return output;
}

std::string bitsetToString(std::bitset<64> bits) {			//Преобразует 64-битный bitset в строку символов.
	std::string result;
	for (int i = 0; i < 8; i++) {
		std::bitset<8> part(bits.to_ullong() >> (i * 8));
		result += char(part.to_ulong());
	}
	reverse(result.begin(), result.end());
	return result;
}

bitset<64> IP(bitset<64> T)									//Применяет начальную перестановку к 64-битному bitset.
{
	bitset<64> PT;
	for (int i = 63; i >= 0; i--)
	{
		PT.set(i, T[63 - (INITIAL_PERMUTATION[63 - i] - 1)]);
	}
	return PT;
}

bitset<56> keyPrepare(bitset<64> initialKey)				//Применяет шаги начальной подготовки ключа к 64-битному ключу.
{
	bitset<56> CDKey;
	for (int i = 55; i >= 0; i--)
	{
		CDKey.set(i, initialKey[63 - (PREPARE_KEY[55 - i] - 1)]); // по таблице сжимается с 64 до 56 бит
	}
	return CDKey;
}
std::bitset<28> rotate_left(std::bitset<28> bs, int j) {			//Выполняет циклический сдвиг влево bitset.
	return (bs << j) | (bs >> (28 - j));
}
bitset<56> join_bitsets56(bitset<28> bs1, bitset<28> bs2) {			//Объединяет два 28-битных bitset в 56-битный bitset.
	bitset<56> result;
	for (int i = 0; i < 28; i++) {
		result[i] = bs1[i];
		result[i + 28] = bs2[i];
	}
	return result;
}

bitset<64> join_bitsets64(bitset<32> bs1, bitset<32> bs2) {			//Объединяет два 32-битных bitset в 64-битный bitset.
	bitset<64> result;
	for (int i = 0; i < 32; i++) {
		result[i] = bs1[i];
		result[i + 32] = bs2[i];
	}
	return result;
}

bitset<48> finalPrepareKey(bitset<56> CD)							//Применяет шаги окончательной подготовки ключа к 56-битному ключу, получая 48-битный подо ключ	
{
	bitset<48> CDKey;
	for (int i = 47; i >= 0; i--)
	{
		CDKey.set(i, CD[55 - (FINAL_PREPARE_KEY[47 - i] - 1)]);
	}
	return CDKey;
}
vector<bitset<48>> keyGeneration(bitset<64> initialKey)				//Генерирует все 16 раундовых ключей для процесса шифрования/расшифрования DES.
{
	bitset<56> preparedKey = keyPrepare(initialKey); // Начальная подготовка ключа
	bitset<28> C = (preparedKey >> 28).to_ulong();
	bitset<28> D = (preparedKey & bitset<56>(0xFFFFFFF)).to_ulong();
	vector<bitset<28>> vecCi;
	vector<bitset<28>> vecDi;
	vecCi.push_back(rotate_left(C, 1)); // Сдвиг на 1 влево
	vecDi.push_back(rotate_left(D, 1));// Сдвиг на 1 влево
	for (int i = 1; i < 16; i++)
	{
		if (i == 1 || i == 8 || i == 15)
		{
			vecCi.push_back(rotate_left(vecCi[i - 1], 1));// Сдвиг на 1 влево
			vecDi.push_back(rotate_left(vecDi[i - 1], 1));// Сдвиг на 1 влево
		}
		else
		{
			vecCi.push_back(rotate_left(vecCi[i - 1], 2));// Сдвиг на 2 влево
			vecDi.push_back(rotate_left(vecDi[i - 1], 2));// Сдвиг на 2 влево
		}
	}
	vector<bitset<56>>vecCiDi;
	for (int i = 0; i < vecCi.size(); i++)
	{
		vecCiDi.push_back(join_bitsets56(vecDi[i], vecCi[i])); // Склеиваем Ci и Di
	}
	vector<bitset<48>> keys;
	for (int i = 0; i < vecCiDi.size(); i++)
	{
		keys.push_back(finalPrepareKey(vecCiDi[i])); // Финальная перестановка ключей до 48 бит
	}
	return keys;
}
bitset<48> E(bitset<32> R)							//	Применяет операцию расширения к 32-битному bitset, получая 48-битный bitset.		
{
	bitset<48> ExpandedR;
	for (int i = 47; i >= 0; i--)
	{
		ExpandedR.set(i, R[31 - (EXPANSION[47 - i] - 1)]); // Перестановка с помощью таблицы
	}
	return ExpandedR;
}

bitset<48> xor_addition(bitset<48> bitset1, bitset<48> bitset2) {			//Выполняет поразрядное XOR-сложение двух bitset.
	return bitset1 ^ bitset2;
}

int getRow(int a, int b)							//Получает индекс строки для определенного S-Box на основе двух входных бит.
{
	int merge = a * 10 + b;
	string mergeString = to_string(merge);
	bitset<2> bitMerge{ mergeString };
	return bitMerge.to_ulong();
}

int getCol(int a, int b, int c, int d)				//Получает индекс столбца для определенного S-Box на основе четырех входных бит.			
{
	int merge = a * 1000 + b * 100 + c * 10 + d;
	string mergeString = to_string(merge);
	bitset<4> bitMerge{ mergeString };
	return bitMerge.to_ulong();
}

bitset<32> convertS(std::bitset<48> XORedKey)		//Применяет подстановку S-Box к 48-битному bitset.	
{
	vector<bitset<6>> SBlock; //
	bitset<32> mergeBlocks; // То что мы возвращаем
	bitset<4> bitsetArray[8]; //  Для получения из таблицы S_BOX

	string bits_str = XORedKey.to_string();
	bitset<6> part1(bits_str.substr(0, 6));
	SBlock.push_back(part1);
	bitset<6> part2(bits_str.substr(6, 6));
	SBlock.push_back(part2);
	bitset<6> part3(bits_str.substr(12, 6));
	SBlock.push_back(part3);
	bitset<6> part4(bits_str.substr(18, 6));
	SBlock.push_back(part4);
	bitset<6> part5(bits_str.substr(24, 6));
	SBlock.push_back(part5);
	bitset<6> part6(bits_str.substr(30, 6));
	SBlock.push_back(part6);
	bitset<6> part7(bits_str.substr(36, 6));
	SBlock.push_back(part7);
	bitset<6> part8(bits_str.substr(42, 6));
	SBlock.push_back(part8);

	for (int i = 0; i < 8; i++)
	{
		int row = getRow(SBlock[i][5], SBlock[i][0]); // Получение строки
		int col = getCol(SBlock[i][4], SBlock[i][3], SBlock[i][2], SBlock[i][1]); // Получение столбца
		unsigned long long value = S_BOX[i][row][col];
		bitset<4> BBlock{ value };
		bitsetArray[i] = BBlock;
	}
	for (int i = 0; i < 8; i++) {
		mergeBlocks |= bitsetArray[i].to_ullong();
		if (i < 7) { // Не сдвигаем на последней итерации
			mergeBlocks <<= 4;
		}
	}
	return mergeBlocks;
}

bitset<32> Penetration(bitset<32> bitset1)			//Применяет P-перестановку к 32-битному bitset.
{
	bitset<32> PenetratedBit;
	for (int i = 31; i >= 0; i--)
	{
		PenetratedBit.set(i, bitset1[31 - (P[31 - i] - 1)]);
	}
	return PenetratedBit;
}

bitset<32> F(bitset<32> R, bitset<48> key)					//Функция Фейстеля, которая является ядром алгоритма DES. Она принимает 32-битный блок данных и 48-битный подо ключ в качестве входных данных и выполняет подстановки и перестановки, чтобы генерировать новый 32-битный блок данных.
{
	bitset<48> ER = E(R);		// Функция расширения правой части до 48 бит
	bitset<48> XORedKeyR = xor_addition(ER, key); // XOR сложения ключа и Расширенной правой части
	bitset<32> convertedS = convertS(XORedKeyR); // Конвертация с помощью S блоков
	bitset<32> Penetraded = Penetration(convertedS); // Перестановка
	return Penetraded;
}

bitset<64> IPend(bitset<64> bitset1)					//Применяет конечную перестановку к 64-битному bitset.
{
	bitset<64> FinalPenet;
	for (int i = 63; i >= 0; i--)
	{
		FinalPenet.set(i, bitset1[63 - (FINAL_PERMUTATION[63 - i] - 1)]);
	}
	return FinalPenet;
}

bitset<64> encrypt(bitset<64> text, vector<bitset<48>> keys)			//Шифрует 64-битный блок данных с помощью алгоритма DES и предоставленного ключа.
{
	bitset<64> cryptedText;
	text = IP(text); // Начальная перестановка
	bitset<32> L(text.to_ullong() >> 32);
	bitset<32> R(text.to_ullong() & 0xffffffff);
	for (int i = 0; i < 16; i++)
	{
		bitset<32> Ltemp = L;
		L = R;
		R = Ltemp ^ F(R, keys[i]); // XOR с функцией Фейстеля
	}
	cryptedText = join_bitsets64(R, L); // Соединяем правую и левую
	cryptedText = IPend(cryptedText); // Конечная перестановка
	return cryptedText;
}

bitset<64> decrypt(bitset<64> cryptedText, vector<bitset<48>> keys)			//Расшифровывает 64-битный блок зашифрованного текста с помощью алгоритма DES и того же ключа (который должен быть
{
	bitset<64> decryptedText;
	cryptedText = IP(cryptedText);
	bitset<32> L(cryptedText.to_ullong() >> 32);
	bitset<32> R(cryptedText.to_ullong() & 0xffffffff);
	for (int i = 15; i >= 0; i--)
	{
		bitset<32> Rtemp = R;
		R = L;
		L = Rtemp ^ F(L, keys[i]);
	}
	decryptedText = join_bitsets64(R, L);
	decryptedText = IPend(decryptedText);
	return decryptedText;
}
//bitset<64> encrypt_DES_CFB(bitset<64> text, bitset<64> IV, vector<bitset<64>> keys)
//{
//    bitset<64> cryptedText;
//    bitset<64> prevBlock = IV; // Предыдущий зашифрованный блок начинается с IV
//    for (int i = 0; i < 16; i++)
//    {
//        cryptedText = encrypt(prevBlock, keys); // Шифруем предыдущий блок
//        prevBlock = cryptedText ^ text; // Применяем XOR с открытым текстом
//        text = cryptedText; // Передаем зашифрованный текст на следующую итерацию
//    }
//    return cryptedText;
//}
//
//
//bitset decrypt_DES_CFB(bitset<64> cryptedText, bitset<64> IV, vector<bitset<64>> keys)
//{
//	bitset<64> decryptedText;
//	bitset<64> prevBlock = IV; // Предыдущий зашифрованный блок начинается с IV
//	for (int i = 0; i < 16; i++)
//	{
//		bitset<64> tempBlock = cryptedText;
//		cryptedText = decrypt(prevBlock, keys); // Шифруем предыдущий блок
//		prevBlock = tempBlock; // Сохраняем зашифрованный блок для следующей итерации
//
//		//std::cout<<bitsetToString(tempBlock)<<std::endl;
//		//std::cout<<bitsetToString(decryptedText)<<std::endl;
//		decryptedText ^= tempBlock; // Применяем XOR с зашифрованным текстом
//	}
//	return decryptedText;
//}

