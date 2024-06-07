#ifndef TOTPTABLE_H
#define TOTPTABLE_H

#include <FL/Fl_Table.H>
#include <FL/Fl_Table_Row.H>
#include <FL/fl_draw.H>
#include <vector>
#include <string>
#include <TOTP.h>
#include <Timer.h>


class TotpTable : public Fl_Table_Row
{
    public:
        //TotpTable();
        virtual ~TotpTable();

        TotpTable(int X, int Y, int W, int H, const char* L=0);
        void add_row(const std::vector<std::string>& row_data);
        void add_row(const std::vector<std::string>& visible_data, const std::vector<std::string>& hidden_data);
        std::string get_hidden_data(int row_index);
        int get_size();
        void update_row(int row_index, bool visible);
        void hide_rows();
        void show_rows();

    protected:
        void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) override {
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
                    fl_color(FL_GRAY0);
                    fl_rect(X, Y, W, H);
                    fl_color(FL_BLACK);
                    /*if (R < table_data.size() && C < table_data[R].size()) {
                        fl_draw(table_data[R][C].c_str(), X, Y, W, H, FL_ALIGN_CENTER);
                    }*/
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

    private:
        std::vector<std::vector<std::string>> table_data;
        std::vector<std::vector<std::string>> visible_column_data;
        std::vector<std::vector<std::string>> hidden_column_data;       //col0 is key, col1 is secret, col2 is #of digits, col3 is timestep

        static TOTP* totpGen;
};

#endif // TOTPTABLE_H
