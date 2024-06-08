#ifndef ENTRY_H
#define ENTRY_H

#include <string>

struct Entry_d {
    std::string issuer;
    std::string secret;
    std::string digits;
    std::string timestep;
};

#endif // ENTRY_H
