#include "App.h"
#include "MainFrame.h"
#include <wx-3.3/wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    MainFrame* mainFrame = new MainFrame("Jake's EZ Sample Triggering");
    mainFrame->SetClientSize(300, 500);
    mainFrame->Center();
    mainFrame->Show();
    return true;
}
