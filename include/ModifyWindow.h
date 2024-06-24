#ifndef MODIFYWINDOW_H
#define MODIFYWINDOW_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Spinner.H>
#include <TotpTable.h>
#include <cstdint>
#include <TOTP.h>
#include <Timer.h>
#include <Entry.h>
#include <FileHandler.h>

class ModifyWindow : public Fl_Window
{
    public:
        ModifyWindow(int W, int H, const char* L=0, int selected_row=-1, bool visible=true);
        virtual ~ModifyWindow();

        static void modify_callback(Fl_Widget* widget, void* data);
        static void cancel_modify_callback(Fl_Widget* widget, void* data);
        void set_table(TotpTable* t);
        void populate_window();
        TotpTable* get_table();
        void set_selected_row(int row);
        int get_selected_row();

        Entry_d entry;

    protected:

    private:
        Fl_Input* col1_input;
        Fl_Input* col2_input;
        Fl_Spinner* col3_input;
        Fl_Spinner* col4_input;
        Fl_Button* update_button;
        Fl_Button* cancel_button;
        TotpTable* table;

        static TOTP* totpGen;

        int selected_row = -1;
        bool visibility;
};

#endif // MODIFYWINDOW_H
