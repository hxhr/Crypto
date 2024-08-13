//#include "CertificateRepository.h"
//#include <iostream>
//
//void CertificateRepository::storeCertificate(const string& id, const vector<string>& certPath) {
//    cout << "Storing certificate for ID: " << id << endl;
//    for (const string& cert : certPath) {
//        cout << cert << endl;
//    }
//    certStorage[id] = certPath;
//}
//
//vector<string> CertificateRepository::queryCertificate(const string& id) const {
//    cout << "Querying certificate path for ID: " << id << endl;
//    auto it = certStorage.find(id);
//    if (it != certStorage.end()) {
//        for (const string& cert : it->second) {
//            cout << cert << endl;
//        }
//        return it->second;
//    }
//    cout << "Certificate path not found for ID: " << id << endl;
//    return {};
//}

#include "CertificateRepository.h"
#include <iostream>

void CertificateRepository::storeCertificate(const string& id, const vector<string>& certPath) {
    cout << "Storing certificate for ID: " << id << endl;
    certStorage[id] = certPath;  // 只存储完整且无重复的路径
}

vector<string> CertificateRepository::queryCertificate(const string& id) const {
    cout << "Querying certificate path for ID: " << id << endl;
    auto it = certStorage.find(id);
    if (it != certStorage.end()) {
        return it->second;
    }
    cout << "Certificate path not found for ID: " << id << endl;
    return {};
}
