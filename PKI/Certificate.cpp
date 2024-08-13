#include "Certificate.h"
#include <NTL/ZZ.h>
#include <fstream>
#include <direct.h>
#include <io.h>
using namespace NTL;

vector<std::string> stringSplit(const string& str, char delim) {
    stringstream ss(str);
    string item;
    vector<std::string> elems;
    while (getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

string ZZ2str(const ZZ& zz) {
    stringstream s;
    s << zz;
    return s.str();
}

ZZ str2ZZ(const string& str) {
    int len = str.length();
    ZZ res;
    res = 0;
    for (int i = 0; i < len; i++) {
        res = res * 10 + str[i] - '0';
    }
    return res;
}

string int2str(int n) {
    stringstream s;
    s << n;
    return s.str();
}

int str2int(const string& str) {
    int res(0);
    int len = str.length();
    for (int i = 0; i < len; i++)
        res = res * 10 + str[i] - '0';
    return res;
}

TA::TA(const string& id) {
    primeLen = PRIME_LEN1;
    this->sig.rsa.GenerateKey();
    this->ID = id;
}

TA::TA(int len, const string& id) {
    try {
        if (len == PRIME_LEN1 || len == PRIME_LEN2)
            primeLen = len;
        else
            throw string("prime length not allowed");
    }
    catch (string e) {
        cerr << e << endl;
    }
    this->sig.rsa.GenerateKey(len);
    this->ID = id;
}

string TA::selfSign() {
    string str1 = this->ID;
    Public_key pub = this->sig.rsa.GetPublicKey();
    string str2 = ZZ2str(pub.b);
    string str3 = ZZ2str(pub.n);
    ZZ sig_obj = str2ZZ(str1 + str2 + str3);
    ZZ s = this->sig.sig(sig_obj % pub.n);

    string cert =
        "CA:\n"
        + this->ID + "\n"
        + "b:\n"
        + ZZ2str(pub.b) + "\n"
        + "n:\n"
        + ZZ2str(pub.n) + "\n"
        + "s:\n"
        + ZZ2str(s) + "\n";

    string filename = ".\\" + this->ID + "_cert.txt";
    ofstream fcert(filename, ios::out);
    if (!fcert.is_open()) {
        cerr << "cert open error" << endl;
        return "";
    }
    fcert << cert;
    fcert.close();

    return cert;  // 返回生成的证书字符串
}


string TA::makeCertificate(Client& user) const {
    int pubKeyLen[2];
    user.sig.rsa.GenerateKey(primeLen);
    Public_key pub = user.sig.rsa.GetPublicKey();
    pubKeyLen[0] = ZZ2str(pub.b).length();
    pubKeyLen[1] = ZZ2str(pub.n).length();

    string str1 = user.getID();
    string str2 = ZZ2str(pub.b);
    string str3 = ZZ2str(pub.n);
    ZZ sig_obj = str2ZZ(str1 + str2 + str3);

    Public_key pubTA = this->sig.rsa.GetPublicKey();
    ZZ s = this->sig.sig(sig_obj % pubTA.n);

    string cert =
        "Client:\n"
        + user.getID() + "\n"
        + "b:\n"
        + ZZ2str(pub.b) + "\n"
        + "n:\n"
        + ZZ2str(pub.n) + "\n"
        + "s:\n"
        + ZZ2str(s) + "\n"
        + "TA_ID:\n"
        + this->ID + "\n"
        + "b_len:\n"
        + int2str(pubKeyLen[0]) + "\n"
        + "n_len:\n"
        + int2str(pubKeyLen[1]) + "\n";

    string dir = ".\\" + user.getID();
    if (_access(dir.c_str(), 0) == -1)
        _mkdir(dir.c_str());
    string filename = ".\\" + user.getID() + "\\" + this->ID + "_cert.txt";
    ofstream fcert(filename, ios::out);
    if (!fcert.is_open()) {
        cerr << "cert open error" << endl;
        return "";
    }
    fcert << cert;
    fcert.close();

    return cert;
}

string TA::makeCACertificate(TA& ca) const {
    // 获取下级 CA 的公钥
    Public_key pub = ca.sig.rsa.GetPublicKey();
    string str1 = ca.getID();
    string str2 = ZZ2str(pub.b);
    string str3 = ZZ2str(pub.n);

    // 生成签名对象并进行签名
    ZZ sig_obj = str2ZZ(str1 + str2 + str3);
    Public_key pubTA = this->sig.rsa.GetPublicKey();
    ZZ s = this->sig.sig(sig_obj % pubTA.n);

    // 生成证书内容
    string cert =
        "CA:\n"
        + ca.getID() + "\n"
        + "b:\n"
        + ZZ2str(pub.b) + "\n"
        + "n:\n"
        + ZZ2str(pub.n) + "\n"
        + "s:\n"
        + ZZ2str(s) + "\n"
        + "Issuer:\n"
        + this->ID + "\n";

    // 保存证书到文件
    string filename = ".\\" + ca.getID() + "_cert.txt";
    ofstream fcert(filename, ios::out);
    if (!fcert.is_open()) {
        cerr << "cert open error" << endl;
        return "";
    }
    fcert << cert;
    fcert.close();

    return cert;
}


//string TA::makeCACertificate(TA& ca) const {
//    ca.sig.rsa.GenerateKey(primeLen);
//    Public_key pub = ca.sig.rsa.GetPublicKey();
//    string str1 = ca.getID();
//    string str2 = ZZ2str(pub.b);
//    string str3 = ZZ2str(pub.n);
//    ZZ sig_obj = str2ZZ(str1 + str2 + str3);
//    ZZ s = this->sig.sig(sig_obj % pub.n);
//
//    string cert =
//        "CA:\n"
//        + ca.getID() + "\n"
//        + "b:\n"
//        + ZZ2str(pub.b) + "\n"
//        + "n:\n"
//        + ZZ2str(pub.n) + "\n"
//        + "s:\n"
//        + ZZ2str(s) + "\n"
//        + "Issuer:\n"
//        + this->ID + "\n";
//
//    string filename = ".\\" + ca.getID() + "_cert.txt";
//    ofstream fcert(filename, ios::out);
//    if (!fcert.is_open()) {
//        cerr << "cert open error" << endl;
//        return "";
//    }
//    fcert << cert;
//    fcert.close();
//
//    return cert;
//}

void Client::callCertificate(const TA& ta) {
    this->Certificate = ta.makeCertificate(*this);
}

void Client::writeLog(const string& client, const string& TAname, bool is_pass) {
    string dir = ".\\" + this->ID;
    if (_access(dir.c_str(), 0) == -1)
        _mkdir(dir.c_str());
    string filename = ".\\" + this->ID + "\\ver.log";
    ofstream log(filename, ios::out | ios::app);
    if (!log.is_open()) {
        cerr << "log open error" << endl;
        return;
    }

    time_t rawtime;
    struct tm* ptminfo;

    time(&rawtime);
    ptminfo = localtime(&rawtime);

    log << "--------------------" << endl;
    log << "date: " << ptminfo->tm_year + 1900 << "/" << ptminfo->tm_mon + 1 << "/" << ptminfo->tm_mday << endl;
    log << "time: " << ptminfo->tm_hour << ":" << ptminfo->tm_min << ":" << ptminfo->tm_sec << endl;
    log << "from:" << client << endl;
    log << "to: " << this->ID << endl;
    log << "TA:" << TAname << endl;
    if (is_pass)
        log << "status: pass" << endl;
    else
        log << "status: fail" << endl;
    log << "--------------------" << endl;

    log.close();
}

bool Client::verifyCertificate(const string& cert, const TA& ta) {
    string TAname = ta.getID();
    stringstream stream(cert);
    string ID;
    getline(stream, ID);
    getline(stream, ID);

    string bstr, nstr;
    ZZ b, n;
    Public_key pub;
    getline(stream, bstr);
    getline(stream, bstr);
    getline(stream, nstr);
    getline(stream, nstr);
    b = str2ZZ(bstr);
    n = str2ZZ(nstr);
    pub.b = b;
    pub.n = n;

    string sstr;
    ZZ s;
    getline(stream, sstr);
    getline(stream, sstr);
    s = str2ZZ(sstr);

    ZZ sig_obj = str2ZZ(ID + bstr + nstr);

    Public_key pubTA = ta.getPubTA();
    this->sig.rsa.setPublicKey(pubTA);

    bool is_pass = this->sig.ver(sig_obj % pubTA.n, s % pubTA.n);
    if (is_pass)
        this->sig.rsa.setPublicKey(pub);

    writeLog(ID, TAname, is_pass);

    return is_pass;
}


bool TA::verifyCertificate(const string& cert, const TA& ta) const {
    stringstream stream(cert);
    string type;
    getline(stream, type);  // 获取证书类型

    cout << "Verifying certificate: " << endl << cert << endl;

    if (type == "CA:") {
        string caID;
        getline(stream, caID);
        string bstr, nstr;
        ZZ b, n;
        Public_key pub;
        getline(stream, bstr);
        getline(stream, bstr);
        getline(stream, nstr);
        getline(stream, nstr);
        b = str2ZZ(bstr);
        n = str2ZZ(nstr);
        pub.b = b;
        pub.n = n;

        cout << "Parsed CA certificate:" << endl;
        cout << "ID: " << caID << endl;
        cout << "b: " << b << endl;
        cout << "n: " << n << endl;

        string sstr;
        ZZ s;
        getline(stream, sstr);
        getline(stream, sstr);
        s = str2ZZ(sstr);

        cout << "s: " << s << endl;

        ZZ sig_obj = str2ZZ(caID + bstr + nstr);
        cout << "sig_obj: " << sig_obj << endl;

        Public_key pubTA = ta.getPubTA();
        cout << "pubTA.b: " << pubTA.b << endl;
        cout << "pubTA.n: " << pubTA.n << endl;

        RsaSig sig;
        sig.rsa.setPublicKey(pubTA);

        cout << "Verifying signature..." << endl;
        bool result = sig.ver(sig_obj % pubTA.n, s % pubTA.n);

        cout << "Signature verification result: " << result << endl;

        return result;
    }

    return false;
}

void Client::operator=(const Client& client) {
    this->Certificate = client.Certificate;
    this->ID = client.ID;
    this->sig = client.sig;
}

void TA::operator=(const TA& ta) {
    this->ID = ta.ID;
    this->sig = ta.sig;
    this->primeLen = ta.primeLen;
}
