#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <cstdio>

int main() {
  Fl_Window win(0, 0, 512, 512, "FLTK C++ app");
  Fl_Button btn(0, 0, 512, 512, "Click me!");
  btn.callback([](auto){ fputs("Clicked!", stderr); });
  win.end();
  win.show();
  return Fl::run();
}
