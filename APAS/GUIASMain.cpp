/***************************************************************
 * Name:      GUIASMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Felipe Diniz (felipebordonidiniz@gmail.com)
 * Created:   2021-02-08
 * Copyright: Felipe Diniz ()
 * License:   MIT License
 **************************************************************/


#ifdef __BORLANDC__
    #pragma hdrstop
#endif



// Includes for the application


#include <cmath>
#include "include/GUIASMain.h"
#ifndef _MSC_VER
#include <stdbool.h>
#include <pthread.h>
#else
#define bool int
#define true 1
#define false 0
#define strdup _strdup
#endif

#if defined(__MINGW32__) ||  defined(_MSC_VER)
#undef BOOLEAN
#include <windows.h>
typedef FARPROC funptr_t;
void *dlopen (const char *, int);
funptr_t dlsym (void *, const char *);
int dlclose (void *);
char *dlerror (void);
#define RTLD_LAZY	1	/* lazy function call binding */
#define RTLD_NOW	2	/* immediate function call binding */
#define RTLD_GLOBAL	4	/* symbols in this dlopen'ed obj are visible to other dlopen'ed objs */
static char errstr[128];
#else
#include <dlfcn.h> /* to load libraries*/
#include <unistd.h>
#include <ctype.h>
typedef void *  funptr_t;
#endif
using namespace std;
/* functions exported by ngspice */
funptr_t ngSpice_Init_handle = NULL;
funptr_t ngSpice_Command_handle = NULL;
funptr_t ngSpice_Circ_handle = NULL;
funptr_t ngSpice_CurPlot_handle = NULL;
funptr_t ngSpice_AllVecs_handle = NULL;
funptr_t ngSpice_GVI_handle = NULL;

int cieq(register char *p, register char *s);
int ciprefix(const char *p, const char *s);

/* Case insensitive prefix. */
int
ciprefix(const char *p, const char *s)
{
    while (*p) {
        if ((isupper(*p) ? tolower(*p) : *p) !=
            (isupper(*s) ? tolower(*s) : *s))
            return(false);
        p++;
        s++;
    }
    return (true);
}

char *errmsg = NULL, *curplot, *vecname;
char const *loadstring;
int *ret, i;
char ** circarray;
char **vecarray;

bool no_bg = true;
bool not_yet = true;
bool will_unload = false;
bool error_ngspice = false;

int vecgetnumber = 0;
double v2dat;
static bool has_break = false;
int testnumber = 0;
void alterp(int sig);

void * ngdllhandle = NULL;

#ifndef _MSC_VER
pthread_t mainthread;
#endif // _MSC_VER

/* Callback function called from bg thread in ngspice to transfer
   any string created by printf or puts. Output to stdout in ngspice is
   preceded by token stdout, same with stderr.*/
int
ng_getchar(char* outputreturn, int ident, void* userdata)
{

    /* setting a flag if an error message occurred */
    if (ciprefix("stderr Error:", outputreturn))
        error_ngspice = true;
    return 0;
}

/* Callback function called from bg thread in ngspice to transfer
   simulation status (type and progress in percent. */
int
ng_getstat(char* outputreturn, int ident, void* userdata)
{
    printf("%s\n", outputreturn);
    return 0;
}

/* Callback function called from ngspice upon starting (returns true) or
  leaving (returns false) the bg thread. */
int
ng_thread_runs(bool noruns, int ident, void* userdata)
{
    no_bg = noruns;
    if (noruns)
        printf("bg not running\n");
    else
        printf("bg running\n");

    return 0;
}

/* Callback function called from bg thread in ngspice if fcn controlled_exit()
   is hit. Do not exit, but unload ngspice. */
int
ng_exit(int exitstatus, bool immediate, bool quitexit, int ident, void* userdata)
{

    if(quitexit) {
        printf("DNote: Returned from quit with exit status %d\n", exitstatus);
    }
    if(immediate) {
        printf("DNote: Unload ngspice\n");
        ((int * (*)(char*)) ngSpice_Command_handle)("quit");
        dlclose(ngdllhandle);
    }

    else {
        printf("DNote: Prepare unloading ngspice\n");
        will_unload = true;
    }

    return exitstatus;

}

/* Callback function called from bg thread in ngspice once per accepted data point */
int
ng_data(pvecvaluesall vdata, int numvecs, int ident, void* userdata)
{


    v2dat = vdata->vecsa[vecgetnumber]->creal;
    if (!has_break && (v2dat > 0.5)) {
    /* using signal SIGTERM by sending to main thread, alterp() then is run from the main thread,
      (not on Windows though!)  */
#ifndef _MSC_VER
        if (testnumber == 4)
            pthread_kill(mainthread, SIGTERM);
#endif
        has_break = true;
    /* leave bg thread for a while to allow halting it from main */
#if defined(__MINGW32__) || defined(_MSC_VER)
        Sleep (100);
#else
        usleep (100000);
#endif
//        ret = ((int * (*)(char*)) ngSpice_Command_handle)("bg_halt");
    }
    return 0;
}

/* Case insensitive str eq.
   Like strcasecmp( ) XXX */
int
cieq(register char *p, register char *s)
{
    while (*p) {
        if ((isupper(*p) ? tolower(*p) : *p) !=
            (isupper(*s) ? tolower(*s) : *s))
            return(false);
        p++;
        s++;
    }
    return (*s ? false : true);
}

