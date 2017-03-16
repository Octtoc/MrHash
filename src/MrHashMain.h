/***************************************************************
 * Name:      MrHashMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2015-12-18
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef MRHASHMAIN_H
#define MRHASHMAIN_H

//(*Headers(MrHashFrame)
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

enum CalcHash {
    HL_CRC32, HL_Keccak, HL_MD5, HL_SHA1, HL_SHA256, HL_SHA3 };

class MrHashFrame: public wxFrame
{
    public:

        MrHashFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~MrHashFrame();

    private:
        wxString CalculateHash(wxString filepath, CalcHash hashvalues);

        void OnPopupClick(wxCommandEvent &event);

        //(*Handlers(MrHashFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnFile(wxCommandEvent& event);
        void OnListBox1Select(wxCommandEvent& event);
        void OnListView1BeginDrag(wxListEvent& event);
        void OnListView1RightDown(wxListEvent& event);
        void OnListView1ColumnRClick(wxListEvent& event);
        void OnListView1ItemRClick(wxListEvent& event);
        //*)

        //(*Identifiers(MrHashFrame)
        static const long ID_LISTVIEW1;
        static const long ID_MENUITEM1;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(MrHashFrame)
        wxMenuItem* MenuItem3;
        wxStatusBar* StatusBar1;
        wxListView* ListView1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // MRHASHMAIN_H
