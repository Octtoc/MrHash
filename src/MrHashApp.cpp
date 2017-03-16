/***************************************************************
 * Name:      MrHashApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2015-12-18
 * Copyright:  ()
 * License:
 **************************************************************/

#include "MrHashApp.h"

//(*AppHeaders
#include "MrHashMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP_CONSOLE(MrHashApp);

bool MrHashApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	MrHashFrame* Frame = new MrHashFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
