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

#include <unordered_map>
#include <stdexcept>

class TOTP
{
    public:
        static std::string generateTOTP(const std::string& key, uint64_t counter, size_t digits = 6);

        static std::string base32_encode(const std::string& input);
        static std::string base32_decode(const std::string& input);
        static size_t decode_b32key(uint8_t **k, size_t len);

    protected:

    private:

};

#endif // TOTP_H
