#ifndef RESISTOR_H
#define RESISTOR_H

#include "NetlistIdentifier.h"

enum ResistorType
{
    StandardResistor,
    SemiconductorResistor,
    EquationResistor,
    NonLinearResistor,
    NoIdentity,
};

class Resistor : public CircuitComponent
{
    public:
        /** Default constructor */
        Resistor();
        /** Default destructor */
        virtual ~Resistor();
        static wxArrayString IdentifyInformation(wxString information);
        static ResistorType IdentifyResistor(wxArrayString rData);

    protected:

    private:
};

#endif // RESISTOR_H
