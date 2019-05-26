#ifndef CMAINDIALOG_H
#define CMAINDIALOG_H

#include "defines.h"
#include <vector>
#include <map>
#include <memory>

#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Window.H>

class Fl_Widget;
class Fl_Chart;
//class Fl_Window;
//class Fl_Scroll;
//class Fl_Group;

class Fl_Counter;
class Fl_Check_Button;
class Fl_Box;
class Fl_Button;

class CMarket;
class CStock;
class CParticipant;

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
        static void Calculator(Fl_Widget* w, void* pvoidMainDialog);
        void Calculator(Fl_Widget* w);
        static void Reset(Fl_Widget* w, void* pvoidMarket);
        static void Step(Fl_Widget* w, void* pvoidMarket);
        static void LaunchScenario_1(Fl_Widget* w, void* v);
        static void ViewResults(Fl_Widget* w, void* pvoidMainDialog);
        void ViewResults(Fl_Widget* w);
        void ViewResultsOfMarketPlace();
        void ViewResultsOfParticipants();
        std::map<std::pair<int,int>, std::map<def::eProduct, std::vector<double>>> GetParticGraphInfo();
        std::vector<double> GetValuesFor(int nPartId,int nTypeInfo,def::eProduct eProduct);

        static void NumPartUpdated(Fl_Widget* w, void* pvoidMainDialog);
        void NumPartUpdated(Fl_Widget* w);

        static void LaunchEditedScenario(Fl_Widget* w, void* pvoidMainDialog);
        void LaunchEditedScenario(Fl_Widget* w);

    protected:

        std::shared_ptr<CMarket> m_pMarket;

        //Market Prices Chart
        Fl_Group* m_pMarketPricesChartGroup;
        std::string m_sChartMin;
        std::string m_sChartMax;
        Fl_Counter* m_pCounterZoomValue;
        Fl_Counter* m_pCounterFirstStep;
        Fl_Counter* m_pCounterLastStep;

        //Participant Chart
        //Fl_Group* m_pParticipantsChartGroup;

        struct StructPartiChartGroup: public Fl_Scroll
        {
            StructPartiChartGroup();
            StructPartiChartGroup(int x, int y, int w, int h, const char* c=0);

            bool ViewParticipant(int nPartId) const;
            std::string GetNameOfParticipant(int nPartId);

            std::vector<double> GetValuesFor(CMarket* pMarket,int nPartId, int nTypeInfo,def::eProduct eProduct); //todo

            void InitCharts(
                           const std::map<int,Fl_Check_Button*> mapInfo_cbShow,
                            const std::map<int,Fl_Check_Button*> mapProd_cbShow,
                            const StructPartiChartGroup* psPartiChartGroupOld,
                            const std::map<def::eProduct,std::string>& mapProdNames,
                            CMarket* pMarket,
                            const CMainDialog* pMainDialog
                           );


            struct StructCheckButtons
            {
                std::map<int,Fl_Check_Button*> mapPartId_CheckB;
                std::map<int,std::string> mapPartId_sName;
                bool IsChecked(int nPartId) const;
                std::string GetName(int nPartId);

            } sCheckButtons;

            std::vector<Fl_Chart*> vCharts;
            double dMinBound;
            double dMaxBound;

            std::string sChartMin;
            std::string sChartMax;

        }* m_psPartiChartGroup;

        //CheckBoxes de información que quiero visulizar
        std::map<int,Fl_Check_Button*> m_mapInfo_cbShow;
        std::map<int,Fl_Check_Button*> m_mapProd_cbShow;

        //Número de participantes para las condiciones iniciales
        Fl_Counter* m_pCounterNumPartValue;

        //Precios iniciales
        struct StructInitPrices
        {
            std::map<def::eProduct, double> GetInitPricesMap();
            std::map<def::eProduct,Fl_Counter*> m_mapProd_cInitPrice;
        } m_sInitPrices;


        //Controles para condiciones iniciales:
        struct StructPartiValuesGroup: public Fl_Scroll
        {
            StructPartiValuesGroup():Fl_Scroll(0,0,100,100){;}
            StructPartiValuesGroup(int x, int y, int w, int h, const char* c=0):
                Fl_Scroll(x,y,w,h,c){;}

            void InitPartiValuesControls(StructPartiValuesGroup* pPartValuesGroupNew,
                                        long nNumOfParticipants,
                                        StructPartiValuesGroup* pPartValuesGroupOld
                                        );


            CStock GetInitStock(int nPartIndex);
            std::map<def::eProduct, double> GetPrCapacityMap(int nPartIndex);
            std::map<def::eProduct, double> GetSatisfactionMap(int nPartIndex);
            std::map<def::eProduct, double> GetConsumptionMap(int nPartIndex);

            std::map<std::pair<int,def::eProduct>,Fl_Counter*> m_mapPart_Prod_InitStockCounter;
            std::map<std::pair<int,def::eProduct>,Fl_Counter*> m_mapPart_Prod_ProductionCounter;
            std::map<std::pair<int,def::eProduct>,Fl_Counter*> m_mapPart_Prod_SatisfactionCounter;
            std::map<std::pair<int,def::eProduct>,Fl_Counter*> m_mapPart_Prod_ConsumptionCounter;
            std::map<std::pair<int,int>,std::string> m_map_pairPartProd_Name;
        }* m_psPartiValuesGroup;

        //

        //Calculadora
        struct StructCalculator: public Fl_Window
        {

            StructCalculator();//:Fl_Window(0,0,1000,1000)

            static void CalcSatisf(Fl_Widget* w, void* pvoidStructCalculator);
            void CalcSatisf();
            static void CalcConsumpt(Fl_Widget* w, void* pvoidStructCalculator);
            void CalcConsumpt();
            static void CalcProduction(Fl_Widget* w, void* pvoidStructCalculator);
            void CalcProduction();

            struct TestMarketWithParticipant
            {
                TestMarketWithParticipant(StructCalculator* pStructCalc);
                ~TestMarketWithParticipant();
                CParticipant* GetParticipantRef(){return m_pParticipant;}
                CMarket* GetMarketRef(){return m_pMarket;}

                CParticipant *m_pParticipant;
                CMarket *m_pMarket;
            };

            //std::map<def::eProduct,Fl_Counter*> m_mapPrices;
            std::map<def::eProduct,Fl_Counter*> m_mapStock;
            //std::map<def::eProduct,Fl_Counter*> m_mapNewProducts;

            Fl_Box* m_pBoxOfResults;

            Fl_Box* m_pBoxPrices;
            Fl_Box* m_pBoxStock;
            Fl_Box* m_pBoxProduction;
            Fl_Box* m_pBoxConsumption;
            Fl_Box* m_pBoxSatisfaction;
            Fl_Box* m_pBox6;

            std::map<def::eProduct,Fl_Counter*> m_mapProd_CounterPrices;
            std::map<def::eProduct,Fl_Counter*> m_mapProd_CounterStock;
            std::map<def::eProduct,Fl_Counter*> m_mapProd_CounterProduction;
            std::map<def::eProduct,Fl_Counter*> m_mapProd_CounterConsumption;
            std::map<def::eProduct,Fl_Counter*> m_mapProd_CounterSatisfaction;
            std::map<def::eProduct,Fl_Counter*> m_mapProd_Counter6;

            Fl_Button* m_butCalcSatisf;
            Fl_Button* m_butCalcConsumpt;
            Fl_Button* m_butCalcProduction;

        }* m_psCalculator;
        //

        Fl_Window *m_pWindow;
        Fl_Scroll *m_pScroll;

    private:
};

#endif // CMAINDIALOG_H
