#include <gtk/gtk.h>

//#define _USE_MATH_DEFINES
#include <fstream>
#include <algorithm>
#include <cstring>
#include <stdlib.h>     /* atof */
#include <iostream>
#include <vector>
#include <ctype.h>
#include <cmath>
#include "include/NGSPICE_Program.h"
#include "include/globalVariable.h"
#include "include/gnuplot_i.hpp"
#include "include/graphics.h"
#include <string>
#include <string.h>
#include <list>
#include "include/method_gm_Id.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <filesystem>
#include <stdio.h>
#include <ctype.h>
#include <regex>
#include <regex.h>
#include <math.h>
# define M_PI           3.14159265358979323846  /* pi */

namespace fs = std::filesystem;
typedef void *  funptr_t;
#include <bits/stdc++.h>
using namespace std;



mosfetGraph pmos_graph;
mosfetGraph nmos_graph;
list<Simulation> simulations;
list<model> models;
Gnuplot g1("lines");
Gnuplot g2("lines");


GtkBuilder *builder;


GtkLabel *label;
GtkEntry *entry;
GtkWidget *window_main;
GtkWidget *window_loading;
GtkWidget *window_attachment;
GtkWidget *window_register;
GtkWidget *window_result;
GtkWidget *window_final;
GtkComboBoxText *label_model;
GtkComboBoxText *entry_var_pmos;
GtkComboBoxText *entry_var_nmos;
GtkComboBoxText *label_pmos;
GtkComboBoxText *label_nmos;
GtkLabel *label_model_road;
GtkLabel *label_main_var1;
GtkLabel *label_main_var2;
GtkLabel *label_main_var3;

GtkLabel *result_gain_pmos;
GtkLabel *result_Fc_pmos;
GtkLabel *result_Ft_pmos;
GtkLabel *result_L_pmos;
GtkLabel *result_w_pmos;
GtkLabel *result_gm_id_pmos;
GtkLabel *result_gm_pmos;

GtkLabel *result_gain_nmos;
GtkLabel *result_Fc_nmos;
GtkLabel *result_Ft_nmos;
GtkLabel *result_L_nmos;
GtkLabel *result_w_nmos;
GtkLabel *result_gm_id_nmos;
GtkLabel *result_gm_nmos;



GtkLabel *result_gain_pmos2;
GtkLabel *result_fc_pmos2;
GtkLabel *result_ft_pmos2;
GtkLabel *result_L_pmos2;
GtkLabel *result_W_pmos2;
GtkLabel *result_CL_pmos2;
GtkLabel *result_eficiencia_pmos2;
GtkLabel *result_gm_pmos2;
GtkLabel *result_fu_pmos2;
GtkLabel *result_vth_pmos2;
GtkLabel *result_vgs_pmos2;
GtkLabel *result_vds_pmos2;
GtkLabel *result_ro_pmos2;
GtkLabel *result_id_pmos2;
GtkLabel *result_model_pmos2;


GtkLabel *result_gain_nmos2;
GtkLabel *result_fc_nmos2;
GtkLabel *result_ft_nmos2;
GtkLabel *result_L_nmos2;
GtkLabel *result_W_nmos2;
GtkLabel *result_CL_nmos2;
GtkLabel *result_eficiencia_nmos2;
GtkLabel *result_gm_nmos2;
GtkLabel *result_fu_nmos2;
GtkLabel *result_vth_nmos2;
GtkLabel *result_vgs_nmos2;
GtkLabel *result_vds_nmos2;
GtkLabel *result_ro_nmos2;
GtkLabel *result_id_nmos2;
GtkLabel *result_model_nmos2;



void inicializar_GUI();

void mensagem(const char *text, string secondary_text, char icon_name[100])
{
    int n = secondary_text.length();
    char char_array[n + 1];
    strcpy(char_array, secondary_text.c_str());
    g_print("carai");

    GtkMessageDialog *message = GTK_MESSAGE_DIALOG(gtk_builder_get_object(builder, "message"));

    g_object_set(message, "text", text, NULL);
    g_object_set(message, "secondary_text", char_array, NULL);
    g_object_set(message, "icon_name", icon_name, NULL);

    gtk_widget_show_all (GTK_WIDGET(message));
    gtk_dialog_run      (GTK_DIALOG(message));
    gtk_widget_hide     (GTK_WIDGET(message));

}


bool isNumber(const string& str)
{
   /* regex rgx("\\d+(\\.\\d+)?");
    std::regex_constants::c
    std::regex_match("this is 2 a test 4",m,rgx);
    cout<<m.size();
    regex

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        if (match_str.size() > N) {
            std::cout << "  " << match_str << '\n';
        }
    }

    */
    return regex_match (str, regex("(\\d+)(\\.\\d+)?"));

    //return str.find_first_not_of("0123456789.") == string::npos;
}

void usar_estilo()
{
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "static/uiCSS.css", FALSE);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void on_button_cadastrar_clicked(GtkWidget *widget, gpointer data)
{
    gtk_widget_show(window_register);
    //gtk_widget_destroy(window_main);
    gtk_widget_hide(window_main);

}

