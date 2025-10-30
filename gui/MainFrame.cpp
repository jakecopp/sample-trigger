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
    OUTPUT_PATH_BUTTON_ID = 8
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame) 
    EVT_BUTTON(TRIGGER_BUTTON_ID, MainFrame::OnTriggerButtonClicked)
    EVT_BUTTON(SOURCE_FILE_BUTTON_ID, MainFrame::OnSelectSourceFileButtonClicked)
    EVT_BUTTON(SAMPLE_FILE_BUTTON_ID, MainFrame::OnSelectSampleFileButtonClicked)
    EVT_SLIDER(GAIN_SLIDER_ID, MainFrame::OnGainSliderChanged)
    EVT_SPINCTRL(COOLDOWN_SPIN_CTRL, MainFrame::OnCooldownSpinCtrlChanged)
    EVT_BUTTON(OUTPUT_PATH_BUTTON_ID, MainFrame::OnOutputPathButtonClicked)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title) {
    millisecondsCooldown = 50;
    gainThreshold = 0.05;
    outputFilePath = outputFilePathDefaultValue;

    wxPanel* panel = new wxPanel(this);
    wxButton* selectSourceFileButton = new wxButton(panel, SOURCE_FILE_BUTTON_ID, "Open Source File", wxPoint(20, 30), wxSize(-1, -1));
    sourceFileLabel = new wxStaticText(panel, wxID_ANY, "No file selected", wxPoint(150, 30), wxSize(-1, -1));
    wxButton* selectSampleFileButton = new wxButton(panel, SAMPLE_FILE_BUTTON_ID, "Open Sample File", wxPoint(20, 60), wxSize(-1, -1));
    sampleFileLabel = new wxStaticText(panel, wxID_ANY, "No file selected", wxPoint(150, 60), wxSize(-1, -1));
    wxButton* outputPathButton = new wxButton(panel, OUTPUT_PATH_BUTTON_ID, "Save output as", wxPoint(20, 90), wxSize(-1, -1));
    outputFileLabel = new wxStaticText(panel, wxID_ANY, outputFilePathDefaultValue, wxPoint(150, 90), wxSize(-1, -1));


    wxStaticText* text1 = new wxStaticText(panel, wxID_ANY, "Release Time (ms)", wxPoint(20, 130), wxSize(-1, -1));
    wxSpinCtrl spinCtrl = new wxSpinCtrl(panel, COOLDOWN_SPIN_CTRL, "Release", wxPoint(20, 150), wxSize(100, -1), wxSP_ARROW_KEYS, millisecondsCooldownRangeMin, millisecondsCooldownRangeMax, millisecondsCooldownDefaultValue);
    
    wxStaticText* text2 = new wxStaticText(panel, wxID_ANY, "Gain Threshold (dB)", wxPoint(150, 130), wxSize(-1, -1));
    wxStaticText* text5 = new wxStaticText(panel, wxID_ANY, "  6 -", wxPoint(175, 150), wxSize(-1, -1), wxALIGN_RIGHT);
    wxStaticText* text3 = new wxStaticText(panel, wxID_ANY, "  0 -", wxPoint(175, 195), wxSize(-1, -1), wxALIGN_RIGHT);
    wxStaticText* text4 = new wxStaticText(panel, wxID_ANY, "-60 -", wxPoint(168, 433), wxSize(-1, -1), wxALIGN_RIGHT);
    wxSlider* gainSlider = new wxSlider(panel, GAIN_SLIDER_ID, gainDefaultValue, gainRangeMin, gainRangeMax, wxPoint(200, 150), wxSize(10, 300));

    wxButton* triggerButton = new wxButton(panel, TRIGGER_BUTTON_ID, "Trigger Samples", wxPoint(20, 450), wxSize(-1, -1));
    CreateStatusBar();
}


void MainFrame::OnSelectSourceFileButtonClicked(wxCommandEvent& evt) {
    wxFileDialog fileDlg(this, _("Open source WAV file"), "", "",
                       "WAV file|*.wav", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
 
    if (fileDlg.ShowModal() == wxID_OK){
        wxString path = fileDlg.GetPath();
        setSourceFilePath(path);
        sourceFileLabel->SetLabel(path);
        wxLogStatus("Source selected: '%s'", fileDlg.GetPath());
    } else {
        wxLogError("Cannot open file '%s'.", fileDlg.GetPath());
    }
    
}
void MainFrame::OnSelectSampleFileButtonClicked(wxCommandEvent& evt) {
    wxFileDialog fileDlg(this, _("Open sample WAV file"), "", "",
                       "WAV file|*.wav", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
 
    if (fileDlg.ShowModal() == wxID_OK){
        wxString path = fileDlg.GetPath();
        setSampleFilePath(path);
        sampleFileLabel->SetLabel(path);
        wxLogStatus("Sample selected: '%s'", fileDlg.GetPath());
    } else {
        wxLogError("Cannot open file '%s'.", fileDlg.GetPath());
    }
}

void MainFrame::OnGainSliderChanged(wxCommandEvent& evt) {

    double skewFactor = 4.0;
    double skew = pow(10.0f, skewFactor * std::log10(((double)evt.GetInt() - (double)gainRangeMin)/((double)gainRangeMax - (double)gainRangeMin)));
    double gain = ((skew * (double)(gainRangeMax - gainRangeMin)) + gainRangeMin) / 1000;

    double decibels = decibelsFromGain(gain);
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

    triggerSamples(millisecondsCooldown, gainThreshold, false, sourceFilePath, sampleFilePath, outputFilePath);

    std::cout << sourceFilePath << std::endl;
    std::cout << sampleFilePath << std::endl;
    std::cout << gainThreshold << std::endl;
    std::cout << millisecondsCooldown << std::endl;
    std::cout << outputFilePath << std::endl;

    wxLogStatus("Done.");

}

void MainFrame::OnCooldownSpinCtrlChanged(wxSpinEvent& evt) {
    setMillisecondsCooldown(evt.GetPosition());
    wxLogStatus("Release: %d ms", evt.GetPosition());
}

void MainFrame::OnOutputPathButtonClicked(wxCommandEvent& evt) {
    wxFileDialog saveFileDialog(this, _("Save File As"), wxEmptyString, outputFilePathDefaultValue, "WAV file|*.wav", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString path = saveFileDialog.GetPath();
        setOutputFilePath(path);
        outputFileLabel->SetLabel(path);
        wxLogStatus("Output file: %s", path);
    }
    
}

