#ifndef DELETEWINDOW_H
#define DELETEWINDOW_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <TotpTable.h>
#include <cstdint>
#include <TOTP.h>
#include <Timer.h>
#include <FileHandler.h>

class DeleteWindow : public Fl_Window
{
    public:
        DeleteWindow(int W, int H, const char* L=0, int selected_row = -1, TotpTable* tb = nullptr);
        virtual ~DeleteWindow();

        static void confirm_delete_callback(Fl_Widget* widget, void* data);
        static void cancel_delete_callback(Fl_Widget* widget, void* data);
        void set_table(TotpTable* t);
        TotpTable* get_table();
        void set_selected_row(int row);
        int get_selected_row();

    protected:

    private:
        TotpTable* table;
        Fl_Button* ok_btn;
        Fl_Button* cancel_btn;
        Fl_Box* item_label;
        Fl_Window* win;

        int selected_row = -1;
};

#endif // DELETEWINDOW_H
