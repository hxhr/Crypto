#include "Certificate.h"
#include "CertificateRepository.h"
#include <NTL/ZZ.h>
#include <sstream>
#include <iostream>

using namespace std;
using namespace NTL;

int main() {

    // 创建根CA并自签名
    TA CAroot("CAroot");
    string caRootCert = CAroot.selfSign();  // 获取CAroot的完整证书内容

    // 创建下级CA并由根CA签发证书
    TA CA1("CA1");
    TA CA2("CA2");

    string ca1Cert = CAroot.makeCACertificate(CA1);
    string ca2Cert = CAroot.makeCACertificate(CA2);

    // 创建客户端 Alice, Bob 和 Eve，并向 CA1 或 CA2 申请证书
    Client Alice("Alice");
    Client Bob("Bob");
    Client Eve("Eve");

    string aliceCert = CA1.makeCertificate(Alice);
    string bobCert = CA2.makeCertificate(Bob);
    string eveCert = CA1.makeCertificate(Eve);

    // 创建证书库并存储证书
    CertificateRepository certRepo;
    certRepo.storeCertificate("CAroot", { caRootCert });  // 存储CAroot的完整证书
    certRepo.storeCertificate("CA1", { caRootCert, ca1Cert });  // 存储CA1的完整路径
    certRepo.storeCertificate("CA2", { caRootCert, ca2Cert });  // 存储CA2的完整路径
    certRepo.storeCertificate("Alice", { caRootCert, ca1Cert, aliceCert });  // 存储Alice的完整路径
    certRepo.storeCertificate("Bob", { caRootCert, ca2Cert, bobCert });  // 存储Bob的完整路径
    certRepo.storeCertificate("Eve", { caRootCert, ca1Cert, eveCert });  // 存储Eve的完整路径

    // Alice 向 Bob 发送消息和签名
    string message = "Hello, Bob!";
    ZZ messageHash = str2ZZ(message);
    ZZ aliceSignature = Alice.sig.sig(messageHash);



   // Bob 查询 Alice 的证书路径
    cout << "Bob querying certificate path for Alice:" << endl;
    vector<string> aliceCertPath = certRepo.queryCertificate("Alice");
    if (aliceCertPath.empty()) {
        cout << "No certificate path found for Alice." << endl;
        return 1;
    }

    // 打印 Alice 的证书路径
    for (const string& cert : aliceCertPath) {
        cout << cert << endl << "---------------------" << endl;
    }

    // Bob 验证 Alice 的证书路径
    bool validCertPath = true;
    TA* currentCA = &CAroot;

    for (size_t i = 0; i < aliceCertPath.size(); ++i) {
        cout << "Bob 验证 Alice 的证书，路径索引 " << i << endl;

        // 打印当前的 currentCA 信息
        cout << "当前的 currentCA 是: " << currentCA->getID() << endl;

        // 使用 Bob 来验证 Alice 证书路径中的每一张证书
        if (!Bob.verifyCertificate(aliceCertPath[i], *currentCA)) {
            validCertPath = false;
            cout << "证书验证在路径索引 " << i << " 失败" << endl;
            break;
        }

        // 仅当验证的证书是 CA 证书时更新 currentCA
        if (aliceCertPath[i].find("CA:") != string::npos) {
            if (aliceCertPath[i].find("CA1") != string::npos) {
                currentCA = &CA1;  // 更新为 CA1
            }
            else if (aliceCertPath[i].find("CA2") != string::npos) {
                currentCA = &CA2;  // 更新为 CA2
            }
            else if (aliceCertPath[i].find("CAroot") != string::npos) {
                currentCA = &CAroot;  // 保持为 CAroot
            }
        }
    }

    if (!validCertPath) {
        cout << "Alice 的证书路径无效." << endl;
        return 1;
    }
    else {
        cout << "Alice 的证书路径验证成功." << endl;
    }




    // Bob 用 Alice 的公钥验证签名
    Public_key alicePubKey = Alice.sig.rsa.GetPublicKey();
    Bob.sig.rsa.setPublicKey(alicePubKey);


    if (Bob.sig.ver(messageHash, aliceSignature)) {
        cout << "Message from Alice is verified successfully!" << endl;
    }
    else {
        cout << "Failed to verify message from Alice." << endl;
    }


    return 0;



    return 0;
}
