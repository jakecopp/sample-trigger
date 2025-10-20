#include <wx-3.3/wx/wx.h>
#include<wx-3.3/wx/frame.h>
#include<wx-3.3/wx/string.h>
#include <wx-3.3/wx/spinctrl.h>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
    
private:
    int millisecondsCooldown;
    double gainThreshold;
    std::string sourceFilePath;
    std::string sampleFilePath;
    std::string outputFilePath;

    const int gainRangeMin = 1;
    const int gainRangeMax = 2000;
    const int millisecondsCooldownRangeMin = 1;
    const int millisecondsCooldownRangeMax = 1000;

    const int gainDefaultValue = 1000;
    const int millisecondsCooldownDefaultValue = 50;
    const std::string outputFilePathDefaultValue = "output-audio.wav";

    wxStaticText* sourceFileLabel;
    wxStaticText* sampleFileLabel;
    wxStaticText* outputFileLabel;

    // wxSpinCtrl spinCtrl;
    void OnGainSliderChanged(wxCommandEvent& evt);
    void OnSelectSourceFileButtonClicked(wxCommandEvent& evt);
    void OnSelectSampleFileButtonClicked(wxCommandEvent& evt);
    void OnTriggerButtonClicked(wxCommandEvent& evt);
    void OnCooldownSpinCtrlChanged(wxSpinEvent& evt);
    // void OnOutputPathChanged(wxCommandEvent& evt);
    void OnOutputPathButtonClicked(wxCommandEvent& evt);


    void setMillisecondsCooldown(int m) {
        millisecondsCooldown = m;
    }
    void setGainThreshold(double g) {
        gainThreshold = g;
    }
    void setSourceFilePath(wxString s) {
        sourceFilePath = s.ToStdString();;
    }
    void setSampleFilePath(wxString s) {
        sampleFilePath = s.ToStdString();;
    }
     void setOutputFilePath(wxString s) {
        outputFilePath = s.ToStdString();;
    }
    wxDECLARE_EVENT_TABLE();
};