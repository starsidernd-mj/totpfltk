#include "AES.h"

AES::AES() {}

AES::~AES() {}

void AES::show_popup(std::string title, std::string msg) {
    fl_message_title(title.c_str());
    fl_message(msg.c_str());
    if(!DEBUG) std::cerr << msg.c_str() << std::endl;
}

void AES::aes_init() {
    key = (unsigned char*)malloc(AES_KEY_LENGTH/8);
    iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    memset(key, 0x00, AES_KEY_LENGTH/8);
    memset(iv, 0x00, AES_BLOCK_SIZE);
}

bool AES::aes_encrypt(const std::string &input_filename, const std::string &output_filename, const std::string &password) {
    // encrypt data using AES
    int enc_length;

    // Derive the key and IV from the password using a key derivation function
    if(!PKCS5_PBKDF2_HMAC_SHA1(password.c_str(), password.length(), NULL, 0, 10000, AES_KEY_LENGTH/8, key)) {
        show_popup("Key fail", "Key derivation failed");
        return false;
    }

    // open input and output files
    std::ifstream ifs(input_filename, std::ios::binary);
    if(!ifs.is_open()) {
        show_popup("File input failure", "File open failed");
        return false;
    }

    std::ofstream ofs(output_filename, std::ios::binary);
    if(!ofs.is_open()) {
        show_popup("File output failure", "File open failed");
        return false;
    }

    // create encryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if(!ctx) {
        show_popup("Encryption failed", "Encryption context creation failed");
        return false;
    }

    // initialize AES encryption
    if(EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        show_popup("AES failed", "AES encryption initialization failed");
        EVP_CIPHER_CTX_cleanup(ctx);
        return false;
    }

    // encrypt the file
    //const size_t buffer_size = 1024;
    //unsigned char buffer[buffer_size];
    //unsigned char cipher_buffer[buffer_size + AES_BLOCK_SIZE];
    unsigned char* cipher_buffer;
    int cipher_len = 0;
    ifs.seekg(0);

    while(!ifs.eof()) {
        std::string line;
        size_t byte_cnt;

        getline(ifs, line);
        std::cout << "Read from file: " << line << std::endl;
        byte_cnt = line.size();

        unsigned char* uchar_line = new unsigned char[line.size() + 1];
        std::copy(line.begin(), line.end(), uchar_line);
        uchar_line[line.size()] = '\0';

        /*cipher_buffer = (unsigned char*)malloc(sizeof(uchar_line)+AES_BLOCK_SIZE);

        if(!EVP_EncryptUpdate(ctx, cipher_buffer, &cipher_len, uchar_line, byte_cnt)) {
            show_popup("AES update", "AES encryption update failed");
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        ofs.write(reinterpret_cast<char *>(cipher_buffer), cipher_len);*/
    }

    if(EVP_EncryptFinal_ex(ctx, cipher_buffer, &cipher_len) != 1) {
        show_popup("AES final", "AES encryption finalization failed");
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    ofs.write(reinterpret_cast<char *>(cipher_buffer), cipher_len);

    // clean up
    EVP_CIPHER_CTX_free(ctx);
    ifs.close();
    ofs.close();
    return true;
}

bool AES::aes_decrypt(const std::string &input_filename, const std::string &output_filename, const std::string &password) {
    // AES decryption function
    // Set up key and IV
    unsigned char key[AES_KEY_LENGTH/8];
    unsigned char iv[AES_BLOCK_SIZE*2];
    memset(key, 0x00, AES_KEY_LENGTH/8);
    memset(iv, 0x00, AES_BLOCK_SIZE*2);

    // Derive the key and IV from the password using a key derivation function
    if(!PKCS5_PBKDF2_HMAC_SHA1(password.c_str(), password.length(), NULL, 0, 10000, AES_KEY_LENGTH/8, key)) {
        show_popup("Key fail", "Key derivation failed");
        return false;
    }

    // open input and output files
    std::ifstream ifs(input_filename, std::ios::binary);
    std::ofstream ofs(output_filename, std::ios::binary);
    if(!ifs.is_open() || !ofs.is_open()) {
        show_popup("File failure", "File open failed");
        return false;
    }

    // create decryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if(!ctx) {
        show_popup("Decryption failed", "Decryption context creation failed");
        return false;
    }

    // initialize AES decryption (AES-256-CBC)
    if(EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        show_popup("AES failed", "AES decryption initialization failed");
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // decrypt the file
    const size_t buffer_size = 1024;
    unsigned char buffer[buffer_size];
    unsigned char plain_buffer[buffer_size + AES_BLOCK_SIZE];
    int plain_len = 0;

    while(!ifs.eof()) {
        ifs.read(reinterpret_cast<char *>(buffer), buffer_size);
        int bytes_read = ifs.gcount();
        std::cout << "Bytes read: " << bytes_read << std::endl;
        if(EVP_DecryptUpdate(ctx, plain_buffer, &plain_len, buffer, bytes_read) != 1) {
            show_popup("AES failed", "AES decryption finalization failed. Wrong password or corrupted file.");
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        ofs.write(reinterpret_cast<char *>(plain_buffer), plain_len);
    }

    if(EVP_DecryptFinal_ex(ctx, plain_buffer, &plain_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        //test
        unsigned long err = ERR_get_error();
        char err_msg[120];
        ERR_error_string_n(err, err_msg, sizeof(err_msg));
        std::cerr << "error: " << err_msg << std::endl;
        //end test
        show_popup("Decrypt failed", "Decryption finalization failed. Wrong password or corrupted file.");
        return false;
    }
    ofs.write(reinterpret_cast<char *>(plain_buffer), plain_len);

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

std::vector<unsigned char> AES::to_vector(std::string const& str) {
    return std::vector<unsigned char>(str.data(), str.data() + str.length() + 1);
}
