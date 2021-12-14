#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H

#include <string>
#include <string.h>
#include <list>


using namespace std;
struct OperationPoint {
    float vSweep;  //Vov
    float mn_id;
    float mn_gm;
    float mn_gds;
    float mn_gmbs;
    float mn_sat;
    float mn_vth;
    float mn_cgd;
    float mn_cgg;
    float mn_vgs;
    float mn_vds;
    float mn_vbs;
    float eff_n;  //gm/Id
    float ov_n;
    float ft_n;   //ft
    float av_n;   //Ganho
    float id_w_n;
    float ro_n;
    float eff_ft_n;
    bool  filter1;
    bool  filter2;
};

struct Simulation {
    list<OperationPoint> lookupTable;
    string vds;
    string comprimento;
    string type;
    string name;
    bool filter1;
};


struct model {
    string name;
    string vds;
    string manufacturer;
    string vth;
    string lmin;
    string lmax;
};

class mosfetGraph{
    public:
        vector<float> x, y, z;
};


#endif // GLOBALVARIABLE_H
