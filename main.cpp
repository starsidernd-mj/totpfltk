#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/fl_message.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Input.H>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>


#include <TotpTable.h>
#include <InputWindow.h>
#include <Timer.h>
#include <Entry.h>
#include <FileHandler.h>
#include <DeleteWindow.h>
#include <ModifyWindow.h>

#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <regex>

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

bool DEBUG = false;

Fl_Window *window;
TotpTable *table;
Fl_Dial *dial;
Fl_Button *addButton;
Fl_Button *hideButton;
Fl_Button *exitButton;
Fl_Button *importButton;
Fl_Button *exportButton;
Fl_Check_Button *topBox;
static TOTP* totpGen;
std::vector<Entry_d> fileContents;

bool visible = true;
std::string saveFilePath = "/etc/totpfltk/keys";
//std::string saveFilePath = "";
// csv file format
// issuer,secret,digits,timestep
const int AES_KEY_LENGTH = 256;  // AES key size in bits (256-bit AES)

void show_delete_window_callback(Fl_Widget* widget, void* data) {
    //std::cout << "Menu option selected: " << "Delete" << std::endl;
    TotpTable* table = (TotpTable*)data;
    DeleteWindow* delete_win = new DeleteWindow(230, 100, "Delete Entry", table->get_selected_row(), table);
    delete_win->set_table(table);
    delete_win->show();
}

void show_modify_window_callback(Fl_Widget* widget, void* data) {
    //std::cout << "Menu option selected: " << "Modify" << std::endl;
    TotpTable* table = (TotpTable*)data;
    ModifyWindow* modify_win = new ModifyWindow(400, 250, "Modify Entry", table->get_selected_row(), visible);
    modify_win->set_table(table);
    modify_win->populate_window();
    modify_win->show();
}

void show_input_window(Fl_Widget* widget, void* data) {
    TotpTable* table = (TotpTable*)data;
    InputWindow* input_win = new InputWindow(400, 250, "Add Entry", visible);
    input_win->set_table(table);
    input_win->show();
}

void hide_callback(Fl_Widget* widget, void* data) {
    Fl_Button* button = (Fl_Button*)widget;
    // Change the button's text
    const char* btn_current = button->label();
    if(btn_current == "Show") {
        button->label("Hide");
        visible = true;
        table->show_rows();
    } else if(btn_current == "Hide") {
        button->label("Show");
        visible = false;
        table->hide_rows();
    }
}

void exit_callback(Fl_Widget* widget, void* data) {
    exit(0);
}

void import_callback(Fl_Widget* widget, void* data) {
    Fl_Button* button = (Fl_Button*)widget;

}

void show_popup(std::string title, std::string msg) {
    fl_message_title(title.c_str());
    fl_message(msg.c_str());
}

