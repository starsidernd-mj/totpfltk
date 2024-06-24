#include "DeleteWindow.h"

DeleteWindow::DeleteWindow(int W, int H, const char* L, int selected_row, TotpTable* tb) : Fl_Window(W, H, L) {
    /*std::vector<std::vector<std::string>> tmp = table->get_visible_column_data();
    int selrow = table->get_selected_row();
    const std::string tmpstring = tmp[selrow][0];
    const char* label = tmpstring.c_str();*/
    this->selected_row = selected_row;
    this->table = tb;

    ok_btn = new Fl_Button(10, 60, 100, 30, "OK");
    ok_btn->callback(confirm_delete_callback, this);

    cancel_btn = new Fl_Button(120, 60, 100, 30, "Cancel");
    cancel_btn->callback(cancel_delete_callback, this);

    end();
    show();
}

DeleteWindow::~DeleteWindow()
{
    //dtor
}

void DeleteWindow::set_table(TotpTable* t) {
    table = t;
}

TotpTable* DeleteWindow::get_table() {
    return table;
}

void DeleteWindow::set_selected_row(int row) {
    selected_row = row;
}

void DeleteWindow::confirm_delete_callback(Fl_Widget* widget, void* data) {
    DeleteWindow* win = (DeleteWindow*)data;
    TotpTable* tb = (TotpTable*)win->table;

    int idx = win->get_selected_row();
    //std::vector<std::vector<std::string>> tb_vis_data = tb->get_visible_column_data();
    //std::vector<std::vector<std::string>> tb_hid_data = tb->get_hidden_column_data();

    //std::vector<std::string> tmp = tb_vis_data.at(idx);
    //std::cout << "Removing row: " << idx << ", '" << tmp[0] << "'" << std::endl;

    // do delete logic here
    tb->remove_row(idx);

    // call filehandler and modify file
    FileHandler::modify_file(idx, "/etc/totpfltk/keys");

    win->hide();
}

void DeleteWindow::cancel_delete_callback(Fl_Widget* widget, void* data) {
    Fl_Window* win = (Fl_Window*)data;
    win->hide();
}

int DeleteWindow::get_selected_row() {
    return selected_row;
}
