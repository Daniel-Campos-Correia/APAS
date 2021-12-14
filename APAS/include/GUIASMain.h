/***************************************************************
 * Name:      GUIASMain.h
 * Purpose:   Defines Application Frame
 * Author:    Felipe Diniz (felipebordonidiniz@gmail.com)
 * Created:   2021-02-08
 * Copyright: Felipe Diniz ()
 * License:
 **************************************************************/

#ifndef GUIASMAIN_H
#define GUIASMAIN_H


#include "sharedspice.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <vector>
#include "StringUtils.h"
#include <iostream>
#include <gtk/gtk.h>
using namespace std;
void OnRunSimulationClick(string sourceFile);
int config_ngspice();

int
ciprefix(const char *p, const char *s);

/* callback functions used by ngspice */
int
ng_getchar(char* outputreturn, int ident, void* userdata);

int
ng_getstat(char* outputreturn, int ident, void* userdata);

int
ng_thread_runs(bool noruns, int ident, void* userdata);

ControlledExit ng_exit;
SendData ng_data;
SendInitData ng_initdata;



#endif // GUIASMAIN_H
