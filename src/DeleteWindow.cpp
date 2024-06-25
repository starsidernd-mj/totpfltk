#include "DeleteWindow.h"

DeleteWindow::DeleteWindow(int W, int H, const char* L, int selected_row, TotpTable* tb) : Fl_Window(W, H, L) {
    this->selected_row = selected_row;
    this->table = tb;

    std::vector<std::vector<std::string>> visible_column_data = tb->get_visible_column_data();
    std::string issuer = visible_column_data.at(selected_row)[0];
    //std::cout << "Issuer: " << issuer.c_str() << std::endl;

    char shortBuffer[100];
    strncpy(shortBuffer, issuer.c_str(), 99);
    shortBuffer[99] = '\0';  // Ensure null-termination

    this->label(shortBuffer);

    //item_label = new Fl_Box(10, 20, 200, 40);
    //item_label->type(FL_NO_BOX);
    //item_label->label(shortBuffer);

    ok_btn = new Fl_Button(10, 60, 100, 30, "OK");
    ok_btn->callback(confirm_delete_callback, this);

    cancel_btn = new Fl_Button(120, 60, 100, 30, "Cancel");
    cancel_btn->callback(cancel_delete_callback, this);

    end();
}

DeleteWindow::~DeleteWindow()
{
    //dtor
    free(table);
    free(ok_btn);
    free(cancel_btn);
    free(item_label);
    free(win);
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

    // do delete logic here
    tb->remove_row(idx);

    // call filehandler and modify file
    FileHandler::remove_entry_file(idx, "/etc/totpfltk/keys");

    win->hide();
}

void DeleteWindow::cancel_delete_callback(Fl_Widget* widget, void* data) {
    Fl_Window* win = (Fl_Window*)data;
    win->hide();
}

int DeleteWindow::get_selected_row() {
    return selected_row;
}
