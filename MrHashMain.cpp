/***************************************************************
 * Name:      MrHashMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2015-12-18
 * Copyright:  ()
 * License:
 **************************************************************/

#include "MrHashMain.h"

#include <fstream>

#include "hashlibrary/crc32.h"
#include "hashlibrary/keccak.h"
#include "hashlibrary/md5.h"
#include "hashlibrary/sha1.h"
#include "hashlibrary/sha256.h"
#include "hashlibrary/sha3.h"

#include <wx/menu.h>
#include <wx/listbox.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
#include <wx/clipbrd.h>

//(*InternalHeaders(MrHashFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(MrHashFrame)
const long MrHashFrame::ID_LISTVIEW1 = wxNewId();
const long MrHashFrame::ID_MENUITEM1 = wxNewId();
const long MrHashFrame::idMenuQuit = wxNewId();
const long MrHashFrame::idMenuAbout = wxNewId();
const long MrHashFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MrHashFrame,wxFrame)
    //(*EventTable(MrHashFrame)
    //*)
END_EVENT_TABLE()

MrHashFrame::MrHashFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(MrHashFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(463,230));
    ListView1 = new wxListView(this, ID_LISTVIEW1, wxPoint(208,368), wxSize(312,120), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTVIEW1"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu1, ID_MENUITEM1, _("Open File"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem3);
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_LISTVIEW1,wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,(wxObjectEventFunction)&MrHashFrame::OnListView1ItemRClick);
    Connect(ID_MENUITEM1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MrHashFrame::OnFile);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MrHashFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MrHashFrame::OnAbout);
    //*)

    ListView1->AppendColumn("Dateiname");
    ListView1->AppendColumn("CRC32");
    ListView1->AppendColumn("Keccak");
    ListView1->AppendColumn("MD5");
    ListView1->AppendColumn("SHA1");
    ListView1->AppendColumn("SHA256");
    ListView1->AppendColumn("SHA3");
    ListView1->AppendColumn("Pfad");
}

MrHashFrame::~MrHashFrame()
{
    //(*Destroy(MrHashFrame)
    //*)
}

wxString MrHashFrame::CalculateHash(wxString filepath, CalcHash hashvalues)
{
    Hash *algorithm;
    algorithm = NULL;
    switch(hashvalues)
    {
    case HL_CRC32:
        algorithm = new CRC32;
        break;
    case HL_Keccak:
        algorithm = new Keccak;
        break;
    case HL_MD5:
        algorithm = new MD5;
        break;
    case HL_SHA1:
        algorithm = new SHA1;
        break;
    case HL_SHA256:
        algorithm = new SHA256;
        break;
    case HL_SHA3:
        algorithm = new SHA3;
        break;
    }
    wxString hashvalue;

    const size_t BufferSize = 144*7*1024;
    char* buffer = new char[BufferSize];
    // select input source: either file or standard-in
    std::ifstream file;
    std::istream* input = NULL;

    // open file
    file.open(filepath.c_str(), std::ios::in | std::ios::binary);

    if (!file)
    {
        std::cerr << "Can't open '" << filepath << "'" << std::endl;
        return wxT("");
    }
    input = &file;

    // process file
    while (*input)
    {
        input->read(buffer, BufferSize);
        std::size_t numBytesRead = size_t(input->gcount());
        algorithm->add(buffer, numBytesRead);
    }

    hashvalue << algorithm->getHash();

    // clean up
    file.close();
//    delete[] buffer;
//    if (input != NULL)
//        delete input;

    //if (algorithm != NULL)
    //    delete algorithm;
    //delete algorithm;

    return hashvalue;
}

void MrHashFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void MrHashFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void MrHashFrame::OnFile(wxCommandEvent& event)
{
    wxString filename, filepath;
    wxString crc32hash, keccakhash, md5hash, sha1hash, sha256hash, sha3hash;

    wxFileDialog fileDialog(this, _("Select file"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
    if (fileDialog.ShowModal() == wxID_OK)
    {
        filepath = fileDialog.GetPath();
        filename = fileDialog.GetFilename();
    }
    else
    {
        return ;
    }

    crc32hash = CalculateHash(filepath, HL_CRC32);
    keccakhash = CalculateHash(filepath, HL_Keccak);
    md5hash = CalculateHash(filepath, HL_MD5);
    sha1hash = CalculateHash(filepath, HL_SHA1);
    sha256hash = CalculateHash(filepath, HL_SHA256);
    sha3hash = CalculateHash(filepath, HL_SHA3);

    wxListItem t1;
    t1.SetId(0);
    t1.SetText(filename);

    ListView1->InsertItem(0,t1);
    ListView1->SetItem(0, 0, filename);
    ListView1->SetItem(0, 1, crc32hash);
    ListView1->SetItem(0, 2, keccakhash);
    ListView1->SetItem(0, 3, md5hash);
    ListView1->SetItem(0, 4, sha1hash);
    ListView1->SetItem(0, 5, sha256hash);
    ListView1->SetItem(0, 6, sha3hash);
    ListView1->SetItem(0, 7, filepath);
}

void MrHashFrame::OnPopupClick(wxCommandEvent &event)
{
    wxString clipHashText;
    //void *data=static_cast<wxMenu *>(event.GetEventObject())->GetClientData();
 	switch(event.GetId()) {
 		case 1:
 		    clipHashText = ListView1->GetItemText(0,1);
 			break;
 		case 2:
 		    clipHashText = ListView1->GetItemText(0,2);
 			break;
        case 3:
            clipHashText = ListView1->GetItemText(0,3);
 			break;
        case 4:
            clipHashText = ListView1->GetItemText(0,4);
 			break;
        case 5:
            clipHashText = ListView1->GetItemText(0,5);
 			break;
        case 6:
            clipHashText = ListView1->GetItemText(0,6);
 			break;
 	}
 	if(wxTheClipboard->Open())
    {
        wxTheClipboard->SetData(new wxTextDataObject(clipHashText));
        wxTheClipboard->Close();
    }
}

void MrHashFrame::OnListView1ItemRClick(wxListEvent& event)
{
    void *data = reinterpret_cast<void *>(event.GetItem().GetData());

    wxMenu menu(wxT("Copy"));
    menu.SetClientData( data );
    menu.Append(1, "CRC32");
    menu.Append(2, "Keccak");
    menu.Append(3, "MD5");
    menu.Append(4, "SHA1");
    menu.Append(5, "SHA256");
    menu.Append(6, "SHA3");

    menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MrHashFrame::OnPopupClick), NULL, this);
    PopupMenu(&menu);
}
