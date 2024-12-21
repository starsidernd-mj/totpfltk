#include "ConfirmInput.h"

ConfirmInput::ConfirmInput(int x, int y, int w, int h, const char *l, Fl_Button *submit_btn) : Fl_Input(x, y, w, h, l), submit_button(submit_btn)
{
    //ctor
}

ConfirmInput::~ConfirmInput()
{
    //dtor
}

