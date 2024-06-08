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
        static std::string generateTOTP2(const std::string& key, uint64_t counter, size_t digits = 6);

        static std::string base32_encode(const std::string& input);
        static std::string base32_decode(const std::string& input);
        static size_t decode_b32key(uint8_t **k, size_t len);
        static uint8_t *hmac(unsigned char *key, int kl, uint64_t interval);
        static uint32_t DT(uint8_t *digest);
        static uint32_t mod_hotp(uint32_t bin_code, int digits);

        /*static uint32_t TOTPa(uint8_t *key, size_t kl, uint64_t time, int digits);
        static uint32_t DT(uint8_t *digest);
        static uint8_t *hmac(unsigned char *key, int kl, uint64_t interval);
        static uint32_t mod_hotp(uint32_t bin_code, int digits);
        static uint32_t HOTP(uint8_t *key, size_t kl, uint64_t interval, int digits);*/

    protected:

    private:
        // Base32 decoding table
        static const std::unordered_map<char, int> base32_map;

};

#endif // TOTP_H
