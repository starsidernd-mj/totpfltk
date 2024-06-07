#ifndef TOTP_H
#define TOTP_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <cmath>
#include <openssl/hmac.h>
#include <openssl/sha.h>

class TOTP
{
    public:
        static std::string generateTOTP(const std::string& key, uint64_t counter, size_t digits = 6);

    protected:

    private:

};

#endif // TOTP_H
