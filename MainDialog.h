#ifndef CMAINDIALOG_H
#define CMAINDIALOG_H

#include "defines.h"
#include <vector>
#include <map>
#include <memory>
class CMarket;

class Fl_Widget;
class Fl_Chart;
class Fl_Window;
class Fl_Group;
class Fl_Counter;
class Fl_Check_Button;


class CMainDialog
{

    enum eInfoToShow
    {
        STOCK_SENT = 0,
        STOCK_RECEIVED,
        STOCK_INI,
        STOCK_FIN,
        STOCK_CONSUM,
        STOCK_PROD,
        SATISF,
        NUM_OF_INFO
    };

    const std::vector<int> vconstInfoToShow = {STOCK_SENT, STOCK_RECEIVED, STOCK_INI, STOCK_FIN,STOCK_CONSUM,STOCK_PROD,SATISF};
    const std::map<int,std::string> mapconstInfo_Name = {{STOCK_SENT,"STOCK_SENT"},
                                                            {STOCK_RECEIVED, "STOCK_RECEIVED"},
                                                            {STOCK_INI, "STOCK_INI"},
                                                            {STOCK_FIN,"STOCK_FIN"},
                                                            {STOCK_CONSUM,"STOCK_CONSUM"},
                                                            {STOCK_PROD,"STOCK_PROD"},
                                                            {SATISF,"SATISF"} };


    public:
        CMainDialog();
        virtual ~CMainDialog();
        void run();

        static void Destroy(Fl_Widget* w, void* pvoidMarket);
        static void Reset(Fl_Widget* w, void* pvoidMarket);
        static void Step(Fl_Widget* w, void* pvoidMarket);
        static void LaunchScenario_1(Fl_Widget* w, void* v);
        static void ViewResults(Fl_Widget* w, void* pvoidMainDialog);
        void ViewResults(Fl_Widget* w);
        void ViewResultsOfMarketPlace();
        void ViewResultsOfParticipants();
        std::map<std::pair<int,int>, std::map<def::eProduct, std::vector<double>>> GetParticGraphInfo();

        static void NumPartUpdated(Fl_Widget* w, void* pvoidMainDialog);
        void NumPartUpdated(Fl_Widget* w);

        static void LaunchEditedScenario(Fl_Widget* w, void* pvoidMainDialog);
        void LaunchEditedScenario(Fl_Widget* w);

    protected:

        std::shared_ptr<CMarket> m_pMarket;

        //Market Prices Chart
        //Fl_Chart *m_pChart;
        Fl_Group* m_pMarketPricesChartGroup;
        //double m_dMinChartBound;
        //double m_dMaxChartBound;
        std::string m_sChartMin;
        std::string m_sChartMax;
        Fl_Counter* m_pCounterZoomValue;
        Fl_Counter* m_pCounterFirstStep;
        Fl_Counter* m_pCounterLastStep;

        //Participant Chart
        Fl_Group* m_pParticipantsChartGroup;
        std::map<int,Fl_Check_Button*> m_mapPartId_CheckB;
        std::map<int,std::string> m_mapPartId_sName;
        std::string m_sPartChartMin;
        std::string m_sPartChartMax;

        std::map<int,Fl_Check_Button*> m_mapInfo_cbShow;
        std::map<int,Fl_Check_Button*> m_mapProd_cbShow;

        //
        Fl_Counter* m_pCounterNumPartValue;

        std::map<std::pair<int,def::eProduct>,Fl_Counter*> m_mapPart_Prod_cbShow;

        std::map<std::pair<int,int>,std::string> m_map_pairPartProd_Name;

        Fl_Group* m_pPartValuesGroup;
//
//    std::map<long,CStock> GetStockIniMap(){return m_mapLogStockIni;} //Inicial y final?
//    std::map<long,CStock> GetStockFinMap(){return m_mapLogStockFin;} //Inicial y final?
//    std::map<long,CStock> GetLogConsume(){return m_mapLogConsume;}
//    std::map<long,CStock> GetLogProduction(){return m_mapLogProduction;}
//    std::map<long,CStock> GetLogSatisfaction(){return m_mapLogSatisfaction;} //TODO

        //

        Fl_Window *m_pWindow;

    private:
};

#endif // CMAINDIALOG_H
