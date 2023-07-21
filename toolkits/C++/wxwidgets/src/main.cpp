#include <wx/wx.h>

class MyApp : public wxApp {
public:
    bool OnInit() override {
        if (!wxApp::OnInit())
            return false;

        auto win = new wxFrame(nullptr, wxID_ANY, L"Hello wxWidgets!");
        win->SetSize(512, 512);
        auto btn = new wxButton(win, wxID_ANY, "Click me!");
        btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [](auto) { puts("Clicked!"); });
        win->Show();
        return true;
    }
};

int main(int argc, char **argv) {
    wxApp::SetInstance(new MyApp{});
    return wxEntry(argc, argv);
}