#ifndef RSA_H
#define RSA_H
#include<NTL/ZZ.h>
using namespace NTL;


const int PRIME_LEN1 = 512;
const int PRIME_LEN2 = 1024;

class RSA {
public:
	struct Public_key
	{
		ZZ n;
		ZZ b;
	};
	Public_key pub;// 用于存储外部设置的公钥，例如用于验证签名
	RSA();
	void setPublicKey(Public_key);// 仅设置公有部分的公钥，不影响key.pub
	void GenerateKey(int l=PRIME_LEN1);
	ZZ encrypt(ZZ);
	ZZ decrypt(ZZ);
	Public_key GetPublicKey() { return this->key.pub; }
private:
	struct Private_key
	{
		ZZ p;
		ZZ q;
		ZZ a;
	};
	struct Key//自己本身的公钥私钥
	{
		Public_key pub;
		Private_key pri;
	};
	Key key;
};

#endif 

