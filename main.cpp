#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/fl_message.H>
#include <TotpTable.h>
#include <InputWindow.h>
#include <Timer.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <Entry.h>
#include <FileHandler.h>

Fl_Window *window;
TotpTable *table;
Fl_Dial *dial;
Fl_Button *addButton;
Fl_Button *hideButton;
Fl_Button *exitButton;
Fl_Check_Button *topBox;
static TOTP* totpGen;

bool visible = true;
std::string saveFilePath = "/etc/totpfltk/keys";
// csv file format
// issuer,secret,digits,timestep


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

// Countdown callback function
void countdown_cb(void* data) {
    Fl_Dial* dial = (Fl_Dial*)data;
    int value = (int)dial->value();
    if (value > 0) {
        dial->value(value - 1);
        //Timer::get_countdown();
    } else {
        dial->value(Timer::get_countdown()); // Reset to 30 seconds when it reaches 0
        for(int i = 0; i < table->get_size(); i++) {
            table->update_row(i, visible);
        }
    }
    Fl::repeat_timeout(1.0, countdown_cb, data);
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

int main(int argc, char **argv) {
    // Check user permissions
    if(geteuid() != 0) {
        fl_message_title("Permission Denied");
        fl_message("This application must be run as root (sudo).\n"
                   "Please restart the application with superuser privileges.");

        // Exit the application
        exit(EXIT_FAILURE);
    }

    window = new Fl_Window(400, 620, "TOTP FLTK");

    table = new TotpTable(20, 20, 360, 500);
    table->rows(0);           // 0 rows
    table->cols(2);           // 2 columns
    table->col_header(1);     // Enable column headers
    table->row_header(0);     // Enable row headers
    table->col_width_all(179);// Set column width to 100
    table->end();

    dial = new Fl_Dial(30, 540, 40, 40, "");
    dial->type(FL_FILL_DIAL);
    dial->angle1(0);
    dial->angle2(360);
    dial->minimum(0);
    dial->maximum(30);
    dial->value(Timer::get_countdown());
    Fl::add_timeout(1.0, countdown_cb, dial);

    // add button to add entry
    addButton = new Fl_Button(100, 545, 80, 30, "Add");
    addButton->type(FL_NORMAL_BUTTON);
    addButton->callback(show_input_window, table);

    // add button to show or hide entries
    hideButton = new Fl_Button(200, 545, 80, 30, "Hide");
    hideButton->type(FL_NORMAL_BUTTON);
    hideButton->callback(hide_callback);

    // add button to exit
    exitButton = new Fl_Button(300, 545, 80, 30, "Exit");
    exitButton->type(FL_NORMAL_BUTTON);
    exitButton->callback(exit_callback);

    // add check box to check for always on top
    topBox = new Fl_Check_Button(100, 580, 80, 30, "Stay on top");
    topBox->value(0);
    topBox->callback(on_top_toggle_callback, (void*)window);
    topBox->hide();

    // Attempt to read the file
    std::vector<Entry_d> fileContents;
    try {
        fileContents = FileHandler::readFile(saveFilePath);

        for (Entry_d row : fileContents) {
            //std::cout << row.issuer << " " << row.secret << " " << row.digits << " " << row.timestep << std::endl;
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
        //exit(EXIT_FAILURE);
    }

    window->end();
    window->show(argc, argv);
    return Fl::run();
}
