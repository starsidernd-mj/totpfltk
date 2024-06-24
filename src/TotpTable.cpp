#include "TotpTable.h"


TotpTable::~TotpTable()
{
    //dtor
}

TotpTable::TotpTable(int X, int Y, int W, int H, const char* L)
    : Fl_Table_Row(X, Y, W, H, L),
    menu_button(new Fl_Menu_Button(0, 0, 0, 0)),
    hidden_column_data({}),
    selected_row(-1) {
    menu_button->type(Fl_Menu_Button::POPUP3);
    menu_button->add("Modify", 0, menu_callback, (void*)"Modify");
    //menu_button->add("Delete", 0, show_delete_window_callback, (void*)"Delete");
    //menu_button->add("Delete", 0, show_delete_window_callback, this);

    rows(0); // Initialize with 0 rows
    cols(2); // 2 columns
    end(); // Close the group
}

void TotpTable::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) {
    static char s[40];
    switch (context) {
        case CONTEXT_COL_HEADER:
            fl_push_clip(X, Y, W, H);
            fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, color());
            fl_color(FL_BLACK);
            if (C == 0) strcpy(s, "Issuer");
            else if (C == 1) strcpy(s, "Key");
            else strcpy(s, "Secret");
            fl_draw(s, X+6, Y, W, H, FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
            fl_pop_clip();
            return;
        case CONTEXT_ROW_HEADER:
            fl_push_clip(X, Y, W, H);
            fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, color());
            fl_color(FL_BLACK);
            sprintf(s, "%d", R + 1);
            fl_draw(s, X, Y, W, H, FL_ALIGN_LEFT);
            fl_pop_clip();
            return;
        case CONTEXT_CELL:
            fl_push_clip(X, Y, W, H);
            fl_color(FL_WHITE);
            fl_rectf(X, Y, W, H);
            if (R == selected_row) {
                fl_color(FL_YELLOW);
                fl_rectf(X, Y, W, H);
            }
            fl_color(FL_GRAY0);
            fl_rect(X, Y, W, H);
            fl_color(FL_BLACK);
            if (R < visible_column_data.size() && C < visible_column_data[R].size()) {
                if (C == 0) strcpy(s, visible_column_data[R][C].c_str());
                else if (C == 1) strcpy(s, visible_column_data[R][C].c_str());
                else if (C == 2) strcpy(s, hidden_column_data[R][C].c_str());
                else if (C == 3) strcpy(s, hidden_column_data[R][C].c_str());
                else if (C == 4) strcpy(s, hidden_column_data[R][C].c_str());
                else strcpy(s, hidden_column_data[R][C].c_str()); // Display the hidden column data
                fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
            }
            fl_pop_clip();
            return;
        default:
            return;
    }
}

int TotpTable::handle(int event) {
    switch (event) {
        case FL_RELEASE:
            {
                int row = callback_row();
                int col = callback_col();
                if (Fl::event_button() == FL_LEFT_MOUSE) {
                    if(row >= 0 && row < rows()) {
                        selected_row = row;
                        //std::cout << "L click, row col: " << row << ", " << col << std::endl;
                        redraw();
                    }
                    return 1;
                } else if (Fl::event_button() == FL_RIGHT_MOUSE) {
                    std::cout << "R click, row col: " << row << ", " << col << std::endl;
                    if (row >= 0 && row < rows()) {
                        selected_row = row;
                        std::cout << "R click, selrow: " << selected_row << std::endl;
                        redraw();
                        menu_button->position(Fl::event_x(), Fl::event_y());
                        menu_button->popup();
                        return 1;
                    }
                }
            } break;
        default:
            break;
    }
    return Fl_Table_Row::handle(event);
}

void TotpTable::add_row(const std::vector<std::string>& row_data) {
    table_data.push_back(row_data);
    rows(table_data.size()); // Update the number of rows
    redraw(); // Redraw the table to reflect changes
}

void TotpTable::add_row(const std::vector<std::string>& visible_data, const std::vector<std::string>& hidden_data) {
    visible_column_data.push_back(visible_data);
    hidden_column_data.push_back(hidden_data);
    rows(visible_column_data.size()); // Update the number of rows
    redraw(); // Redraw the table to reflect changes
}

void TotpTable::remove_row(int row) {
    //std::cout << "TotpTable remove row " << row << std::endl;

    visible_column_data.erase(visible_column_data.begin() + row);
    hidden_column_data.erase(hidden_column_data.begin() + row);
    rows(visible_column_data.size()); // Update the number of rows
    redraw(); // Redraw the table to reflect changes
}