void on_button_Iniciar_clicked(GtkWidget *widget, gpointer data)
{
    // Confirmar se todos os dados estão preenchidos de maneira adequada

   // list<string> lookUpDir = listArchives("teste");

        // Confirmar se todos os dados estão preenchidos de maneira adequada
    const gchar *entry_var1;
    const gchar *entry_var2;
    const gchar *entry_var3;
    const gchar *entry_Model_Name;
    const gchar *entry_main_var1;
    const gchar *entry_main_var2;
    const gchar *entry_main_var3;

    entry_var1 = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_var1")));
    entry_var2 = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_var2")));
    entry_var3 = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_var3")));
    entry_Model_Name = gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_builder_get_object(builder,"label_model")));

    entry_main_var1 = gtk_label_get_text(GTK_LABEL(gtk_builder_get_object(builder, "label_var1")));
    entry_main_var2 = gtk_label_get_text(GTK_LABEL(gtk_builder_get_object(builder, "label_var2")));
    entry_main_var3 = gtk_label_get_text(GTK_LABEL(gtk_builder_get_object(builder, "label_var3")));

    string archive_path = "./models/results/";
    string secondary_text = "";
    float aux2 = 0;

    if(!strcmp(entry_Model_Name, ""))
        secondary_text = "Modelo - ";
    else{
        string var_aux = "";
        var_aux += entry_Model_Name;
        transform(var_aux.begin(), var_aux.end(), var_aux.begin(), ::tolower);
        archive_path = archive_path + var_aux;
        g_print(archive_path.c_str());
        if(!std::filesystem::exists(archive_path)){
            mensagem("Erro no Modelo Selecionado.", "Recadastre o modelo no Sistema", "emblem-default");
            return;
        }
       // archive_path += archive_path + "/";
    }

    if(strcmp(entry_var1, "") == 0)
        secondary_text = secondary_text + string(entry_main_var1) +" - ";
    else if (!isNumber(entry_var1)){
        secondary_text = secondary_text + entry_main_var1 +" - ";
    }
    if(!strcmp(entry_var2, ""))
        secondary_text = secondary_text + entry_main_var2 +" - ";
    else if (!isNumber(entry_var2)){
        secondary_text = secondary_text + entry_main_var2 +" - ";
    }

    if(!strcmp(entry_var3, ""))
        secondary_text = secondary_text + entry_main_var3 +" - ";
    else if (!isNumber(entry_var3)){
        secondary_text = secondary_text +entry_main_var3 +" - ";
    }

    if(strcmp(secondary_text.c_str(), ""))
    {
        mensagem("Falta informações obrigatórias ou as mesmas estão incorretas", secondary_text, "emblem-default");
        return;
    } else{
        mensagem("Cadastro Concluido com Suscesso", "Aguarde a configuração do Novo Modelo", "emblem-default");
    }


    // Iniciando tela de carregamento
    gtk_widget_show(window_loading);

   const gchar *entry_text;
    const gchar *entry_box_project;
    entry_text  = gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_builder_get_object(builder,"label_Parametros_de_Projeto")));
    list<string> lookUpDir = listArchives(archive_path);
    tableCreation(lookUpDir, archive_path);
    if(strcmp(entry_text, "LG") == 0){
        testGainAndL(atof((entry_var1)), atof((entry_var3)), atof((entry_var2)));
    }else if(strcmp(entry_text, "LF") == 0){
        return;
    }
    else if(strcmp(entry_text, "gmG") == 0){
        testGainAndEfficiency(atof((entry_var1)), atof((entry_var3)), atof((entry_var2)));
    } else{
        return;
    }






    // Gerando os resultados


    //Finalizando tela de carregamento
    gtk_widget_hide(window_loading);
    gtk_widget_hide(window_main);

    auto it2 = simulations.begin();
    while (it2 != simulations.end()){
        if(!strcmp(it2->type.c_str(), "pmos") && it2->filter1 == true){
            gtk_combo_box_text_append(label_pmos,it2->name.c_str(),it2->name.c_str());
        } else if (!strcmp(it2->type.c_str(), "nmos") && it2->filter1 == true){
            gtk_combo_box_text_append(label_nmos,it2->name.c_str(),it2->name.c_str());
        }
        advance(it2, 1);
    }



    gtk_widget_show(window_result);

    // Abrir Tela de Resultados ainda não projetada


}

void on_loading_Window_destroy(GtkWidget *widget, gpointer data)
{
    // Desbloquear as outras telas
}

void on_widget_attachment_destroy(GtkWidget *widget, gpointer data)
{
    //Liberar Tela window_register

}

void on_button_select_clicked(GtkWidget *widget, gpointer data)
{
    const gchar *entry_attachment;
    entry_attachment  = gtk_file_chooser_get_preview_filename (GTK_FILE_CHOOSER(gtk_builder_get_object(builder,"entry_attachment")));
    gtk_widget_hide(window_attachment);
    label_model_road = GTK_LABEL(gtk_builder_get_object(builder, "label_model_road"));
    gtk_label_set_text(label_model_road,entry_attachment );
}

void on_label_Parametros_de_Projeto_changed(GtkWidget *widget, gpointer data)
{
    const gchar *entry_text;
    const gchar *entry_box_project;

    entry_text  = gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_builder_get_object(builder,"label_Parametros_de_Projeto")));
    label_main_var1 = GTK_LABEL(gtk_builder_get_object(builder, "label_var1"));
    label_main_var2 = GTK_LABEL(gtk_builder_get_object(builder, "label_var2"));
    label_main_var3 = GTK_LABEL(gtk_builder_get_object(builder, "label_var3"));
    if(strcmp(entry_text, "LG") == 0){
        gtk_label_set_text(label_main_var1,"Ganho\n Minimo" );
        gtk_label_set_text(label_main_var2,"L mínimo");
        gtk_label_set_text(label_main_var3,"L máximo");
    }else if(strcmp(entry_text, "LF") == 0){
        gtk_label_set_text(label_main_var1,"Faixa de\n Passagem" );
        gtk_label_set_text(label_main_var2,"L mínimo");
        gtk_label_set_text(label_main_var3,"L máximo");
    }else if(strcmp(entry_text, "gmG") == 0){
        gtk_label_set_text(label_main_var1,"Ganho" );
        gtk_label_set_text(label_main_var2,"Eficiencia\n mínima");
        gtk_label_set_text(label_main_var3,"Eficiencia\n máxima");
    } else{
        gtk_label_set_text(label_main_var1,"Faixa de\n Passagem" );
        gtk_label_set_text(label_main_var2,"Eficiencia\n mínima");
        gtk_label_set_text(label_main_var3,"Eficiencia\n máxima");
    }


}

