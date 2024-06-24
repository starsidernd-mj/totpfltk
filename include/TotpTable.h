#ifndef TOTPTABLE_H
#define TOTPTABLE_H

#include <FL/Fl_Table.H>
#include <FL/Fl_Table_Row.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <vector>
#include <string>
#include <TOTP.h>
#include <Timer.h>


class TotpTable : public Fl_Table_Row
{
    public:
        TotpTable(int X, int Y, int W, int H, const char* L=0);
        virtual ~TotpTable();

        void add_row(const std::vector<std::string>& row_data);
        void add_row(const std::vector<std::string>& visible_data, const std::vector<std::string>& hidden_data);
        void remove_row(int row);

        std::string get_hidden_data(int row_index);
        int get_size();
        void update_row(int row_index, bool visible);
        void hide_rows();
        void show_rows();

        std::vector<std::vector<std::string>> get_table_data();
        std::vector<std::vector<std::string>> get_visible_column_data();
        std::vector<std::vector<std::string>> get_hidden_column_data();

        static void menu_callback(Fl_Widget* w, void* data);
        static void show_modify_window_callback(Fl_Widget* widget, void* data);
        static void show_delete_window_callback(Fl_Widget* widget, void* data);

        static void confirm_delete_callback(Fl_Widget* widget, void* data);
        static void cancel_delete_callback(Fl_Widget* widget, void* data);

        int get_selected_row();

        Fl_Menu_Button* get_menu_button();

    protected:
        void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) override;
        int handle(int event) override;

    private:
        std::vector<std::vector<std::string>> table_data;
        std::vector<std::vector<std::string>> visible_column_data;
        std::vector<std::vector<std::string>> hidden_column_data;       //col0 is key, col1 is secret, col2 is #of digits, col3 is timestep

        int selected_row;

        static TOTP* totpGen;

        Fl_Menu_Button* menu_button;
};

#endif // TOTPTABLE_H
