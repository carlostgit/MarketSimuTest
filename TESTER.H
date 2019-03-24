#ifndef CTESTER_H
#define CTESTER_H

#include "Defines.h"

#include <vector>
#include <string>


class CTester
{
public:
    CTester();
    virtual ~CTester();

    void TestStock();
    void TestParticipantOfferAndDemand();
    void TestParticipantOfferAndDemand_2();

    protected:
private:
};

namespace Test
{
    void AddTime(std::string sName, double dTime);
    double GetTime(std::string sName);
    void SetTime(std::string sName, double dTime);

    extern std::map<std::string, double> g_mapName_Time;

    void DrawGraphics(std::map< std::string, std::vector<double> >* pmapGraphics, double dZoom);
}


#endif // CTESTER_H