void on_entry_curve_pmos_changed(GtkWidget *widget, gpointer data)
{
///////////////////////////////////////////////////////////////////
    const gchar *entry_text;
    const gchar *entry_box_curve_pmos;
    string aux = " ";
    int scale = 1;
    entry_text  = gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_builder_get_object(builder,"entry_curve_pmos")));

    result_L_pmos = GTK_LABEL(gtk_builder_get_object(builder, "result_L_pmos"));
    result_L_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_L_pmos2"));
    result_vds_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_vds_pmos2"));

    pmos_graph.x.clear();
    pmos_graph.y.clear();
    pmos_graph.z.clear();

    auto it2 = simulations.begin();
    while (it2 != simulations.end()){
        if(!strcmp(it2->name.c_str(), entry_text)){
            //gtk_combo_box_text_append(label_pmos,it2->name.c_str(),it2->name.c_str());
            gtk_label_set_text(result_L_pmos,it2->comprimento.c_str());
            gtk_label_set_text(result_L_pmos2 ,it2->comprimento.c_str());
            gtk_label_set_text(result_vds_pmos2  ,it2->vds.c_str());
            auto it = it2->lookupTable.begin();
            gtk_combo_box_text_remove_all(entry_var_pmos);
            scale = 19;
            while (it != it2->lookupTable.end())
            {
                if(it->filter1 == true){
                    scale = scale +1;
                    if(scale == 20){
                        gtk_combo_box_text_append(entry_var_pmos,std::to_string(it->eff_n).c_str() , std::to_string(it->eff_n).c_str());
						 pmos_graph.x.push_back(it->eff_n);
                        pmos_graph.y.push_back(it->av_n);
                        pmos_graph.z.push_back(it->ft_n/pow(10, 9));
                        scale = 1;
                    }
                }
                advance(it, 1);
            }



        }
        advance(it2, 1);
    }

}

void on_entry_Fu_pmos_editing_done(GtkWidget *widget, gpointer data)
{

}

void on_entry_CL_pmos_editing_done(GtkWidget *widget, gpointer data)
{

}


