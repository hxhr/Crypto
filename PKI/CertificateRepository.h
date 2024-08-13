#ifndef CERTIFICATEREPOSITORY_H
#define CERTIFICATEREPOSITORY_H

#include <string>
#include <vector>
#include <map>
using namespace std;

class CertificateRepository {
public:
    void storeCertificate(const string& id, const vector<string>& certPath);
    vector<string> queryCertificate(const string& id) const;

private:
    map<string, vector<string>> certStorage;
};

#endif // CERTIFICATEREPOSITORY_H
