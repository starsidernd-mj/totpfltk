#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Spinner.H>
#include <TotpTable.h>
#include <cstdint>
#include <TOTP.h>
#include <Timer.h>

class InputWindow : public Fl_Window
{
    public:
        //InputWindow();
        virtual ~InputWindow();

        InputWindow(int W, int H, const char* L=0, bool visible=true);
        static void add_callback(Fl_Widget* widget, void* data);
        void set_table(TotpTable* t);

    protected:

    private:
        Fl_Input* col1_input;
        Fl_Input* col2_input;
        Fl_Spinner* col3_input;
        Fl_Spinner* col4_input;
        Fl_Button* add_button;
        TotpTable* table;

        static TOTP* totpGen;
        std::string totp;

        bool visibility;
};

#endif // INPUTWINDOW_H
