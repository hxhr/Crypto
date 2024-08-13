#include "Certificate.h"
#include <NTL/ZZ.h>
#include <sstream>
using namespace std;
using namespace NTL;

int main()
{
    TA ta("Authority");
    TA ta2("Fake");
    Client HJL("HJL");
    Client JST("JST");
    Client BXY("JST");

    JST.callCertificate(ta);
    BXY.callCertificate(ta2);

    cout << "HJL 验证 JST的证书" << endl;
    if (HJL.verifyCertificate(JST.getCertificate(), ta)) {
        cout << "pass" << endl;
    }
    else {
        cout << "fail" << endl;
    }

    cout << "HJL 验证 BXY的证书" << endl;
    if (HJL.verifyCertificate(BXY.getCertificate(), ta)) {
        cout << "pass" << endl;
    }
    else {
        cout << "fail" << endl;
    }

    return 0;
}