void on_entry_var_pmos_changed(GtkWidget *widget, gpointer data)
{
    float gain, ft, JId, gmId, vgs, vth, ro;
    const gchar *entry_Fu_pmos;
    const gchar *entry_CL_pmos;
    const gchar *entry_curve_pmos;
    const gchar *entry_var_pmos;

    result_gain_pmos = GTK_LABEL(gtk_builder_get_object(builder, "result_gain_pmos"));
    result_Fc_pmos = GTK_LABEL(gtk_builder_get_object(builder, "result_Fc_pmos"));
    result_Ft_pmos = GTK_LABEL(gtk_builder_get_object(builder, "result_Ft_pmos"));
    result_L_pmos = GTK_LABEL(gtk_builder_get_object(builder, "result_L_pmos"));
    result_w_pmos = GTK_LABEL(gtk_builder_get_object(builder, "result_w_pmos"));
    result_gm_id_pmos = GTK_LABEL(gtk_builder_get_object(builder, "result_gm_id_pmos"));
    result_gm_pmos = GTK_LABEL(gtk_builder_get_object(builder, "result_gm_pmos"));

    result_gain_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_gain_pmos2"));
    result_fc_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_fc_pmos2"));
    result_ft_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_ft_pmos2"));
    result_L_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_L_pmos2"));
    result_W_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_W_pmos2"));
    result_CL_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_CL_pmos2"));
    result_eficiencia_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_eficiencia_pmos2"));
    result_gm_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_gm_pmos2"));
    result_fu_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_fu_pmos2"));
    result_vth_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_vth_pmos2"));
    result_vgs_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_vgs_pmos2"));

    result_ro_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_ro_pmos2"));
    result_id_pmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_id_pmos2"));


    entry_Fu_pmos = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_Fu_pmos")));
    entry_CL_pmos = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_CL_pmos")));
    entry_curve_pmos = gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_builder_get_object(builder,"entry_curve_pmos")));
    entry_var_pmos = gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_builder_get_object(builder,"entry_var_pmos")));

    if(!entry_curve_pmos)
        entry_curve_pmos="";
    if(!entry_var_pmos)
        entry_var_pmos="";
    else{
        gtk_label_set_text(result_gm_id_pmos,entry_var_pmos);
    }

    auto it2 = simulations.begin();
    while (it2 != simulations.end()){
        if(!strcmp(it2->name.c_str(), entry_curve_pmos)){
            auto it = it2->lookupTable.begin();
            while (it != it2->lookupTable.end())
            {
                if(!strcmp(std::to_string(it->eff_n).c_str(), entry_var_pmos)){
                    gain = it->av_n;
                    ft   = it->ft_n * pow(10,-9);
                    JId  = it->id_w_n;
                    gmId = it->eff_n;
                    vgs = it->vSweep;
                    vth = it->mn_vth;
                    ro = it->ro_n;
                    gtk_label_set_text(result_gain_pmos,std::to_string(it->av_n).c_str());
                    gtk_label_set_text(result_gain_pmos2 ,std::to_string(it->av_n).c_str());
                    gtk_label_set_text(result_Ft_pmos,std::to_string(ft).c_str());
                    gtk_label_set_text(result_ft_pmos2 ,std::to_string(ft).c_str());
                    gtk_label_set_text(result_vgs_pmos2 ,std::to_string(vgs).c_str());
                    gtk_label_set_text(result_vth_pmos2 ,std::to_string(vth).c_str());
                    gtk_label_set_text(result_ro_pmos2 ,std::to_string(ro).c_str());
                }
                advance(it, 1);
            }
        }
        advance(it2, 1);
    }
    if(!strcmp(entry_Fu_pmos, "") || !isNumber(entry_Fu_pmos) || !strcmp(entry_CL_pmos, "") || !isNumber(entry_CL_pmos) ){
        gtk_label_set_text(result_Fc_pmos,"N/A");
        gtk_label_set_text(result_w_pmos,"N/A");
        gtk_label_set_text(result_gm_pmos,"N/A");
    }else{
        double fu, fc, gm, w, cl, Id;
        fu = std::stof(entry_Fu_pmos);
        cl = std::stof(entry_CL_pmos)*pow(10,-12);
        gm = 2*M_PI*fu*cl;
        g_print(std::to_string(gm).c_str());
        fc = fu/gain;
        Id = gm/gmId;
        w = Id/JId*pow(10,9);
        fc = fc * pow(10,-6);
        fu = fu* pow(10,-6);
        gm = gm * pow(10,3);
        gtk_label_set_text(result_Fc_pmos,std::to_string(fc).c_str());
        gtk_label_set_text(result_w_pmos, std::to_string(w).c_str());
        gtk_label_set_text(result_gm_pmos,std::to_string(gm).c_str());
        cl = cl*pow(10,12);
        Id = Id*pow(10,3);
        gtk_label_set_text(result_fc_pmos2 ,std::to_string(fc).c_str());
        gtk_label_set_text(result_W_pmos2 , std::to_string(w).c_str());
        gtk_label_set_text(result_CL_pmos2 ,std::to_string(cl).c_str());
        gtk_label_set_text(result_eficiencia_pmos2 ,std::to_string(gmId).c_str());
        gtk_label_set_text(result_gm_pmos2 , std::to_string(gm).c_str());
        gtk_label_set_text(result_fu_pmos2 ,std::to_string(fu).c_str());
        gtk_label_set_text(result_id_pmos2 ,std::to_string(Id).c_str());

    }

}

void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    cout << endl << "Press ENTER to continue..." << endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}

int GNU_Plotter(mosfetGraph xmos)
{
    // if path-variable for gnuplot is not set, do it with:
    // Gnuplot::set_GNUPlotPath("C:/program files/gnuplot/bin/");

    #if defined(__APPLE__)
      // set a special standard terminal for showonscreen (normally not needed),
      //   e.g. Mac users who want to use x11 instead of aqua terminal:
     Gnuplot::set_terminal_std("x11");
    #endif
    cout << "*** example of gnuplot control through C++ ***" << endl << endl;

    //
    // Using the GnuplotException class
    //
    try
    {
        string dy;

        float yMax = *max_element(begin(xmos.z), end(xmos.z));
        float yMin = *min_element(begin(xmos.z), end(xmos.z));

        dy = to_string(ceil((yMax - yMin)/10));

        g1.reset_all();
        g1.reset_plot();
        g1.set_grid();
        g1.set_xlabel("gm/Id (S/A)");
        g1.set_ylabel("Ganho (V/V)", "Ft (GHz)", dy);
        g1.set_title("Slopes\\nNew Line");
        g1.set_style("lines").plot_xy(xmos.x,xmos.y,"Ganho",1).plot_xy(xmos.x,xmos.z,"Ft",2);

        g1.showonscreen(); // window output
    }
    catch (GnuplotException &ge)
    {
        cout << ge.what() << endl;
    }


    cout << endl << "*** end of gnuplot example" << endl;

    return 0;
}


void on_button_grafic_pmos_clicked(GtkWidget *widget, gpointer data)
{
    GNU_Plotter(pmos_graph);
}

void on_button_grafic_nmos_clicked(GtkWidget *widget, gpointer data)
{
    GNU_Plotter(nmos_graph);
}

