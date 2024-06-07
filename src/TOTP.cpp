#include "TOTP.h"

std::string TOTP::generateTOTP(const std::string& key, uint64_t counter, size_t digits) {
    std::cout << key << " " << counter << " " << digits << std::endl;
    // Prepare data for HMAC
    std::string data(reinterpret_cast<char*>(&counter), sizeof(counter));

    // Compute HMAC-SHA1
    unsigned char hash[SHA_DIGEST_LENGTH];
    unsigned int mdLen[10];
    //HMAC(EVP_sha1(), key.c_str(), key.size(), reinterpret_cast<unsigned char*>(data.data()), data.size(), hash, mdLen);
    HMAC(EVP_sha1(), key.c_str(), key.size(), reinterpret_cast<const unsigned char*>(data.data()), data.size(), hash, nullptr);

    // Get last nibble of the digest to determine the starting offset
    size_t offset = hash[SHA_DIGEST_LENGTH - 1] & 0xF;

    // Extract 4 bytes from the hash, starting from the determined offset
    uint32_t truncatedHash = (hash[offset] & 0x7F) << 24 | (hash[offset + 1] & 0xFF) << 16 | (hash[offset + 2] & 0xFF) << 8 | (hash[offset + 3] & 0xFF);

    // Apply modulo to get a number that fits in 'digits' decimal places
    truncatedHash %= static_cast<uint32_t>(std::pow(10, digits));

    // Convert to string with leading zeros if necessary
    std::ostringstream result;
    result << std::setw(digits) << std::setfill('0') << truncatedHash;

    return result.str();
}
