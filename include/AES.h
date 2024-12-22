#ifndef AES_H
#define AES_H

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <regex>
#include <cstring>
#include <iostream>

#include <FL/Fl_Input.H>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_message.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>

class AES
{
    public:
        AES();
        virtual ~AES();

        //const int AES_BLOCK_SIZE = 16;
        const int AES_KEY_SIZE = 32;
        const int AES_KEY_LENGTH = 256;  // AES key size in bits (256-bit AES)
        const bool DEBUG = false;

        unsigned char* key;
        unsigned char* iv;

        void show_popup(std::string title, std::string msg);
        void aes_init();
        bool aes_encrypt(const std::string &input_filename, const std::string &output_filename, const std::string &password);
        bool aes_decrypt(const std::string &input_filename, const std::string &output_filename, const std::string &password);
        std::vector<unsigned char> to_vector(std::string const& str);

    protected:

    private:

};

#endif