void on_entry_curve_nmos_changed(GtkWidget *widget, gpointer data)
{
    const gchar *entry_text;
    const gchar *entry_box_curve_nmos;
    string aux = " ";
    int scale = 1;
    entry_text  = gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_builder_get_object(builder,"entry_curve_nmos")));
    result_L_nmos = GTK_LABEL(gtk_builder_get_object(builder, "result_L_nmos"));
    result_vds_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_vds_nmos2"));
    result_L_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_L_nmos2"));

    nmos_graph.x.clear();
    nmos_graph.y.clear();
    nmos_graph.z.clear();

    auto it2 = simulations.begin();
    while (it2 != simulations.end()){
        if(!strcmp(it2->name.c_str(), entry_text)){
            //gtk_combo_box_text_append(label_nmos,it2->name.c_str(),it2->name.c_str());
            auto it = it2->lookupTable.begin();
            scale = 19;
            gtk_label_set_text(result_L_nmos,it2->comprimento.c_str());
            gtk_label_set_text(result_L_nmos2,it2->comprimento.c_str());
            gtk_label_set_text(result_vds_nmos2 ,it2->vds.c_str());
            gtk_combo_box_text_remove_all(entry_var_nmos);
            while (it != it2->lookupTable.end())
            {
                if(it->filter1 == true){
                    scale = scale +1;
                    if(scale == 20){
                        gtk_combo_box_text_append(entry_var_nmos,std::to_string(it->eff_n).c_str() , std::to_string(it->eff_n).c_str());
						nmos_graph.x.push_back(it->eff_n);
                        nmos_graph.y.push_back(it->av_n);
                        nmos_graph.z.push_back(it->ft_n/pow(10, 9));
                        scale = 1;
                    }
                }
                advance(it, 1);
            }
        }
        advance(it2, 1);
    }

}

void on_entry_Fu_nmos_editing_done(GtkWidget *widget, gpointer data)
{


}
void on_entry_CL_nmos_editing_done(GtkWidget *widget, gpointer data)
{


}




void on_button_var_nmos_changed(GtkWidget *widget, gpointer data)
{
    float gain, ft, JId, gmId, vgs, vth,ro;
    const gchar *entry_Fu_nmos;
    const gchar *entry_CL_nmos;
    const gchar *entry_curve_nmos;
    const gchar *entry_var_nmos;

    result_gain_nmos = GTK_LABEL(gtk_builder_get_object(builder, "result_gain_nmos"));
    result_Fc_nmos = GTK_LABEL(gtk_builder_get_object(builder, "result_Fc_nmos"));
    result_Ft_nmos = GTK_LABEL(gtk_builder_get_object(builder, "result_Ft_nmos"));
    result_L_nmos = GTK_LABEL(gtk_builder_get_object(builder, "result_L_nmos"));
    result_w_nmos = GTK_LABEL(gtk_builder_get_object(builder, "result_w_nmos"));
    result_gm_id_nmos = GTK_LABEL(gtk_builder_get_object(builder, "result_gm_id_nmos"));
    result_gm_nmos = GTK_LABEL(gtk_builder_get_object(builder, "result_gm_nmos"));

    result_gain_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_gain_nmos2"));
    result_fc_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_fc_nmos2"));
    result_ft_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_ft_nmos2"));
    result_L_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_L_nmos2"));
    result_W_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_W_nmos2"));
    result_CL_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_CL_nmos2"));
    result_eficiencia_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_eficiencia_nmos2"));
    result_gm_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_gm_nmos2"));
    result_fu_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_fu_nmos2"));
    result_vth_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_vth_nmos2"));
    result_vgs_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_vgs_nmos2"));

    result_ro_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_ro_nmos2"));
    result_id_nmos2 = GTK_LABEL(gtk_builder_get_object(builder, "result_id_nmos2"));

    entry_Fu_nmos = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_Fu_nmos")));
    entry_CL_nmos = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_CL_nmos")));
    entry_curve_nmos = gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_builder_get_object(builder,"entry_curve_nmos")));
    entry_var_nmos = gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_builder_get_object(builder,"entry_var_nmos")));

    if(!entry_curve_nmos)
        entry_curve_nmos="";
    if(!entry_var_nmos)
        entry_var_nmos="";
    else{
        gtk_label_set_text(result_gm_id_nmos,entry_var_nmos);
    }

    auto it2 = simulations.begin();
    while (it2 != simulations.end()){
        if(!strcmp(it2->name.c_str(), entry_curve_nmos)){
            auto it = it2->lookupTable.begin();
            while (it != it2->lookupTable.end())
            {
                if(!strcmp(std::to_string(it->eff_n).c_str(), entry_var_nmos)){
                    gain = it->av_n;
                    ft   = it->ft_n * pow(10,-9);
                    JId  = it->id_w_n;
                    g_print(std::to_string(JId).c_str());
                    gmId = it->eff_n;
                    vgs = it->vSweep;
                    vth = it->mn_vth;
                    ro = it->ro_n;
                    gtk_label_set_text(result_gain_nmos,std::to_string(it->av_n).c_str());
                    gtk_label_set_text(result_gain_nmos2 ,std::to_string(it->av_n).c_str());
                    gtk_label_set_text(result_Ft_nmos,std::to_string(ft).c_str());
                    gtk_label_set_text(result_ft_nmos2 ,std::to_string(ft).c_str());
                    gtk_label_set_text(result_vgs_nmos2 ,std::to_string(vgs).c_str());
                    gtk_label_set_text(result_vth_nmos2 ,std::to_string(vth).c_str());
                    gtk_label_set_text(result_ro_nmos2 ,std::to_string(ro).c_str());
                }
                advance(it, 1);
            }
        }
        advance(it2, 1);
    }
    if(!strcmp(entry_Fu_nmos, "") || !isNumber(entry_Fu_nmos) || !strcmp(entry_CL_nmos, "") || !isNumber(entry_CL_nmos) ){
        gtk_label_set_text(result_Fc_nmos,"N/A");
        gtk_label_set_text(result_w_nmos,"N/A");
        gtk_label_set_text(result_gm_nmos,"N/A");
    }else{
        double fu, fc, gm, w, cl, Id;
        fu = std::stof(entry_Fu_nmos);
        cl = std::stof(entry_CL_nmos)*pow(10,-12);
        gm = 2*M_PI*fu*cl;
        g_print(std::to_string(gm).c_str());
        fc = fu/gain;
        Id = gm/gmId;
        w = Id/JId*pow(10,9);
        fc = fc * pow(10,-6);
        fu = fu* pow(10,-6);
        gm = gm * pow(10,3);
        gtk_label_set_text(result_Fc_nmos,std::to_string(fc).c_str());
        gtk_label_set_text(result_w_nmos, std::to_string(w).c_str());
        gtk_label_set_text(result_gm_nmos,std::to_string(gm).c_str());
        cl =cl*pow(10,12);
        Id = Id*pow(10,3);
        gtk_label_set_text(result_fc_nmos2 ,std::to_string(fc).c_str());
        gtk_label_set_text(result_W_nmos2 , std::to_string(w).c_str());
        gtk_label_set_text(result_CL_nmos2 ,std::to_string(cl).c_str());
        gtk_label_set_text(result_eficiencia_nmos2 ,std::to_string(gmId).c_str());
        gtk_label_set_text(result_gm_nmos2 , std::to_string(gm).c_str());
        gtk_label_set_text(result_fu_nmos2 ,std::to_string(fu).c_str());
        gtk_label_set_text(result_id_nmos2 ,std::to_string(Id).c_str());

    }

}

