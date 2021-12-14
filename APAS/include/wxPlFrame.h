#ifndef WXPLFRAME_H
#define WXPLFRAME_H



#include <fstream>
#include <iostream>
#include <complex>
#include "stdlib.h"
#include "StringUtils.h"

using namespace std;

class wxPlFrame : public wxPLplotwindow<wxFrame>
{

    public:
        static wxPLplotstream* pls;
        wxPlFrame( const wxString &title );
        virtual void plot();
        virtual void plotD(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);
        wxSize mySize;
    private:
        static PLFLT xinit, yinit, xend, yend;
        virtual void OnLocate( const PLGraphicsIn &graphicsIn );
        void OnIdle( wxIdleEvent &event );
        void OnLeftDown( wxMouseEvent &event );
        void OnLeftUp( wxMouseEvent &event );
        void OnRightButton(wxMouseEvent &event);
        void EnterWindow( wxMouseEvent &event);
        void ExitWindow( wxMouseEvent &event);
        //void OnMouse(wxMouseEvent &event);
        bool m_plotted;
        virtual void ZoomingIn(PLFLT xmin, PLFLT ymin);
        virtual void ZoomingOut();
        bool isCursorInside;
        bool hasStartedZooming;
};

#endif // WXPLFRAME_H
