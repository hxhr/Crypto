#ifndef CERTIFICATE_H
#define CERTIFICATE_H

#include <string>
#include <vector>
#include <sstream>
#include "RsaSig.h"
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

class TA;
class Client {
    friend class TA;
public:
    Client() {}
    Client(const string& id) : ID(id), sig() {
        // 确保 sig 对象的 RSA 生成或公钥设置已经正确初始化
        // 如果需要，可以在这里进一步初始化 sig 的 RSA 部分
    }
    void operator=(const Client&);
    string getID() const { return ID; }

    void callCertificate(const TA&);
    bool verifyCertificate(const string&, const TA&);
    string getCertificate() const { return Certificate; }
    void writeLog(const string&, const string&, bool);

public:
    RsaSig sig;

private:
    string Certificate;
    string ID;
};

class TA {
public:
    TA() {}
    TA(const string&);
    TA(int, const string&);
    void operator=(const TA&);
    string makeCertificate(Client&) const;
    string makeCACertificate(TA&) const;
    string selfSign();
    bool verifyCertificate(const string& cert, const TA& ta) const;
    string getID() const { return ID; }
    Public_key getPubTA() const { return this->sig.rsa.GetPublicKey(); }

private:
    RsaSig sig;
    string ID;
    int primeLen;
};

vector<string> stringSplit(const string& str, char delim);
string ZZ2str(const ZZ& zz);
ZZ str2ZZ(const string& str);
string int2str(int n);
int str2int(const string& str);

#endif // CERTIFICATE_H
