#ifndef TOTP_H
#define TOTP_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <sstream>
#include <ctime>
#include <cmath>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <bitset>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>

#include <unordered_map>
#include <stdexcept>

class TOTP
{
    public:
        static std::string generateTOTP(const std::string& key, uint64_t counter, size_t digits = 6);
        static std::string base32_decode(const std::string& input);

    protected:

    private:

};

#endif // TOTP_H
