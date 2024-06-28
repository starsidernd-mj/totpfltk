#include "TOTP.h"

std::string TOTP::generateTOTP(const std::string& key, uint64_t counter, size_t digits) {
    // Prepare data for HMAC
    uint32_t endianness;
    std::string key_proper = key;

    endianness = 0xdeadbeef;
    if ((*(const uint8_t *)&endianness) == 0xef) {
        counter = ((counter & 0x00000000ffffffff) << 32) | ((counter & 0xffffffff00000000) >> 32);
        counter = ((counter & 0x0000ffff0000ffff) << 16) | ((counter & 0xffff0000ffff0000) >> 16);
        counter = ((counter & 0x00ff00ff00ff00ff) <<  8) | ((counter & 0xff00ff00ff00ff00) >>  8);
    };

    // Decode the key to be base32 compliant
    std::transform(key_proper.begin(), key_proper.end(), key_proper.begin(), ::toupper);
    std::string base32_key = base32_decode(key_proper.c_str());

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

    return result.str();
}

std::string TOTP::base32_decode(const std::string& input) {
    std::string output;
    int buffer = 0, bits_left = 0;

    for (char c : input) {
        if (base32_map.find(c) == base32_map.end()) {
            continue;
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
