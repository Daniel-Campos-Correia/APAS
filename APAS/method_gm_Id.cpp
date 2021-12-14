#include "include/method_gm_Id.h"
#include "include/globalVariable.h"

extern list<Simulation> simulations;

void FindReplace(string& line, string& oldString, string& newString) {
  const size_t oldSize = oldString.length();

  // do nothing if line is shorter than the string to find
  if( oldSize > line.length() ) return;

  const size_t newSize = newString.length();
  for( size_t pos = 0; ; pos += newSize ) {
    // Locate the substring to replace
    pos = line.find( oldString, pos );
    if( pos == string::npos ) return;
    if( oldSize == newSize ) {
      // if they're same size, use std::string::replace
      line.replace( pos, oldSize, newString );
    } else {
      // if not same size, replace by erasing and inserting
      line.erase( pos, oldSize );
      line.insert( pos, newString );
    }
  }
}


string string_replace( string src, string const& target, string const& repl)
{
    // handle error situations/trivial cases

    if (target.length() == 0) {
        // searching for a match to the empty string will result in
        //  an infinite loop
        //  it might make sense to throw an exception for this case
        return src;
    }

    if (src.length() == 0) {
        return src;  // nothing to match against
    }

    size_t idx = 0;

    for (;;) {
        idx = src.find( target, idx);
        if (idx == string::npos)  break;

        src.replace( idx, target.length(), repl);
        idx += repl.length();
    }

    return src;
}


vector<string> tokenize(string s, string del )
{
    vector<string> ret;
    int start = 0;
    int end = s.find(del);
    while (end != -1) {
        if(start != end)
        {
        ret.push_back(s.substr(start, end - start));
        }

        start = end + del.size();
        end = s.find(del, start);
    }
    ret.push_back (s.substr(start, end - start));
    return ret;
}

list<string> listArchives(string path)
{
    list<string> listDir;

    DIR *dir;
    struct dirent *lsdir;

    dir = opendir(path.c_str());

    // print all the files and directories within directory

    while ( ( lsdir = readdir(dir) ) != NULL )
    {
        if(strcmp(lsdir->d_name, ".") && strcmp(lsdir->d_name, "..")){
            listDir.push_front(lsdir->d_name);
            cout<< lsdir->d_name <<"\n";
        }

    }

    closedir(dir);

    return listDir;

}


void testGainAndEfficiency(float ganho, float eficiencia_maxima, float eficiencia_minima)
{
    auto it2 = simulations.begin();
    while (it2 != simulations.end())
    {
        //list<OperationPoint>::iterator it;
        auto it = it2->lookupTable.begin();
        bool Verificacao = false;
        while (it != it2->lookupTable.end())
        {
            if(it->av_n > ganho && it->eff_n < eficiencia_maxima && it->eff_n > eficiencia_minima)
            {
                //cout << "\n" << it->av_n << " Result: " <<  ganho*0.99 << "\n";
                it->filter1 = true;
                Verificacao = true;
            }
            advance(it, 1);
        }
        it2->filter1 = Verificacao;
        cout << "\n" << it2->comprimento << " Result: " <<  it2->filter1 << "\n";
        advance(it2, 1);
    }
}


void testGainAndL(float ganho, float L_maximo, float L__minimo)
{
    auto it2 = simulations.begin();
    while (it2 != simulations.end())
    {
        //list<OperationPoint>::iterator it;
        auto it = it2->lookupTable.begin();
        bool Verificacao = false;
        float test = atof(it2->comprimento.c_str());
        if((test-1)< L_maximo && (test+1)> L__minimo){
            cout << "\n" <<  " Result:";
            while (it != it2->lookupTable.end())
            {
                if(it->av_n > ganho)
                {
                    //cout << "\n" << it->av_n << " Result: " <<  ganho*0.99 << "\n";
                    it->filter1 = true;
                    Verificacao = true;
                }
                advance(it, 1);
            }
        }
        it2->filter1 = Verificacao;
        cout << "\n" << it2->comprimento << " Result: " <<  it2->filter1 << "\n";
        advance(it2, 1);
    }
}


void tableCreation(list<string> listLookUpDir, string path)
{
    ifstream entry_archive;
    string line;
    string name = "";
    vector<string> informations;
    string lookUpDir;
    vector<string> table;

    auto it = listLookUpDir.begin();
    while (it != listLookUpDir.end())
    {
        lookUpDir="";
        lookUpDir =lookUpDir +path  + "/" + *it;
        name = "";
        name = name + *it;
        informations = tokenize(name, "_");
        entry_archive.open(lookUpDir);

        if (!entry_archive.is_open())
        {
            cout << "Erro ao Abrir Arquivo" << "\n";
        }

        cout << "Aberto" << "\n";
        int i=0;
        list<OperationPoint> auxList;
        OperationPoint pointAux;
        getline(entry_archive,line);
        while(getline(entry_archive,line)){
            table = tokenize(line, " ");
            pointAux.vSweep = std::stof(table[0]);
            pointAux.mn_id = std::stof(table[1]);
            pointAux.mn_gm = std::stof(table[2]);
            pointAux.mn_gds = std::stof(table[3]);
            pointAux.mn_gmbs = std::stof(table[4]);
            pointAux.mn_sat = std::stof(table[5]);
            pointAux.mn_vth = std::stof(table[6]);
            pointAux.mn_cgd = std::stof(table[7]);
            pointAux.mn_cgg = std::stof(table[8]);
            pointAux.mn_vgs = std::stof(table[9]);
            pointAux.mn_vds = std::stof(table[10]);
            pointAux.mn_vbs = std::stof(table[11]);
            pointAux.eff_n = std::stof(table[12]);
            pointAux.ov_n = std::stof(table[13]);
            pointAux.ft_n = std::stof(table[14]);
            pointAux.av_n = std::stof(table[15]);
            pointAux.id_w_n = std::stof(table[16]);
            pointAux.ro_n = std::stof(table[17]);
            pointAux.eff_ft_n = std::stof(table[18]);
            pointAux.filter1 = false;
            pointAux.filter2 = false;
            auxList.push_front(pointAux);

        }
        entry_archive.close();

        Simulation lookSimulation;

        lookSimulation.lookupTable=auxList;
        lookSimulation.filter1 = false;
        lookSimulation.vds = informations[7];
        lookSimulation.comprimento = informations[5];
        lookSimulation.type = informations[1];
        lookSimulation.name = name;
        cout << lookUpDir << auxList.front().eff_ft_n << "\n";
        simulations.push_front(lookSimulation);

        advance(it, 1);
    }

    return;
}