//void on_button_grafic_nmos_clicked(GtkWidget *widget, gpointer data)
//{

//}
void on_button_Iniciar6_clicked(GtkWidget *widget, gpointer data)
{


}

void on_widget_register_destroy(GtkWidget *widget, gpointer data)
{


}

void on_button_cadastrar2_clicked (GtkWidget *widget, gpointer data)
{

    // Abrir tela window_attachment
        gtk_widget_show(window_attachment);
    // Bloquear window_register tela
}

void on_main_Window_destroy(GtkWidget *widget, gpointer data)
{

}



/////


void on_button_retornar_inicio_clicked(GtkWidget *widget, gpointer data)
{
    gtk_widget_hide(window_register);
    gtk_widget_hide(window_result);
    inicializar_GUI();
}

void on_entry_CLorFu_nmos_changed(GtkWidget *widget, gpointer data)
{

}

void on_entry_teste(GtkWidget *widget, gpointer data)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////                               Acionado quando o modelo é alterado. Muda as informações básicas do modelo                                                    //////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void on_label_model_changed(GtkWidget *widget, gpointer data)
{
    g_print("AAAAA");
    GtkLabel *info_vds;
    GtkLabel *info_manufacturer;
    GtkLabel *info_lmin;
    GtkLabel *info_lmax;
    const gchar *entry_model;
    result_model_nmos2=GTK_LABEL(gtk_builder_get_object(builder, "result_model_nmos2"));;
    result_model_pmos2=GTK_LABEL(gtk_builder_get_object(builder, "result_model_pmos2"));;
    info_vds = GTK_LABEL(gtk_builder_get_object(builder, "info_vds"));
    info_manufacturer = GTK_LABEL(gtk_builder_get_object(builder, "info_manufacturer"));
    info_lmin = GTK_LABEL(gtk_builder_get_object(builder, "info_lmin"));
    info_lmax = GTK_LABEL(gtk_builder_get_object(builder, "info_lmax"));
    entry_model = gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_builder_get_object(builder,"label_model")));
    auto it2 = models.begin();
    while (it2 != models.end()){
        if(!strcmp(it2->name.c_str(), entry_model)){
            gtk_label_set_text(result_model_nmos2 ,   it2->name.c_str());
            gtk_label_set_text(result_model_pmos2 ,   it2->name.c_str());
            gtk_label_set_text(info_vds ,   it2->vds.c_str());
            gtk_label_set_text(info_manufacturer , it2->manufacturer.c_str());
            gtk_label_set_text(info_lmax ,   it2->lmax.c_str());
            gtk_label_set_text(info_lmin , it2->lmin.c_str());
            break;
        }
        advance(it2, 1);
    }

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void on_entry_CLorFU_nmos_editing_done(GtkWidget *widget, gpointer data)
{

}


void on_button_Final_clicked(GtkWidget *widget, gpointer data)
{
    gtk_widget_hide(window_result);
    gtk_widget_show(window_final);
}
///////
void on_main_Window_show(GtkWidget *widget, gpointer data)
{
    ifstream entry_archive;
    string line;
    gchar *name;
    entry_archive.open("./models/info_models.txt");

    if (!entry_archive.is_open())
    {
        mensagem("Ocorreu um Erro na abertura dos Arquivos", "Entre em contato com o Desenvolvedor", "emblem-default");
        exit(1);
    }

    model modelAux;
    vector<string> vectorLine;
    while(getline(entry_archive,line)){
        vectorLine = tokenize(line, "$%");
        modelAux.name = vectorLine[0];
        modelAux.vds = vectorLine[3];
        modelAux.manufacturer = vectorLine[5];
        modelAux.lmin = vectorLine[6];
        modelAux.lmax = vectorLine[7];
        modelAux.vth = vectorLine[3];
        models.push_front(modelAux);
        gtk_combo_box_text_append(label_model,modelAux.name.c_str(),modelAux.name.c_str());
    }
    entry_archive.close();


    return;
}

void copy_archive(const char *from, const char *to)
{
    char buf[BUFSIZ];
    size_t size;

    FILE* source = fopen(from, "rb");
    FILE* dest = fopen(to, "wb");

    // clean and more secure
    // feof(FILE* stream) returns non-zero if the end of file indicator for stream is set

    while (size = fread(buf, 1, BUFSIZ, source)) {
        fwrite(buf, 1, size, dest);
    }

    fclose(source);
    fclose(dest);

}

