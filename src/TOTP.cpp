#include "TOTP.h"

std::string TOTP::generateTOTP(const std::string& key, uint64_t counter, size_t digits) {
    //uint8_t *digest;

    // Prepare data for HMAC
    uint32_t endianness;

    endianness = 0xdeadbeef;
    if ((*(const uint8_t *)&endianness) == 0xef) {
        counter = ((counter & 0x00000000ffffffff) << 32) | ((counter & 0xffffffff00000000) >> 32);
        counter = ((counter & 0x0000ffff0000ffff) << 16) | ((counter & 0xffff0000ffff0000) >> 16);
        counter = ((counter & 0x00ff00ff00ff00ff) <<  8) | ((counter & 0xff00ff00ff00ff00) >>  8);
    };

    // Decode the key to be base32 compliant
    std::string base32_key = base32_decode(key.c_str());

    // Compute HMAC-SHA1
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    HMAC(EVP_sha1(), base32_key.c_str(), base32_key.length(), (const unsigned char *)&counter, sizeof(counter), hash, &hash_len);

    // Get last nibble of the digest to determine the starting offset
    size_t offset = hash[hash_len - 1] & 0xF;

    // Extract 4 bytes from the hash, starting from the determined offset
    uint32_t truncatedHash = (hash[offset] & 0x7F) << 24 | (hash[offset + 1] & 0xFF) << 16 | (hash[offset + 2] & 0xFF) << 8 | (hash[offset + 3] & 0xFF);

    // Apply modulo to get a number that fits in 'digits' decimal places
    truncatedHash %= static_cast<uint32_t>(std::pow(10, digits));

    // Convert to string with leading zeros if necessary
    std::ostringstream result;
    result << std::setw(digits) << std::setfill('0') << truncatedHash;

    //std::cout << "\nGenerateTOTP\nCounter: " << counter << "\nKey: " << base32_key << "\nKey Len: " << base32_key.length() << "\nResult: " << result.str() << "\n--------------------------\n" << std::endl;

    return result.str();
}

std::string TOTP::base32_encode(const std::string& input) {
    std::string output;
    int buffer = 0, bits_left = 0;
    const char base32_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

    for (char c : input) {
        buffer <<= 8;
        buffer |= (c & 0xff);
        bits_left += 8;

        while (bits_left >= 5) {
            output.push_back(base32_chars[(buffer >> (bits_left - 5)) & 0x1f]);
            bits_left -= 5;
        }
    }

    if (bits_left > 0) {
        buffer <<= (5 - bits_left);
        output.push_back(base32_chars[buffer & 0x1f]);
    }

    return output;
}

std::string TOTP::base32_decode(const std::string& input) {
    std::string output;
    int buffer = 0, bits_left = 0;

    // Base32 decoding table
    const std::unordered_map<char, int> base32_map = {
        {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7},
        {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15},
        {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23},
        {'Y', 24}, {'Z', 25}, {'2', 26}, {'3', 27}, {'4', 28}, {'5', 29}, {'6', 30}, {'7', 31}
    };

    for (char c : input) {
        if (base32_map.find(c) == base32_map.end()) {
            throw std::invalid_argument("Invalid character in Base32 string");
        }

        buffer <<= 5;
        buffer |= base32_map.at(c) & 0x1f;
        bits_left += 5;

        if (bits_left >= 8) {
            output.push_back(static_cast<char>((buffer >> (bits_left - 8)) & 0xff));
            bits_left -= 8;
        }
    }

    return output;
}

size_t TOTP::decode_b32key(uint8_t **k, size_t len) {

    const int8_t base32_vals[256] = {
        //    This map cheats and interprets:
        //       - the numeral zero as the letter "O" as in oscar
        //       - the numeral one as the letter "L" as in lima
        //       - the numeral eight as the letter "B" as in bravo
        // 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x00
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x10
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x20
        14, 11, 26, 27, 28, 29, 30, 31,  1, -1, -1, -1, -1,  0, -1, -1, // 0x30
        -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, // 0x40
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 0x50
        -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, // 0x60
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, -1, -1, -1, -1, // 0x70
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x80
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x90
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xA0
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xB0
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xC0
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xD0
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xE0
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xF0
    };

    size_t keylen;
    size_t pos;
    // decodes base32 secret key
    keylen = 0;
    for (pos = 0; pos <= (len - 8); pos += 8) {
    // MSB is Most Significant Bits  (0x80 == 10000000 ~= MSB)
    // MB is middle bits             (0x7E == 01111110 ~= MB)
    // LSB is Least Significant Bits (0x01 == 00000001 ~= LSB)

    // byte 0
    (*k)[keylen+0]  = (base32_vals[(*k)[pos+0]] << 3) & 0xF8; // 5 MSB
    (*k)[keylen+0] |= (base32_vals[(*k)[pos+1]] >> 2) & 0x07; // 3 LSB
    if ((*k)[pos+2] == '=') {
        keylen += 1;
        break;
    }

    // byte 1
    (*k)[keylen+1]  = (base32_vals[(*k)[pos+1]] << 6) & 0xC0; // 2 MSB
    (*k)[keylen+1] |= (base32_vals[(*k)[pos+2]] << 1) & 0x3E; // 5  MB
    (*k)[keylen+1] |= (base32_vals[(*k)[pos+3]] >> 4) & 0x01; // 1 LSB
    if ((*k)[pos+4] == '=') {
        keylen += 2;
        break;
    }

    // byte 2
    (*k)[keylen+2]  = (base32_vals[(*k)[pos+3]] << 4) & 0xF0; // 4 MSB
    (*k)[keylen+2] |= (base32_vals[(*k)[pos+4]] >> 1) & 0x0F; // 4 LSB
    if ((*k)[pos+5] == '=') {
        keylen += 3;
        break;
    }

    // byte 3
    (*k)[keylen+3]  = (base32_vals[(*k)[pos+4]] << 7) & 0x80; // 1 MSB
    (*k)[keylen+3] |= (base32_vals[(*k)[pos+5]] << 2) & 0x7C; // 5  MB
    (*k)[keylen+3] |= (base32_vals[(*k)[pos+6]] >> 3) & 0x03; // 2 LSB
    if ((*k)[pos+7] == '=') {
        keylen += 4;
        break;
    }

    // byte 4
    (*k)[keylen+4]  = (base32_vals[(*k)[pos+6]] << 5) & 0xE0; // 3 MSB
    (*k)[keylen+4] |= (base32_vals[(*k)[pos+7]] >> 0) & 0x1F; // 5 LSB
    keylen += 5;
    }
    (*k)[keylen] = 0;

    return keylen;
}