bool aes_encrypt(const std::string &input_filename, const std::string &output_filename, const std::string &password) {
    // encrypt data using AES
    // Set up key and IV
    unsigned char key[AES_KEY_LENGTH/8];
    unsigned char iv[AES_BLOCK_SIZE];
    memset(key, 0x00, AES_KEY_LENGTH/8);
    memset(iv, 0x00, AES_BLOCK_SIZE);

    // Derive the key and IV from the password using a key derivation function
    if(!PKCS5_PBKDF2_HMAC_SHA1(password.c_str(), password.length(), NULL, 0, 10000, AES_KEY_LENGTH/8, key)) {
        if(!DEBUG) std::cerr << "Key derivation failed" << std::endl;
        show_popup("Key fail", "Key derivation failed");
        return false;
    }

    // open input and output files
    std::ifstream ifs(input_filename, std::ios::binary);
    std::ofstream ofs(output_filename, std::ios::binary);
    if(!ifs.is_open() || !ofs.is_open()) {
        if(!DEBUG) std::cerr << "File open failed" << std::endl;
        show_popup("File failure", "File open failed");
        return false;
    }

    // create encryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if(!ctx) {
        if(!DEBUG) std::cerr << "Encryption context creation failed" << std::endl;
        show_popup("Encryption failed", "Encryption context creation failed");
        return false;
    }

    // initialize AES encryption
    if(EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        if(!DEBUG) std::cerr << "AES encryption initialization failed" << std::endl;
        show_popup("AES failed", "AES encryption initialization failed");
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // encrypt the file
    const size_t buffer_size = 1024;
    unsigned char buffer[buffer_size];
    unsigned char cipher_buffer[buffer_size + AES_BLOCK_SIZE];
    int cipher_len = 0;

    while(ifs.eof()) {
        ifs.read(reinterpret_cast<char *>(buffer), buffer_size);
        int bytes_read = ifs.gcount();
        if(EVP_EncryptUpdate(ctx, cipher_buffer, &cipher_len, buffer, bytes_read) != 1) {
            if(!DEBUG) std::cerr << "AES encryption update failed" << std::endl;
            show_popup("AES update", "AES encryption update failed");
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        ofs.write(reinterpret_cast<char *>(cipher_buffer), cipher_len);
    }

    if(EVP_EncryptFinal_ex(ctx, cipher_buffer, &cipher_len) != 1) {
        if(!DEBUG) std::cerr << "AES encryption finalization failed" << std::endl;
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

bool is_password_strong(const std::string &password) {
    // password length should be between 8 and 32 characters
    if(password.length() < 8 || password.length() > 32) {
        return false;
    }

    // use regex to enforce password rules
    std::regex lowercase("[a-z]");
    std::regex uppercase("[A-Z]");
    std::regex digit("[0-9]");
    std::regex special("[^a-zA-Z0-9]"); // any symbol that's not a letter or digit

    // check if password contains at least one of each above
    if(!std::regex_search(password, lowercase) || !std::regex_search(password, uppercase) || !std::regex_search(password, digit) || !std::regex_search(password, special)) {
        return false;
    }
    return true;
}

void export_callback(Fl_Widget* widget, void* data) {
    Fl_Button* button = (Fl_Button*)widget;

    // create a modal window for password input
    Fl_Window *password_window = new Fl_Window(250, 150, "Enter password");

    // input fields for password and confirmation
    Fl_Input *password_input = new Fl_Input(100, 30, 120, 25, "Password:");
    password_input->type(FL_SECRET_INPUT);

    Fl_Input *confirm_input = new Fl_Input(100, 70, 120, 25, "Confirm:");
    confirm_input->type(FL_SECRET_INPUT);

    // button to submit the password
    Fl_Button *submit_button = new Fl_Button(80, 110, 90, 30, "Submit");

    // callback for when the user submits the password
    submit_button->callback([](Fl_Widget *w, void *data) {
        Fl_Input *password_input = (Fl_Input *)data;
        Fl_Input *confirm_input = (Fl_Input *)w->parent()->child(1);

        const std::string password = password_input->value();
        const std::string confirm_password = confirm_input->value();

        // check if both passwords match
        if(password == confirm_password && !password.empty()) {
            // check if the password is strong enough
            if(!is_password_strong(password)) {
                fl_alert("Password must be between 8 and 32 characters (lower and uppercase), digits and symbols");
                return;
            }
            // open file chooser to select the output file
            const char *output_filename = fl_file_chooser("Select output file", "*", "encrypted.dat");

            // check if the user selected a file
            if(output_filename) {
                // passwords match; now encrypt
                if(aes_encrypt(saveFilePath, output_filename, password)) {
                    if(!DEBUG) std::cout << "File encrypted successfully" << std::endl;
                    show_popup("File encryption", "File encrypted successfully");
                } else {
                    if(!DEBUG) std::cout << "File encryption failed" << std::endl;
                    show_popup("File encryption", "File encryption failed");
                }
                w->parent()->hide();
            } else {
                // user canceled file selection
                fl_alert("No output file selected");
            }
        } else {
            // passwords dont match; show error
            fl_alert("Passwords do not match or are empty, try again");
        }
    }, (void *)password_input);

    password_window->end();
    password_window->set_modal();
    password_window->show();
}

// Countdown callback function
void countdown_cb(void* data) {
    Fl_Dial* dial = (Fl_Dial*)data;
    char tooltip_text[50];
    int value = (int)Timer::get_countdown();
    dial->value(value);
    //std::cout << "Timer-- [" << value << "]" << std::endl;
    if (value > 0) {
        if(value == 30) {
            for(int i = 0; i < table->get_size(); i++) {
                table->update_row(i, visible);
            }
        }
        //change dial to red if <= 10 seconds left
        if(value <= 10) {
            dial->color2(FL_RED);
        } else {
            dial->color2(FL_DARK3);
        }
    }
    Fl::repeat_timeout(0.5, countdown_cb, data);
}

void set_window_always_on_top(Fl_Window* window, bool on_top) {
    Window xid = fl_xid(window);
    Display* disp = fl_display;
    Atom wm_state = XInternAtom(disp, "_NET_WM_STATE", False);
    Atom wm_state_above = XInternAtom(disp, "_NET_WM_STATE_ABOVE", False);
    if (on_top) {
        XChangeProperty(disp, xid, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wm_state_above, 1);
    } else {
        XDeleteProperty(disp, xid, wm_state);
    }
}

void on_top_toggle_callback(Fl_Widget* widget, void* data) {
    Fl_Check_Button* checkbox = (Fl_Check_Button*)widget;
    Fl_Window* window = (Fl_Window*)data;
    bool on_top = checkbox->value();
    set_window_always_on_top(window, on_top);
}

void relaunch_with_sudo(int argc, char* argv[]) {
    std::vector<const char*> args;
    args.push_back("sudo");
    for (int i = 0; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    args.push_back(nullptr);

    execvp("sudo", const_cast<char* const*>(args.data()));
    perror("execvp");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    // Check user permissions
    if(geteuid() != 0) {
        fl_message_title("Permission Denied");
        fl_message("This application must be run as root (sudo).");
        relaunch_with_sudo(argc, argv);
    }

    // get home directory
    /*saveFilePath = getenv("HOME");
    std::cout << saveFilePath << std::endl;
    if(saveFilePath.empty()) {
        std::cerr << "Could not get home directory" << std::endl;
        fl_message_title("Error");
        fl_message("Could not get home directory");
        return -1;
    }*/

    window = new Fl_Window(400, 620, "LinTOTP");

    table = new TotpTable(20, 20, 360, 500);
    table->rows(0);           // 0 rows
    table->cols(2);           // 2 columns
    table->col_header(1);     // Enable column headers
    table->row_header(0);     // Enable row headers
    table->col_width_all(179);// Set column width to 100
    table->get_menu_button()->add("Modify", 0, show_modify_window_callback, table);
    table->get_menu_button()->add("Delete", 0, show_delete_window_callback, table);
    table->end();

    dial = new Fl_Dial(30, 540, 40, 40, "");
    dial->type(FL_FILL_DIAL);
    dial->angle1(0);
    dial->angle2(360);
    dial->minimum(0);
    dial->maximum(30);
    dial->value(Timer::get_countdown());
    Fl::add_timeout(0.5, countdown_cb, dial);

    // add button to add entry
    addButton = new Fl_Button(100, 545, 80, 30, "Add");
    addButton->type(FL_NORMAL_BUTTON);
    addButton->tooltip("Add key");
    addButton->callback(show_input_window, table);

    // add button to show or hide entries
    hideButton = new Fl_Button(200, 545, 80, 30, "Hide");
    hideButton->type(FL_NORMAL_BUTTON);
    hideButton->tooltip("Hide secret keys");
    hideButton->callback(hide_callback);

    // add button to exit
    exitButton = new Fl_Button(300, 545, 80, 30, "Exit");
    exitButton->type(FL_NORMAL_BUTTON);
    exitButton->callback(exit_callback);

    // add check box to check for always on top
    topBox = new Fl_Check_Button(200, 580, 80, 30, "Stay on top");
    topBox->value(0);
    topBox->callback(on_top_toggle_callback, (void*)window);
    topBox->hide();

    // Load the icon images (e.g., a down arrow or import icon)
    Fl_PNG_Image *import_icon = new Fl_PNG_Image("assets/import_icon_s.png");
    Fl_PNG_Image *export_icon = new Fl_PNG_Image("assets/export_icon_s.png");

    // add import button and set it to be a down arrow
    importButton = new Fl_Button(100, 580, 30, 30);
    importButton->image(import_icon);
    importButton->type(FL_NORMAL_BUTTON);
    importButton->tooltip("Import keys");
    importButton->callback(import_callback);

    // add export button and set it to be an up arrow
    exportButton = new Fl_Button(150, 580, 30, 30);
    exportButton->image(export_icon);
    exportButton->type(FL_NORMAL_BUTTON);
    exportButton->tooltip("Export keys");
    exportButton->callback(export_callback);



    // Attempt to read the file
    try {
        fileContents = FileHandler::readFile(saveFilePath);

        for (Entry_d row : fileContents) {
            uint64_t time_step = std::stoi(row.timestep);
            size_t digits = std::stoi(row.digits);
            std::string tmp = totpGen->generateTOTP(row.secret, Timer::get_time(time_step), digits);

            std::vector<std::string> new_row = { row.issuer, visible ? tmp : "******" };
            std::vector<std::string> secret_key = { tmp, row.secret, row.digits, row.timestep };
            table->add_row(new_row, secret_key);
        }
    } catch (const std::exception& e) {
        fl_message_title("Error");
        fl_message(e.what());
    }

    window->end();
    window->show(argc, argv);
    return Fl::run();
}