/*void TotpTable::remove_row(int row) {
    std::cout << "TotpTable remove row " << row << std::endl;
    //std::vector<std::vector<std::string>> tb_data = get_table_data();
    std::vector<std::vector<std::string>> tb_vis_data = get_visible_column_data();
    std::vector<std::vector<std::string>> tb_hid_data = get_hidden_column_data();

    std::cout << "Printing table" << std::endl;
    for(int i = 0; i < tb_vis_data.size(); i++) {
        std::cout << tb_vis_data.at(i)[0] << std::endl;
    }

    //tb_data.erase(table_data.begin() + row);
    tb_vis_data.erase(tb_vis_data.begin() + row);
    tb_hid_data.erase(tb_hid_data.begin() + row);
    rows(tb_vis_data.size()); // Update the number of rows
    redraw(); // Redraw the table to reflect changes

    std::cout << "\n2nd Printing table" << std::endl;
    for(int i = 0; i < tb_vis_data.size(); i++) {
        std::cout << tb_vis_data.at(i)[0] << std::endl;
    }
}*/

std::string TotpTable::get_hidden_data(int row_index) {
    if (row_index >= 0 && row_index < hidden_column_data.size()) {
        return hidden_column_data[row_index][1];
    }
    return ""; // Return an empty string if the row index is out of range
}

int TotpTable::get_size() {
    return visible_column_data.size();
}

void TotpTable::update_row(int row_index, bool visible) {
    if (row_index >= 0 && row_index < visible_column_data.size()) {
        // Generate new TOTP value based on secret
        if(visible) {
            uint64_t time_step = std::stoi(hidden_column_data[row_index][3]);
            size_t digits = std::stoi(hidden_column_data[row_index][2]);
            hidden_column_data[row_index][0] = totpGen->generateTOTP(hidden_column_data[row_index][1], Timer::get_time(time_step), digits);
            visible_column_data[row_index][1] = hidden_column_data[row_index][0];
        } else {
            visible_column_data[row_index][1] = "******";
        }
        redraw();
    }
}

void TotpTable::hide_rows() {
    for(int i = 0; i < visible_column_data.size(); i++) {
        visible_column_data[i][1] = "******";
        redraw();
    }
}

void TotpTable::show_rows() {
    for(int i = 0; i < visible_column_data.size(); i++) {
        visible_column_data[i][1] = hidden_column_data[i][0];
        redraw();
    }
}

std::vector<std::vector<std::string>> TotpTable::get_table_data() {
    return table_data;
}

std::vector<std::vector<std::string>> TotpTable::get_visible_column_data() {
    return visible_column_data;
}

std::vector<std::vector<std::string>> TotpTable::get_hidden_column_data() {
    return hidden_column_data;
}

void TotpTable::menu_callback(Fl_Widget* w, void* data) {
    std::cout << "Menu option selected: " << static_cast<const char*>(data) << std::endl;
}

void TotpTable::show_modify_window_callback(Fl_Widget* widget, void* data) {
    TotpTable* table = (TotpTable*)data;
    //InputWindow* input_win = new InputWindow(400, 250, "Add Entry", visible);
    //input_win->set_table(table);
    //input_win->show();

}

void TotpTable::show_delete_window_callback(Fl_Widget* widget, void* data) {
    TotpTable* table = (TotpTable*)data;
    //DeleteWindow* delete_win = new DeleteWindow(230, 100, "Delete Entry");
    //delete_win->set_table(table);
    //delete_win->show();


    /*std::vector<std::vector<std::string>> tmp = table->get_visible_column_data();
    int selrow = table->get_selected_row();
    const std::string tmpstring = tmp[selrow][0];
    const char* label = tmpstring.c_str();
    //std::cout << "Sel row: " << table->get_selected_row() << std::endl;
    //std::cout << "Sel item: " << tmpstring.c_str() << std::endl;
    //item_label = new Fl_Box(10, 10, 210, 30, label);        //setting the label here from the string causes it to garble. Setting it to literal text like "Hello" is fine, but the c_str() garbles it.


    ok_btn = new Fl_Button(10, 60, 100, 30, "OK");
    ok_btn->callback(confirm_delete_callback, win);

    cancel_btn = new Fl_Button(120, 60, 100, 30, "Cancel");
    cancel_btn->callback(cancel_delete_callback, win);

    win->end();
    win->show();*/
}

void TotpTable::cancel_delete_callback(Fl_Widget* widget, void* data) {
    Fl_Window* win = (Fl_Window*)data;
    win->hide();
}

int TotpTable::get_selected_row() {
    return selected_row;
}

Fl_Menu_Button* TotpTable::get_menu_button() {
    return menu_button;
}