/* Callback function called from bg thread in ngspice once upon intialization
   of the simulation vectors)*/
int
ng_initdata(pvecinfoall intdata, int ident, void* userdata)
{
    int i;
    int vn = intdata->veccount;
    for (i = 0; i < vn; i++) {
        printf("Vector: %s\n", intdata->vecs[i]->vecname);
        /* find the location of V(2) */
        if (cieq(intdata->vecs[i]->vecname, "V(2)"))
            vecgetnumber = i;
    }
    return 0;
}




int config_ngspice()
{

    #ifndef _MSC_VER
        mainthread = pthread_self();
    #endif // _MSC_VER
        printf("Load ngspice.dll\n");
    #ifdef __CYGWIN__
        loadstring = "/cygdrive/c/cygwin/usr/local/bin/cygngspice-0.dll";
    #elif _MSC_VER
    //    loadstring = "ngspice.dll";
        loadstring = "..\\..\\..\\ngspice\\visualc\\sharedspice\\ReleaseOMP.Win32\\ngspice.dll";
    #elif __MINGW32__
        loadstring = "D:\\Spice_general\\ngspice\\visualc-shared\\Debug\\bin\\ngspice.dll";
    #elses
        loadstring = "libngspice.so";
    #endif
printf("aaaa");
    // loads the ngspice library depending on the lines above
    // todo: Can I be sure that all those dependencies will be inside the binary?
    ngdllhandle = dlopen(loadstring, RTLD_NOW);
    printf("aaabgbgbgbbggba");
    errmsg = dlerror();
    if (errmsg)
        printf("%s\n", errmsg);
    if (ngdllhandle)
    {
       printf("sasasasasa \n \n \n");
       printf("ngspice.dll loaded \n \n \n");
       printf("aaaawewerewrwerwer");
    }
    else {
       printf("ngspice.dll not loaded !\n");
       exit(1);
    }
printf("aaaa");
    ngSpice_Init_handle = dlsym(ngdllhandle, "ngSpice_Init");
    errmsg = dlerror();
    if (errmsg)
        printf(errmsg);
    ngSpice_Command_handle = dlsym(ngdllhandle, "ngSpice_Command");
    errmsg = dlerror();
    if (errmsg)
        printf(errmsg);
    ngSpice_CurPlot_handle = dlsym(ngdllhandle, "ngSpice_CurPlot");
    errmsg = dlerror();
    if (errmsg)
        printf(errmsg);
    ngSpice_AllVecs_handle = dlsym(ngdllhandle, "ngSpice_AllVecs");
    errmsg = dlerror();
    if (errmsg)
        printf(errmsg);
    ngSpice_GVI_handle = dlsym(ngdllhandle, "ngGet_Vec_Info");
    errmsg = dlerror();
    if (errmsg)
        printf(errmsg);

        printf("aaaa");
    ret = ((int * (*)(SendChar*, SendStat*, ControlledExit*, SendData*, SendInitData*,
             BGThreadRunning*, void*)) ngSpice_Init_handle)(ng_getchar, ng_getstat,
             ng_exit, NULL, ng_initdata, ng_thread_runs, NULL);

}

vector<string> tokenize2(string s, string del = " ")
{
    vector<string> ret;
    int start = 0;
    int end = s.find(del);
    while (end != -1) {
        ret.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }
    ret.push_back (s.substr(start, end - start));
    return ret;
}


/*****************************************************************
Name:       OnRunSimulationClick(wxCommandEvent& event)
Function:   Calls NGSpice and simulates the open netlist
Explain:    Will call the simulation functions using the shared
            SPICE and save the output in .raw file
*****************************************************************/
void OnRunSimulationClick(std::string sourceFile)
{

    //sourceFile = (char*) currentFilePath.ToStdString().c_str();

    //const wxString aa = (char*) sourceFile.mb_str().data();
    sourceFile = "source " + sourceFile;
    vector <string> Vector_File = tokenize2(sourceFile, "\\");
    string fileName = Vector_File.back();
    string saveCommand = "write ";
    saveCommand =  saveCommand + fileName + ".raw all";


    //const wxString bb = (char*) saveCommand.mb_str().data();
    //wxMessageBox( bb, "Mouse capture demo" );

    ret =((int * (*)(char*)) ngSpice_Command_handle)("source /home/Desktop/exemplo_fc_nmos_m_25_ca_gm_id_130nm.cir");
    //ret =((int * (*)(char*)) ngSpice_Command_handle)((char*) sourceFile.c_str());
    ret = ((int * (*)(SendChar*, SendStat*, ControlledExit*, SendData*, SendInitData*,
             BGThreadRunning*, void*)) ngSpice_Init_handle)(ng_getchar, ng_getstat,
             ng_exit, NULL, ng_initdata, ng_thread_runs, NULL);
    ret = ((int * (*)(char*)) ngSpice_Command_handle)("bg_run");
    ret = ((int * (*)(SendChar*, SendStat*, ControlledExit*, SendData*, SendInitData*,
             BGThreadRunning*, void*)) ngSpice_Init_handle)(ng_getchar, ng_getstat,
             ng_exit, NULL, ng_initdata, ng_thread_runs, NULL);
    //todo: change the names for the same as file's
    ret = ((int * (*)(char*)) ngSpice_Command_handle)("wrdata test.txt V(1)");

    //ret = ((int * (*)(char*)) ngSpice_Command_handle)((char*) saveCommand.c_str());
}




