#include "TotpTable.h"


TotpTable::~TotpTable()
{
    //dtor
}

TotpTable::TotpTable(int X, int Y, int W, int H, const char* L) : Fl_Table_Row(X, Y, W, H, L), hidden_column_data({}) {
    end(); // Close the group
    rows(0); // Initialize with 0 rows
    cols(2); // 2 columns
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
        //redraw_row(row_index); // Redraw the updated row
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
