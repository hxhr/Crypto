//#include<iostream>
//#include<NTL/ZZ.h>
//#include<fstream>
//#include"RSA.h"
//
//#include<bitset>
//
//using namespace std;
//using namespace NTL;
//
//ZZ str2ZZ(const string& str)
//{
//	ZZ res;
//	int len = str.length();
//	for (int i = 0; i < len; i++)
//	{
//		res = (res << 8) + str[i];
//	}
//	return res;
//}
//string ZZ2str(const ZZ& num)
//{
//	ZZ n = num;
//	string res;
//	char ch;
//	while (n != 0)
//	{
//		ch = n % 256;
//		RightShift(n, n, 8);
//		res = ch + res;
//	}
//	return res;
//}
//
//
//int main()
//{
//	int len = 512;
//	ifstream file("message.txt", ios::in);
//	// 检查文件是否成功打开
//	if (!file.is_open()) {
//		std::cout << "Error: failed to open 'message.txt'." << std::endl;
//		return -1;  // 返回一个错误代码
//	}
//	ofstream cipher_text("cipher.txt", ios::out);
//	ofstream decode_file("decode.txt", ios::out);
//	ofstream key_file("key.txt", ios::out);
//	if (!cipher_text.is_open())
//		cout << "cipher fail" << endl;
//	if (!decode_file.is_open())
//		cout << "decode fail" << endl;
//	if (!key_file.is_open())
//		cout << "key fail" << endl;
//	string str;
//	Key k;
//	ZZ cipher;
//	ZZ message;
//	file >> message;
//	k = RSA(len);
//	key_file << "public key:" << endl;
//	key_file << "n,b" << endl;
//	key_file << k.pub.n << "," << k.pub.b << endl;
//	key_file << "private key" << endl;
//	key_file << "p,q,a" << endl;
//	key_file << k.pri.p << "," << k.pri.q << "," << k.pri.a << endl;
//	cout << "private key" << endl;
//	cout << k.pri.p << "," << k.pri.q << "," << k.pri.a << endl;
//	cout << "public key:" << endl;
//	cout << k.pub.n << "," << k.pub.b << endl;
//	cout << message << endl;
//	cipher = RSA_En(k.pub, message);
//	cipher_text << cipher;
//	message = RSA_De(k.pri, k.pub, cipher);
//	decode_file << message;
//}

#include <iostream>
#include <NTL/ZZ.h>
#include <fstream>
#include "RSA.h"

#include <bitset>

using namespace std;
using namespace NTL;

ZZ str2ZZ(const string& str)
{
    ZZ res;
    int len = str.length();
    for (int i = 0; i < len; i++)
    {
        res = (res << 8) + str[i];
    }
    return res;
}

string ZZ2str(const ZZ& num)
{
    ZZ n = num;
    string res;
    char ch;
    while (n != 0)
    {
        ch = n % 256;
        RightShift(n, n, 8);
        res = ch + res;
    }
    return res;
}

int main()
{
    int len = 512;
    ifstream file("message.txt", ios::in);
    // 检查文件是否成功打开
    if (!file.is_open()) {
        std::cout << "Error: failed to open 'message.txt'." << std::endl;
        return -1;  // 返回一个错误代码
    }
    ofstream cipher_text("cipher.txt", ios::out);
    ofstream decode_file("decode.txt", ios::out);
    ofstream key_file("key.txt", ios::out);

    if (!cipher_text.is_open()) {
        cout << "cipher fail" << endl;
        return -1;
    }

    if (!decode_file.is_open()) {
        cout << "decode fail" << endl;
        return -1;
    }

    if (!key_file.is_open()) {
        cout << "key fail" << endl;
        return -1;
    }

    // 读取文件内容到字符串
    string str((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    ZZ message = str2ZZ(str);  // 将字符串转换为ZZ类型的大整数

    Key k;
    ZZ cipher;

    // 生成RSA密钥对
    k = RSA(len);

    // 输出密钥信息到文件和控制台
    key_file << "public key:" << endl;
    key_file << "n,b" << endl;
    key_file << k.pub.n << "," << k.pub.b << endl;
    key_file << "private key" << endl;
    key_file << "p,q,a" << endl;
    key_file << k.pri.p << "," << k.pri.q << "," << k.pri.a << endl;

    cout << "private key" << endl;
    cout << k.pri.p << "," << k.pri.q << "," << k.pri.a << endl;
    cout << "public key:" << endl;
    cout << k.pub.n << "," << k.pub.b << endl;

    // 进行RSA加密和解密
    cipher = RSA_En(k.pub, message);    // 加密消息
    cipher_text << cipher;              // 写入加密消息到文件
    ZZ decoded_message = RSA_De(k.pri, k.pub, cipher);  // 解密消息
    decode_file << ZZ2str(decoded_message);             // 将解密后的ZZ类型转换为字符串并写入文件

    return 0;
}

