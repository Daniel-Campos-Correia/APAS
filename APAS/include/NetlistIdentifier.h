#ifndef NETLISTIDENTIFIER_H
#define NETLISTIDENTIFIER_H


#include "string.h"

#include <map>


// Using dictionaries
using std::map;



enum ComponentTypes
{
    XSPICE_Code_Model,
    ArbitrarySource,
    Capacitor,
    Diode,
    VCVS,
    CCCS,
    VCCS,
    CCVS,
    CurrentSource,
    JFET,
    MutualInductors,
    Inductor,
    MOSFET,
    NumericalDeviceGSS,
    LossyTL,
    CPL,
    BJT,
    Resistor,
    SwitchVC,
    LosslessTL,
    UniformRC_Line,
    VoltageSource,
    SwitchCC,
    SubCircuit,
    SingleLossyTL,
    MESFET
};

class CircuitComponent
{
    public:
        static wxArrayString IdentifyInformation(wxString information);

    protected:

    private:

};






class NetlistIdentifier
{
    typedef std::map<char, ComponentTypes> MapCharComponentTypes;

    public:
        NetlistIdentifier();
        typedef std::map<wxString, std::pair<ComponentTypes, wxArrayString> > ComponentsList;
        typedef std::pair<ComponentTypes, wxArrayString> ComponentInfo;
        static void UpdateComponentsList(wxListBox* myListBox, wxTextCtrl* file);
        static MapCharComponentTypes SpiceComponents;
        static ComponentsList componentsList;
        static wxArrayString SerializeData(wxString data);

    protected:
        static void CountComponents(wxTextCtrl* file);
        static void ProcessComponent(ComponentTypes type, wxString info);

    private:
};

#endif // NETLISTIDENTIFIER_H
