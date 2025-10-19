#include <wx-3.3/wx/wx.h>
#include <wx-3.3/wx/spinctrl.h>

#include "MainFrame.h"
#include "../sample_trigger/sample_trigger.h"

enum IDs {
    GAIN_SLIDER_ID = 2, 
    SOURCE_FILE_BUTTON_ID = 3,
    SAMPLE_FILE_BUTTON_ID = 4,
    TRIGGER_BUTTON_ID = 5,
    COOLDOWN_SPIN_CTRL = 6,
    OUTPUT_FILE_PATH_TEXT_CTRL = 7
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame) 
    EVT_BUTTON(TRIGGER_BUTTON_ID, MainFrame::OnTriggerButtonClicked)
    EVT_BUTTON(SOURCE_FILE_BUTTON_ID, MainFrame::OnSelectSourceFileButtonClicked)
    EVT_BUTTON(SAMPLE_FILE_BUTTON_ID, MainFrame::OnSelectSampleFileButtonClicked)
    EVT_SLIDER(GAIN_SLIDER_ID, MainFrame::OnGainSliderChanged)
    EVT_SPINCTRL(COOLDOWN_SPIN_CTRL, MainFrame::OnCooldownSpinCtrlChanged)
    EVT_TEXT(OUTPUT_FILE_PATH_TEXT_CTRL, MainFrame::OnOutputPathChanged)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title) {
    millisecondsCooldown = 50;
    gainThreshold = 0.05;
    outputFilePath = outputFilePathDefaultValue;


    wxPanel* panel = new wxPanel(this);
    wxButton* selectSourceFileButton = new wxButton(panel, SOURCE_FILE_BUTTON_ID, "Open Source File", wxPoint(20, 50), wxSize(-1, -1));
    wxButton* selectSampleFileButton = new wxButton(panel, SAMPLE_FILE_BUTTON_ID, "Open Sample File", wxPoint(20, 100), wxSize(-1, -1));
    wxSpinCtrl spinCtrl = new wxSpinCtrl(panel, COOLDOWN_SPIN_CTRL, "Release", wxPoint(20, 200), wxSize(100, -1), wxSP_ARROW_KEYS, millisecondsCooldownRangeMin, millisecondsCooldownRangeMax, millisecondsCooldownDefaultValue);
    wxSlider* gainSlider = new wxSlider(panel, GAIN_SLIDER_ID, gainDefaultValue, gainRangeMin, gainRangeMax, wxPoint(200, 150), wxSize(10, 300));

    wxTextCtrl* outputPathCtrl = new wxTextCtrl(panel, OUTPUT_FILE_PATH_TEXT_CTRL, outputFilePathDefaultValue, wxPoint(20, 400), wxSize(-1, -1));
    wxButton* triggerButton = new wxButton(panel, TRIGGER_BUTTON_ID, "Trigger Samples", wxPoint(20, 450), wxSize(-1, -1));
    CreateStatusBar();
}


void MainFrame::OnSelectSourceFileButtonClicked(wxCommandEvent& evt) {
    wxFileDialog fileDlg(this, _("Open WAV file"), "", "",
                       "WAV file|*.wav", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
 
    if (fileDlg.ShowModal() == wxID_OK){
        wxString path = fileDlg.GetPath();
        setSourceFilePath(path);
        wxLogStatus("Source selected: '%s'", fileDlg.GetPath());
    } else {
        wxLogError("Cannot open file '%s'.", fileDlg.GetPath());
    }
    
}
void MainFrame::OnSelectSampleFileButtonClicked(wxCommandEvent& evt) {
    wxFileDialog fileDlg(this, _("Open WAV file"), "", "",
                       "WAV file|*.wav", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
 
    if (fileDlg.ShowModal() == wxID_OK){
        wxString path = fileDlg.GetPath();
        setSampleFilePath(path);
        wxLogStatus("Sample selected: '%s'", fileDlg.GetPath());
    } else {
        wxLogError("Cannot open file '%s'.", fileDlg.GetPath());
    }
}

void MainFrame::OnGainSliderChanged(wxCommandEvent& evt) {

    float skewFactor = 4.0;
    float skew = pow(10.0f, skewFactor * std::log10(((float)evt.GetInt() - (float)gainRangeMin)/((float)gainRangeMax - (float)gainRangeMin)));
    float gain = ((skew * (float)(gainRangeMax - gainRangeMin)) + gainRangeMin) / 1000;

    float decibels = std::log10 (gain) * 20.0;
    wxString str = wxString::Format("Gain: %.1f dB (%.3f)",  decibels, gain);
    setGainThreshold(gain);
    wxLogStatus(str);
}

void MainFrame::OnTriggerButtonClicked(wxCommandEvent& evt) {

    if (sourceFilePath.empty() || sampleFilePath.empty()) {
        wxLogError("Select both a source and a sample file!");
        return;
    }

    wxLogStatus("Triggering samples...");

    triggerSamples(millisecondsCooldown, gainThreshold, sourceFilePath, sampleFilePath, outputFilePath);

    std::cout << sourceFilePath << std::endl;
    std::cout << sampleFilePath << std::endl;
    std::cout << gainThreshold << std::endl;
    std::cout << millisecondsCooldown << std::endl;
    std::cout << outputFilePath << std::endl;

    wxLogStatus("Done.");

}

void MainFrame::OnCooldownSpinCtrlChanged(wxSpinEvent& evt) {
    setMillisecondsCooldown(evt.GetPosition());
    wxLogStatus("Cooldown %d ms", evt.GetPosition());
}

void MainFrame::OnOutputPathChanged(wxCommandEvent& evt) {
    setOutputFilePath(evt.GetString());
    wxLogStatus("Output file: %s", evt.GetString());
}