void on_button_finalizar_clicked(GtkWidget *widget, gpointer data)
{
    // Confirmar se todos os dados estão preenchidos de maneira adequada
    const gchar *entry_Additional;
    const gchar *entry_manufacturer;
    const gchar *entry_format;
    const gchar *entry_VDD;
    const gchar *entry_NMOS;
    const gchar *entry_PMOS;
    const gchar *entry_Model_Name;
    const gchar *entry_attachment;

    entry_Additional = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_Adcional")));
    entry_manufacturer = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_manufacturer")));
    entry_format = gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_builder_get_object(builder,"entry_format")));
    entry_VDD = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_VDD")));
    entry_NMOS = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_NMOS")));
    entry_PMOS = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_PMOS")));
    entry_attachment = gtk_label_get_text(GTK_LABEL(gtk_builder_get_object(builder, "label_model_road")));
    entry_Model_Name = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_Modelo_Name")));

    if(!entry_format)
        entry_format="";
    string secondary_text = "";
    float aux2 = 0;
    if(strcmp(entry_Additional, "") == 0)
        secondary_text = "Informações Adicionais - ";

    if(!strcmp(entry_attachment, "")|| !strcmp(entry_attachment, "Selecione o Arquivo"))
        secondary_text += "Arquivo - ";

    if(!strcmp(entry_manufacturer, ""))
        secondary_text += "Fabricante - ";

    if(!strcmp(entry_format, ""))
        secondary_text += "Formato - ";

    if(!strcmp(entry_VDD, ""))
        secondary_text += "VDD - ";
    else if (!isNumber(entry_VDD)){
        secondary_text += "VDD - ";
    }

    if(!strcmp(entry_Model_Name, ""))
        secondary_text += "Nome -";

    if(!strcmp(entry_NMOS, "") )
        secondary_text += "VTH NMOS - ";
    else if (!isNumber(entry_NMOS)){
        secondary_text += "VTH NMOS - ";
    }

    if(!strcmp(entry_PMOS, ""))
        secondary_text += "VTH PMOS";
    else if (!isNumber(entry_PMOS)){
        secondary_text += "VTH PMOS - ";
    }


    if(strcmp(secondary_text.c_str(), ""))
    {
        mensagem("Falta informações obrigatórias ou as mesmas estão incorretas", secondary_text, "emblem-default");
        return;
    } else{
        mensagem("Cadastro Concluido com Suscesso", "Aguarde a configuração do Novo Modelo", "emblem-default");
    }


    // Iniciando tela de carregamento
    gtk_widget_show(window_loading);


    // Gerando o Arquivo :)

    ifstream entry_archive;
    ofstream out_archive;
    string line;

    string archive_path = "./models/programs/";
    archive_path = archive_path + entry_Model_Name;

    fs::create_directory(archive_path.c_str())?
    g_print("created directory - ") :
    mensagem("Ocorreu um Erro na Criação da Pasta", "Verifique as Permissões", "emblem-default");

    int i = mkdir( archive_path.c_str() , 0700);
    string model_path = archive_path + "/model_" + entry_Model_Name + ".pm";
    archive_path = archive_path + "/" + entry_Model_Name + ".cir";

    entry_archive.open("./models/basic_programs/template_Mosfet.cir");
    out_archive.open(archive_path.c_str());

    if (!entry_archive.is_open() || !out_archive.is_open())  { // Se houve erro na abertura
        try{
            entry_archive.close();
        }
        catch(string e){}
        try{
            out_archive.close();
        }
        catch(string e){}
        mensagem("Ocorreu um Erro na abertura dos Arquivos", "Entre em contato com o Desenvolvedor", "emblem-default");
        return;
    }


    while (getline(entry_archive,line))
    {

        line = string_replace(line, "{$modelMosfet$}", entry_Model_Name);
        line = string_replace(line, "{$w$}"      , "10");
        line = string_replace(line, "{$lambda$}" , "65");
        line = string_replace(line, "{$vdd$}"    , entry_VDD);
        line = string_replace(line, "{$vgnmos$}" , "0.7");
        line = string_replace(line, "{$vgpmos$}" , "0.7");
        line = string_replace(line, "{$compMax$}", "270");
        line = string_replace(line, "{$delta$}"  , "10");
        line = string_replace(line, "{$compMin$}", "130");
        line = string_replace(line, "{$pontos&}" , "5200");

        out_archive << line << endl;

    }

    out_archive.close();
    entry_archive.close();

    copy_archive(entry_attachment, model_path.c_str());

    string result_Path = "./models/results/"; ;
    string var_aux = "";
    var_aux += entry_Model_Name;
    transform(var_aux.begin(), var_aux.end(), var_aux.begin(), ::tolower);
    result_Path = result_Path + (var_aux);
    fs::create_directory(result_Path)?
    g_print("created directory - ") :
    mensagem("Ocorreu um Erro na Criação da Pasta", "Verifique as Permissões", "emblem-default");


    bool test = runSimulation(archive_path);

    if(!test){
        gtk_widget_hide(window_loading);
        mensagem("Ocorreu um Erro na Simulação - Verifique o código criado", "Verifique as Permissões", "emblem-default");
        return;
    }

    list<string> lookUpDir = listArchives(result_Path);

    runSimulation(archive_path);

    fstream modelArchive;
    modelArchive.open("./models/info_models.txt", ios::app);

    if (modelArchive.is_open())
    {
        modelArchive << entry_Model_Name << "$%";
        modelArchive << entry_NMOS << "$%";
        modelArchive << entry_PMOS << "$%";
        modelArchive << entry_VDD << "$%";
        modelArchive << entry_format << "$%";
        modelArchive << entry_manufacturer << "$%";
        modelArchive << "130n" << "$%";
        modelArchive << "270n" << "$%";
        modelArchive.close();
    }




    //Finalizando tela de carregamento
    gtk_widget_hide(window_loading);

    //Voltando para tela inicial
    gtk_widget_hide(window_register);
    inicializar_GUI();

    return;

}

