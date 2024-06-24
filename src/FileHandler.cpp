#include "FileHandler.h"

FileHandler::FileHandler()
{
    //ctor
    saveFilePath = "/etc/totpfltk/keys";
}

FileHandler::~FileHandler()
{
    //dtor
}

void FileHandler::write_file(Entry_d entry, const std::string& filepath) {
    // Open the file in append mode
    std::ofstream file;
    file.open(filepath, std::ios::app);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filepath);
    }
    file << entry.issuer << "," << entry.secret << "," << entry.digits << "," << entry.timestep << "\n";

    file.close();
}

void FileHandler::update_file(Entry_d entry, int row, const std::string& filepath) {
    // Open the file in read mode
    std::ifstream ifile;
    ifile.open(filepath);
    if(!ifile.is_open()) {
        throw std::runtime_error("Failed to open file for reading: " + filepath);
    }

    // Open temp file for writing to. Will rename this to filepath
    std::ofstream tfile;
    std::string tmpFilePath = filepath + "_tmp";
    tfile.open(tmpFilePath, std::ios::app);
    if(!tfile.is_open()) {
        throw std::runtime_error("Failed to open tfile for writing: " + tmpFilePath);
    }

    std::string line;
    int current_line = 0;

    while(getline(ifile, line)) {
        if(current_line != row) {
            tfile << line << std::endl;
        } else {
            tfile << entry.issuer << "," << entry.secret << "," << entry.digits << "," << entry.timestep << "\n";
        }
        ++current_line;
    }

    ifile.close();
    tfile.close();

    if(remove(filepath.c_str()) != 0) {
        throw std::runtime_error("Error deleting original file: " + filepath);
    }

    if(rename(tmpFilePath.c_str(), filepath.c_str()) != 0) {
        throw std::runtime_error("Error renaming temp file: " + tmpFilePath);
    }
}

std::vector<Entry_d> FileHandler::readFile(const std::string& filepath) {
    std::vector<Entry_d> entryData;
    std::string line;
    std::string token;

    // Open the file in read mode
    std::ifstream file;
    file.open(filepath);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    while (std::getline(buffer, line)) {
        std::stringstream tokenBuffer(line);
        std::vector<std::string> tokens;
        while(std::getline(tokenBuffer, token, ',')) {
            tokens.push_back(token);
        }
        Entry_d entry = {tokens[0], tokens[1], tokens[2], tokens[3]};
        entryData.push_back(entry);
    }
    file.close();

    return entryData;
}

void FileHandler::remove_entry_file(int row, const std::string& filepath) {
    // Open the file in read mode
    std::ifstream ifile;
    ifile.open(filepath);
    if(!ifile.is_open()) {
        throw std::runtime_error("Failed to open file for reading: " + filepath);
    }

    // Open temp file for writing to. Will rename this to filepath
    std::ofstream tfile;
    std::string tmpFilePath = filepath + "_tmp";
    tfile.open(tmpFilePath, std::ios::app);
    if(!tfile.is_open()) {
        throw std::runtime_error("Failed to open tfile for writing: " + tmpFilePath);
    }

    std::string line;
    int current_line = 0;

    while(getline(ifile, line)) {
        if(current_line != row) {
            tfile << line << std::endl;
        }
        ++current_line;
    }

    ifile.close();
    tfile.close();

    if(remove(filepath.c_str()) != 0) {
        throw std::runtime_error("Error deleting original file: " + filepath);
    }

    if(rename(tmpFilePath.c_str(), filepath.c_str()) != 0) {
        throw std::runtime_error("Error renaming temp file: " + tmpFilePath);
    }
}
