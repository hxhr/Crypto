#include"RSA.h"
#include"RsaSig.h"
#include<iostream>
#include<fstream>
using namespace std;

#define wfile(file,out)	\
	file<<out;

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
	ofstream sign_file("sign.txt", ios::out);
	ofstream sign_file2("sign2.txt", ios::out);
	ofstream ver_file("ver.txt", ios::out);
	ofstream ver_file2("ver2.txt", ios::out);
	if (!sign_file.is_open())
		cout << "sign fail" << endl;
	if (!sign_file2.is_open())
		cout << "sign2 fail" << endl;
	if (!ver_file.is_open())
		cout << "decode fail" << endl;


	// 读取文件内容到字符串
	string str((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	ZZ x = str2ZZ(str);  // 将字符串转换为ZZ类型的大整数
	RsaSig alice;
	RsaSig bob;
	alice.rsa.GenerateKey();
	bob.rsa.setPublicKey(alice.rsa.GetPublicKey());
	ZZ sign=alice.sig(x);
	ZZ sign2 = sign + 1;
	bool ver=bob.ver(x,sign);
	bool ver2 = bob.ver(x, sign2);

	wfile(sign_file, sign);
	wfile(sign_file2, sign2);
	wfile(ver_file, ver);
	wfile(ver_file2, ver2);

	return 0;
}