void inicializar_GUI()
{
    builder = gtk_builder_new_from_file("ui_TCC.glade"); // Buildando o arquivo .glade (Interface gráfica)

    gtk_builder_add_callback_symbols(builder,
                                 "on_button_cadastrar_clicked", G_CALLBACK(on_button_cadastrar_clicked),
                                 "on_button_Iniciar_clicked", G_CALLBACK(on_button_Iniciar_clicked),
                                 "on_loading_Window_destroy", G_CALLBACK(on_loading_Window_destroy),
                                 "on_widget_attachment_destroy", G_CALLBACK(on_widget_attachment_destroy),
                                 "on_button_select_clicked", G_CALLBACK(on_button_select_clicked),
                                 "on_widget_register_destroy", G_CALLBACK(on_widget_register_destroy),
                                 "on_button_cadastrar2_clicked", G_CALLBACK(on_button_cadastrar2_clicked),
                                 "on_main_Window_destroy", G_CALLBACK(on_main_Window_destroy),
                                 "on_main_Window_show", G_CALLBACK(on_main_Window_show),
                                 "on_button_finalizar_clicked", G_CALLBACK(on_button_finalizar_clicked),
                                 "on_label_Parametros_de_Projeto_changed", G_CALLBACK(on_label_Parametros_de_Projeto_changed),
                                 "on_entry_curve_pmos_changed", G_CALLBACK(on_entry_curve_pmos_changed),
                                 "on_entry_Fu_pmos_editing_done", G_CALLBACK(on_entry_Fu_pmos_editing_done),
                                 "on_entry_CL_pmos_editing_done", G_CALLBACK(on_entry_CL_pmos_editing_done),
                                 "on_entry_var_pmos_changed", G_CALLBACK(on_entry_var_pmos_changed),
                                 "on_button_grafic_pmos_clicked", G_CALLBACK(on_button_grafic_pmos_clicked),
                                 "on_entry_curve_nmos_changed", G_CALLBACK(on_entry_curve_nmos_changed),
                                 "on_entry_Fu_nmos_editing_done", G_CALLBACK(on_entry_CLorFU_nmos_editing_done),
                                 "on_entry_CL_nmos_editing_done", G_CALLBACK(on_entry_CLorFU_nmos_editing_done),
                                 "on_button_var_nmos_changed", G_CALLBACK(on_button_var_nmos_changed),
                                 "on_button_grafic_nmos_clicked", G_CALLBACK(on_button_grafic_nmos_clicked),
                                 "on_button_Iniciar6_clicked", G_CALLBACK(on_button_Iniciar6_clicked),
                                 "on_entry_CL_pmos_changed", G_CALLBACK(on_entry_CLorFU_nmos_editing_done),
                                 "on_entry_Fu_pmos_changed", G_CALLBACK(on_entry_CLorFU_nmos_editing_done),
                                 "on_button_retornar_inicio_clicked", G_CALLBACK(on_button_retornar_inicio_clicked),
                                 "on_button_retornar_inicio2_clicked", G_CALLBACK(on_button_retornar_inicio_clicked),
                                 "on_entry_CL_nmos_changed", G_CALLBACK(on_entry_CLorFu_nmos_changed),
                                 "on_entry_Fu_nmos_changed", G_CALLBACK(on_entry_CLorFu_nmos_changed),
                                 "on_entry_Fu_pmos_remove_widget", G_CALLBACK(on_entry_teste),
                                 "on_button_Final_clicked", G_CALLBACK(on_button_Final_clicked),
                                 "on_label_model_changed", G_CALLBACK(on_label_model_changed),
                                 NULL); // Conexão das funções

    gtk_builder_connect_signals(builder, NULL); //

    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "main_Window"));
    window_loading = GTK_WIDGET(gtk_builder_get_object(builder, "loading_Window"));
    window_attachment = GTK_WIDGET(gtk_builder_get_object(builder, "widget_attachment"));
    window_register = GTK_WIDGET(gtk_builder_get_object(builder, "widget_register"));
    label_model = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "label_model"));
    entry_var_pmos = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "entry_var_pmos"));
    entry_var_nmos = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "entry_var_nmos"));
    label_pmos = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "entry_curve_pmos"));
    label_nmos = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "entry_curve_nmos"));

    window_result = GTK_WIDGET(gtk_builder_get_object(builder, "Result_Window"));
    window_final = GTK_WIDGET(gtk_builder_get_object(builder, "Final_Window"));
    usar_estilo();
    gtk_widget_show(window_main);

}


int main(int argc, char *argv[])
{

    //inicializar_NGSPICE();
    gtk_init(&argc, &argv); // Inicia a Aplicação Relativa ao Glade
    inicializar_GUI();
    gtk_main();
   // config_ngspice();
    //runSimulation("./models/programs/100.cir");
    return 0;

}
