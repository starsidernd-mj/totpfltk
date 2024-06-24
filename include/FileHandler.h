#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <Entry.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class FileHandler
{
    public:
        FileHandler();
        virtual ~FileHandler();
        static void write_file(Entry_d entry, const std::string& filepath="/etc/totpfltk/keys");
        static void update_file(Entry_d entry, int row, const std::string& filepath="/etc/totpfltk/keys");
        static std::vector<Entry_d> readFile(const std::string& filepath="/etc/totpfltk/keys");
        static void remove_entry_file(int row, const std::string& filepath="/etc/totpfltk/keys");

    protected:

    private:
        std::string saveFilePath;
};

#endif // FILEHANDLER_H
