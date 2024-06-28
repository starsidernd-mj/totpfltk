#include "ModifyWindow.h"

ModifyWindow::ModifyWindow(int W, int H, const char* L, int selected_row, bool visible) : Fl_Window(W, H, L) {
    this->visibility = visible;
    this->selected_row = selected_row;
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
    update_button = new Fl_Button(120, 200, 100, 30, "Update");
    update_button->callback(modify_callback, this);
    cancel_button = new Fl_Button(240, 200, 100, 30, "Cancel");
    cancel_button->callback(cancel_modify_callback, this);
    end();
}

ModifyWindow::~ModifyWindow()
{
    //dtor
    free(col1_input);
    free(col2_input);
    free(col3_input);
    free(col4_input);
    free(update_button);
    free(cancel_button);
    free(table);
}

void ModifyWindow::set_table(TotpTable* t) {
    table = t;
}

void ModifyWindow::populate_window() {
    std::vector<std::vector<std::string>> hidden_column_data = table->get_hidden_column_data();
    std::vector<std::vector<std::string>> visible_column_data = table->get_visible_column_data();

    //col0 is key, col1 is secret, col2 is #of digits, col3 is timestep
    std::string issuer = visible_column_data.at(selected_row)[0];
    std::string secret = hidden_column_data.at(selected_row)[1];
    std::string digits = hidden_column_data.at(selected_row)[2];
    std::string tstep = hidden_column_data.at(selected_row)[3];

    col1_input->value(issuer.c_str());
    col2_input->value(secret.c_str());
    col3_input->value(std::stoi(digits));
    col4_input->value(std::stoi(tstep));
}

void ModifyWindow::modify_callback(Fl_Widget* widget, void* data) {
    ModifyWindow* win = (ModifyWindow*)data;
    TotpTable* tb = (TotpTable*)win->table;
    int idx = win->get_selected_row();

    win->entry = {
        win->col1_input->value(),
        strip_invalid_characters(win->col2_input->value()),
        std::to_string((int)(win->col3_input->value())),
        std::to_string((int)(win->col4_input->value()))
    };

    //generate new TOTP data here to add to table
    uint64_t time_step = (int)(win->col4_input->value());
    size_t digits = (int)(win->col3_input->value());
    std::string tmp = totpGen->generateTOTP(win->entry.secret, Timer::get_time(time_step), digits);


    std::vector<std::string> vis_row = { win->col1_input->value(), win->visibility ? tmp : "******" };
    std::vector<std::string> hid_row = { tmp, win->entry.secret, std::to_string(win->col3_input->value()), std::to_string(win->col4_input->value()) };

    // do update logic here
    tb->modify_row(idx, vis_row, hid_row);

    // call filehandler and modify file
    FileHandler::update_file(win->entry, idx, "/etc/totpfltk/keys");

    win->hide();
}

void ModifyWindow::cancel_modify_callback(Fl_Widget* widget, void* data) {
    Fl_Window* win = (Fl_Window*)data;
    win->hide();
}

int ModifyWindow::get_selected_row() {
    return selected_row;
}
