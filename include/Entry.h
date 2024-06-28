#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <unordered_map>
#include <algorithm>

struct Entry_d {
    std::string issuer;
    std::string secret;
    std::string digits;
    std::string timestep;
};

static std::unordered_map<char, int> base32_map = {
    {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7},
    {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15},
    {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23},
    {'Y', 24}, {'Z', 25}, {'2', 26}, {'3', 27}, {'4', 28}, {'5', 29}, {'6', 30}, {'7', 31}
};

static std::string strip_invalid_characters(const std::string& input) {
    std::string cleaned;
    std::string input_proper = input;
    std::transform(input_proper.begin(), input_proper.end(), input_proper.begin(), ::toupper);
    for (char c : input_proper) {
        if (base32_map.find(c) != base32_map.end()) {
            cleaned.push_back(c);
        }
    }
    return cleaned;
}

#endif // ENTRY_H
