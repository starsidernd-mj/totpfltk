#include "InputWindow.h"

InputWindow::~InputWindow()
{
    //dtor
}

InputWindow::InputWindow(int W, int H, const char* L, bool visible) : Fl_Window(W, H, L) {
    visibility = visible;
    col1_input = new Fl_Input(120, 20, 200, 30, "Issuer:");
    col2_input = new Fl_Input(120, 60, 200, 30, "Secret Key:");
    col3_input = new Fl_Spinner(120, 100, 200, 30, "Digits:");
    col3_input->type(FL_INT_INPUT);
    col3_input->step(1);
    col3_input->value(6);
    col4_input = new Fl_Spinner(120, 140, 200, 30, "Time Step:");
    col4_input->type(FL_INT_INPUT);
    col4_input->step(1);
    col4_input->value(30);
    add_button = new Fl_Button(120, 200, 100, 30, "Add");
    add_button->callback(add_callback, this);
    end();
}

void InputWindow::add_callback(Fl_Widget* widget, void* data) {
    InputWindow* win = (InputWindow*)data;
    win->entry = {win->col1_input->value(), win->col2_input->value(), std::to_string((int)(win->col3_input->value())), std::to_string((int)(win->col4_input->value()))};

    // call filehandler and save to file
    FileHandler::write_file(win->entry, "/etc/totpfltk/keys");

    //generate new TOTP data here to add to table
    uint64_t time_step = (int)(win->col4_input->value());
    size_t digits = (int)(win->col3_input->value());
    std::string tmp = totpGen->generateTOTP(win->col2_input->value(), Timer::get_time(time_step), digits);

    std::vector<std::string> new_row = { win->col1_input->value(), win->visibility ? tmp : "******" };
    std::vector<std::string> secret_key = { tmp, win->col2_input->value(), std::to_string(win->col3_input->value()), std::to_string(win->col4_input->value()) };
    TotpTable* table = (TotpTable*)win->table;
    table->add_row(new_row, secret_key);
    win->hide();
}

void InputWindow::set_table(TotpTable* t) {
    table = t;
}
