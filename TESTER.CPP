#include "Tester.h"

#include <iostream>
#include "Stock.h"
#include "Participant.h"
#include "PricesInfo.h"
#include "Market.h"
#include "TransactionInfo.h"

#include <cmath>

#include <iostream>
#include <fstream>


#include <string>
#include <regex>
#include <iterator>
//#include<iostream>
#include <iomanip>
#include <algorithm>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>

#include <chrono>
#include <memory>
#include <tuple>

namespace Test
{
    std::map<std::string, double> g_mapName_Time;

    void AddTime(std::string sName, double dTime)
    {
        g_mapName_Time[sName]=g_mapName_Time[sName]+dTime;
    }
    double GetTime(std::string sName)
    {
        if(g_mapName_Time.end()!=g_mapName_Time.find(sName))
        {
            return g_mapName_Time.at(sName);
        }
        else
            return 0.0;

    }
    void SetTime(std::string sName, double dTime)
    {
        if(g_mapName_Time.end()!=g_mapName_Time.find(sName))
        {
            g_mapName_Time.at(sName)=dTime;
        }
        else
            g_mapName_Time[sName]=dTime;
    }
}

CTester::CTester()
{
    //ctor


}

CTester::~CTester()
{
    //dtor

}

const bool PRINT = false;
Fl_Input* g_ZoomInput;
Fl_Input* g_ZoomOutput;

//void DrawGraphics(std::map< std::string, std::vector<double> >* pmapGraphics, double dZoom);

void Button_cb(Fl_Widget* w, void* v)
{
    std::cout << "Button_cb" << std::endl;

    Test::DrawGraphics((std::map< std::string, std::vector<double> >*)v, 2.0);
}

void Button2_cb(Fl_Widget* w, void* v)
{
    std::cout << "Button2_cb" << std::endl;

    Test::DrawGraphics((std::map< std::string, std::vector<double> >*)v, 4.0);
}


void Button3_cb(Fl_Widget* w, void* v)
{
    std::cout << "Button3_cb" << std::endl;

    std::string strZoomValue;
    std::ostringstream ossZoomValue;
    ossZoomValue << std::fixed << std::setprecision(5) << g_ZoomInput->value();
    strZoomValue = ossZoomValue.str();

    double dZoom = std::stod(strZoomValue);

    g_ZoomOutput->value(g_ZoomInput->value());

    Test::DrawGraphics((std::map< std::string, std::vector<double> >*)v, dZoom);
}


void Test::DrawGraphics(std::map< std::string, std::vector<double> >* pmapGraphics, double dZoom)
{
    std::map< std::string, std::vector<double> > mapGraphics = (*pmapGraphics);

    Fl_Window* window = new Fl_Window(1100,580);
    window->begin();

    double dMinBound=0.0;
    double dMaxBound=0.0;
    std::vector<Fl_Chart*> vCharts;
    unsigned long color=0;
    long nCount = 0;
    std::vector<std::string> vStrings;
    for(auto & pairStringVector : mapGraphics)
    {

        std::vector<double> vValues = pairStringVector.second;

        double dMin=(*std::min_element(vValues.begin(),vValues.end()));
        double dMax=(*std::max_element(vValues.begin(),vValues.end()));

        if (dMin<dMinBound)
            dMinBound=dMin;

        if (dMax>dMaxBound)
            dMaxBound=dMax;

        Fl_Chart *chart = new Fl_Chart(10,10,1000,350);
        if(PRINT) std::cout << "chart->label(): " << chart->label() << std::endl;
        window->add(chart);
        vCharts.push_back(chart);
        chart->type(FL_LINE_CHART);
        chart->box(FL_NO_BOX);
        for(auto & value:vValues)
        {
            chart->add(value,"",color);
        }

        Fl_Box* boxLabel = new Fl_Box(50,360+20*nCount,200,20,pairStringVector.first.c_str());
        boxLabel->labelcolor(color);
        boxLabel->box(FL_UP_BOX);

        std::string strLastValue;
        std::ostringstream ossLastValue;
        ossLastValue << std::fixed << std::setprecision(5) << "Last value: " << pairStringVector.second.back() << " ";
        strLastValue=ossLastValue.str();
        vStrings.push_back(strLastValue); //El string que se pasa al Fl_Box tiene que existir en el momento de mostrar la ventana
        Fl_Box* boxLabelLastValue = new Fl_Box(250,360+20*nCount,200,20, vStrings.back().c_str() );

        color++;
        nCount++;

    }
    //group->end();

    //Fl_Group* group2 = new Fl_Group(10,510,500,200);


    std::string strMinBound;
    std::ostringstream ossMinBound;
    ossMinBound << std::fixed << std::setprecision(2) << "MinBound: " << dMinBound;
    strMinBound=ossMinBound.str();

    Fl_Box* boxLabelMinBound = new Fl_Box(50,300,200,20,strMinBound.c_str());
    window->add(boxLabelMinBound);

    std::string strMaxBound;
    std::ostringstream ossMaxBound;
    ossMaxBound << std::fixed << std::setprecision(2) << "MaxBound: " << dMaxBound;
    strMaxBound=ossMaxBound.str();

    Fl_Box* boxLabelMaxBound = new Fl_Box(50,320,200,20,strMaxBound.c_str());
    window->add(boxLabelMaxBound);

    for (auto & chart:vCharts)
    {
        chart->bounds(dMinBound/dZoom,dMaxBound/dZoom);
    }

    Fl_Button* button = new Fl_Button(50,500,70,20,"zoom");

    button->callback((Fl_Callback*)Button_cb, pmapGraphics);
    if(PRINT) std::cout << "callback añadido " << std::endl;

    Fl_Button* button2 = new Fl_Button(200,500,70,20,"zoom 2");
    button2->callback((Fl_Callback*)Button2_cb, pmapGraphics);

    Fl_Button* button3 = new Fl_Button(350,500,70,20,"zoom 3");
    button3->callback((Fl_Callback*)Button3_cb, pmapGraphics);

    Fl_Input* pZoomInput = new Fl_Input(500,500,70,20,"Zoom");

    g_ZoomInput = pZoomInput;

    Fl_Input* pZoomOutput = new Fl_Input(650,500,70,20,"Output");

    g_ZoomOutput = pZoomOutput;

    window->end();
    window->show();

    Fl::run();

}

void ChangePointsForColons(std::string sFileOrig,std::string sFileNew)
{
    std::string strReplace = "[.]"; //Regex String to search
    std::string strNew = ",";	//String To replace
    std::ifstream filein(sFileOrig); //File to read from
    std::ofstream fileout(sFileNew); //Temporary file
    if(!filein || !fileout) //if both files are not available
	{
        if(PRINT) std::cout << "Error opening files!" << std::endl;

    }
    else
    {
        std::string strTemp;
        //bool found = false;
        while(filein >> strTemp)//it will check line from test to strTemp string
        {
            strTemp = std::regex_replace(strTemp, std::regex(strReplace), strNew);

            strTemp += "\n";
            fileout << strTemp;//output everything to fileout(temp.txt)
            //if(found) break;
        }
    }
}

void CTester::TestStock()
{
    std::map<def::eProduct,double> mapeProd_dAmount;
    mapeProd_dAmount[def::PROD_CLOTH]=1.0;
    mapeProd_dAmount[def::PROD_FOOD]=1.0;
    mapeProd_dAmount[def::PROD_GOLD]=1.0;

    std::map<def::eProduct,double> mapeProd_dAmount_2={{def::PROD_CLOTH,2.0},
                                                        {def::PROD_FOOD,4.0},
                                                        {def::PROD_GOLD,-5.0}};


    CStock stock = CStock{mapeProd_dAmount};

    CStock stock_2 = CStock{mapeProd_dAmount_2};

    if(PRINT)
    {
        std::cout << "stock_2 before operations: " << std::endl;
        stock_2.PrintInfo();
    }

    if(PRINT) std::cout << "stock_2.GetMaxModule(): " << stock_2.GetMaxModule() << std::endl;

    stock_2 = stock_2/2.0;

    if(PRINT)
    {
        std::cout << "stock_2 after stock_2 = stock_2/2.0; : " << std::endl;
        stock_2.PrintInfo();
    }
}

void CalculatePrice(std::vector<std::unique_ptr<CParticipant>>& vupParticipants, CMarket* pMarket, CPricesInfo* pPricesInfo)
{
    //TODO: Arreglar este método un poco más, y meterlo en Market.cpp, (no sé si como método estático)
    //TODO: Hacer un pequeño ejemplo de varios ciclos, asignándole unos ingresos de productos fijos por ciclo a cada Participant
    //TODO: Hacer una clase factory (o job) muy sencilla
    //TODO: Añadir un metodo sencillo en Participant, para elegir fáctory/job
    //TODO: Mejorar el método de GetSatisfaction para que puntuar que haya variedad de productos en el consumo
    std::vector<double> vFoodValues;
    std::vector<double> vClothValues;
    std::vector<double> vStepsBeoreClothChange;
    std::vector<double> vStepsBeoreFoodChange;
    std::vector<double> vStepsBeoreGoldChange;
    std::vector<double> vGain;

    CStock ostockOldSumOfProdInOutsOfMarket;

    long nNumChangesInTendencyCloth = 0;
    long nNumChangesInTendencyFood = 0;
    long nNumChangesInTendencyGold = 0;
    //hacer el for
    long nMaxIterations=100;
    double dGain = 1.0;
    for(
        int i = 0; i<nMaxIterations; i++)
    {
        //std::cout << "Iteration: " <<i<<std::endl;
        //std::cout << "PricesInfo before changing prices:" <<std::endl;
        //pPricesInfo->PrintInfo();
        double dGenerateTransaction = 0.0;
        CStock ostockOriginalSumOfProdInOutsOfMarket;
        //ostockOriginalSumOfProdInOutsOfMarket.PrintInfo();
        for(auto const& pupParticipant:vupParticipants)
        {
            CParticipant* pParticipant= pupParticipant.get();
            std::unique_ptr<CTransactionInfo> upTrans = CMarket::CalculateTransactionInfoWithParticipant(pMarket, pParticipant,pPricesInfo,&dGenerateTransaction);
            ostockOriginalSumOfProdInOutsOfMarket = upTrans->GetSentToMarket() - upTrans->GetReceivedFromMarket();
            //ostockOriginalSumOfProdInOutsOfMarket.PrintInfo();
        }
        //

        for(auto & pairProd_Name : def::mapeProductNames)
        {
            double dProdFluxIntoMarketInPreviousIteration = ostockOldSumOfProdInOutsOfMarket.GetAmount(pairProd_Name.first);
            double dProdFluxIntoMarket= ostockOriginalSumOfProdInOutsOfMarket.GetAmount(pairProd_Name.first);

            bool bChangeOfTendency =false;
            if(dProdFluxIntoMarketInPreviousIteration*dProdFluxIntoMarket<0)
                bChangeOfTendency = true;

            if(true==bChangeOfTendency)
            {
                if(def::PROD_CLOTH == pairProd_Name.first)
                {
                    nNumChangesInTendencyCloth++;
                }
                if(def::PROD_FOOD == pairProd_Name.first)
                {
                    nNumChangesInTendencyFood++;
                }
                if(def::PROD_GOLD == pairProd_Name.first)
                {
                    nNumChangesInTendencyGold++;
                }
            }
        }

        ostockOldSumOfProdInOutsOfMarket = ostockOriginalSumOfProdInOutsOfMarket;
        //

        double dMaxModule=ostockOriginalSumOfProdInOutsOfMarket.GetMaxModule();
        CStock ostockNormalizedSumOfProdInOutsOfMarket = ostockOriginalSumOfProdInOutsOfMarket/dMaxModule;

        bool bReduceGain = false;
        double dParamMinGain=0.0001;
        if(nNumChangesInTendencyCloth > 1 &&
           nNumChangesInTendencyFood > 1 &&
           nNumChangesInTendencyGold > 1)
        {
            if(dGain>dParamMinGain)
            {
                bReduceGain = true;
                nNumChangesInTendencyCloth = 0;
                nNumChangesInTendencyFood = 0;
                nNumChangesInTendencyGold = 0;
            }
            else
            {
                std::cout << "Exiting loop in cycle: " << i << std::endl;
                break;
            }
        }

        if(bReduceGain)
            dGain = dGain/2.0;

        CStock ostockBalancedSumOfProdInOutsOfMarket = ostockOriginalSumOfProdInOutsOfMarket*dGain;
        //pPricesInfo->SetStockForPrices((*pPricesInfo->GetStockForPricesRef()) + ostockOriginalSumOfProdInOutsOfMarket);
        pPricesInfo->SetStockForPrices(pPricesInfo->GetStockForPrices() + ostockBalancedSumOfProdInOutsOfMarket);

        double dMinOfNewPricesStock = pPricesInfo->GetStockForPrices().GetMin();
        //double dParamMinimumForPriceStock = 20.0+i;
        double dParamMinimumForPriceStock = 1.0;
        if(dMinOfNewPricesStock < dParamMinimumForPriceStock)
        {
            if(dMinOfNewPricesStock > 0.0)
            {
                //pPricesInfo->GetStockForPricesRef()->AddForEachPriceStockProd(20.0);
                CStock stockForPrices = pPricesInfo->GetStockForPrices();
                stockForPrices = stockForPrices*(dParamMinimumForPriceStock/dMinOfNewPricesStock);
                pPricesInfo->SetStockForPrices(stockForPrices);
            }
            else
            {
                CStock stockForPrices = pPricesInfo->GetStockForPrices();
                stockForPrices.AddAmountForEachProd(std::fabs(dMinOfNewPricesStock)+dParamMinimumForPriceStock);
                pPricesInfo->SetStockForPrices(stockForPrices);
            }

        }

        //std::cout << "Iteration_: " <<i<<std::endl;
        //std::cout << "PricesInfo after changing prices:" <<std::endl;
        //pPricesInfo->PrintInfo();

        vFoodValues.push_back(pPricesInfo->GetPriceOfProduct(def::eProduct::PROD_FOOD,def::eProduct::PROD_GOLD));
        vClothValues.push_back(pPricesInfo->GetPriceOfProduct(def::eProduct::PROD_CLOTH,def::eProduct::PROD_GOLD));
        vStepsBeoreClothChange.push_back(nNumChangesInTendencyCloth);
        vStepsBeoreFoodChange.push_back(nNumChangesInTendencyFood);
        vStepsBeoreGoldChange.push_back(nNumChangesInTendencyGold);
        vGain.push_back(dGain);

    }

    std::map<std::string, std::vector<double> > mapStrVDouble;
    mapStrVDouble.insert(std::make_pair("Food",vFoodValues));
    mapStrVDouble.insert(std::make_pair("Cloth",vClothValues));
    //mapStrVDouble.insert(std::make_pair("ClothChange",vStepsBeoreClothChange));
    //mapStrVDouble.insert(std::make_pair("FoodChange",vStepsBeoreFoodChange));
    //mapStrVDouble.insert(std::make_pair("GoldChange",vStepsBeoreGoldChange));
    mapStrVDouble.insert(std::make_pair("Gain",vGain));


    Test::DrawGraphics(&mapStrVDouble,1.0);

}

std::tuple<std::unique_ptr<CMarket>,
        std::unique_ptr<CPricesInfo>,
        std::vector<std::unique_ptr<CParticipant>>
        >
        CreateMarketWithParticipants()
{
    std::map<def::eProduct,double> mapeProd_dAmount;
    mapeProd_dAmount[def::PROD_CLOTH]=1.0;
    mapeProd_dAmount[def::PROD_FOOD]=3.3;
    mapeProd_dAmount[def::PROD_GOLD]=5.0;
    CStock stock(mapeProd_dAmount);
    CPricesInfo* pPricesInfo = new CPricesInfo();
    pPricesInfo->SetStockForPrices(stock);

    CMarket* pMarket = new CMarket();


    std::vector<std::unique_ptr<CParticipant>> vpParticipants;
    for(int i=0;i<5;i++)
    {
        CParticipant* pParticipant = new CParticipant();
        pMarket->AddParticipant(pParticipant);

        std::map<def::eProduct, double> mapeProd_dSatisf;
        mapeProd_dSatisf[def::PROD_CLOTH]=1.1;
        mapeProd_dSatisf[def::PROD_FOOD]=1.0;
        mapeProd_dSatisf[def::PROD_GOLD]=1.0;

        pParticipant->SetProductSatisfactionMap(mapeProd_dSatisf);

        std::map<def::eProduct,double> mapPart_1_eProd_dAmount;
        mapPart_1_eProd_dAmount[def::PROD_CLOTH]=1.0;
        mapPart_1_eProd_dAmount[def::PROD_FOOD]=1.0;
        mapPart_1_eProd_dAmount[def::PROD_GOLD]=1.0;
        CStock stockPart1(mapPart_1_eProd_dAmount);
        pParticipant->SetStock(stockPart1);

        vpParticipants.push_back(std::unique_ptr<CParticipant>(pParticipant));
    }

    std::unique_ptr<CMarket> upMarket(pMarket);
    std::unique_ptr<CPricesInfo> upPricesInfo(pPricesInfo);

    return std::make_tuple(std::move(upMarket),
                           std::move(upPricesInfo),
                           std::move(vpParticipants));
}

void CTester::TestParticipantOfferAndDemand_2()
{

    //auto myTuple = CreateMarketWithParticipants();
    std::unique_ptr<CMarket> upMarket;
    std::unique_ptr<CPricesInfo> upPricesInfo;
    std::vector<std::unique_ptr<CParticipant>> vupParticipants;
    std::tie(upMarket,upPricesInfo,vupParticipants) = CreateMarketWithParticipants();
    //std::tie(upMarket,std::ignore,std::ignore) = std::move(myTuple);

    CalculatePrice(vupParticipants, upMarket.get(), upPricesInfo.get());

    //upPricesInfo.get()->PrintInfo();
    //->

}

void CTester::TestParticipantOfferAndDemand()
{

    CMarket* pMarket = new CMarket();
    CParticipant* pParticipant_1 = new CParticipant();
    pMarket->AddParticipant(pParticipant_1);

    std::map<def::eProduct, double> mapeProd_dSatisf;
    mapeProd_dSatisf[def::PROD_CLOTH]=1.1;
    mapeProd_dSatisf[def::PROD_FOOD]=1.0;
    mapeProd_dSatisf[def::PROD_GOLD]=1.0;

    pParticipant_1->SetProductSatisfactionMap(mapeProd_dSatisf);

    std::map<def::eProduct,double> mapPart_1_eProd_dAmount;
    mapPart_1_eProd_dAmount[def::PROD_CLOTH]=1.0;
    mapPart_1_eProd_dAmount[def::PROD_FOOD]=1.0;
    mapPart_1_eProd_dAmount[def::PROD_GOLD]=1.0;
    CStock stockPart1(mapPart_1_eProd_dAmount);
    pParticipant_1->SetStock(stockPart1);


    std::map<def::eProduct,double> mapeProd_dAmount;
    mapeProd_dAmount[def::PROD_CLOTH]=1.0;
    mapeProd_dAmount[def::PROD_FOOD]=3.3;
    mapeProd_dAmount[def::PROD_GOLD]=5.0;
    CStock stock(mapeProd_dAmount);
    CPricesInfo* pPricesInfo = new CPricesInfo();
    pPricesInfo->SetStockForPrices(stock);


    if(PRINT) pPricesInfo->PrintInfo();


    if(PRINT) std::cout << "pParticipant_1->GetStock(): " << std::endl;
    if(PRINT) pParticipant_1->GetStock().PrintInfo();


    std::ofstream myOfStream;
    myOfStream.open("TesterOutput.txt");



    std::vector<double> vFoodValues;
    std::vector<double> vClothValues;

    std::ofstream priceFofstream;
    priceFofstream.open("PriceF.txt");
    std::ofstream priceCofstream;
    priceCofstream.open("PriceC.txt");
    std::ofstream priceGofstream;
    priceGofstream.open("PriceG.txt");

    CStock ostockOldSumOfProdInOutsOfMarket;
    std::map<def::eProduct, double> mapProd_StepsWithoutChangeOfTendency;
    for(auto & pairProd_Name : def::mapeProductNames)
    {
        mapProd_StepsWithoutChangeOfTendency[pairProd_Name.first]=0;
    }

    std::vector<double> vStepsBeoreClothChange;
    std::vector<double> vStepsBeoreFoodChange;
    std::vector<double> vStepsBeoreGoldChange;

    std::vector<double> vGain;

    double dGain = 1.0;

    bool bChangeInTendencyCloth = false;
    bool bChangeInTendencyFood = false;
    bool bChangeInTendencyGold = false;

    long nNumChangesInTendencyCloth = 0;
    long nNumChangesInTendencyFood = 0;
    long nNumChangesInTendencyGold = 0;


    auto start = std::chrono::system_clock::now();
    //std::cout << "f(42) = " << fibonacci(42) << '\n';

    //std::cout << "finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";

    std::chrono::duration<double> elapsed_seconds_Loop1Total;
    std::chrono::duration<double> elapsed_seconds_Loop2Total;
    std::chrono::duration<double> elapsed_seconds_Loop3Total;
    double dMethodPerformance=0.0;

    for (int i=0;i<3000;i++)
    {
        auto startLoop1 = std::chrono::system_clock::now();

        if(PRINT) myOfStream << "\n";
        if(PRINT) myOfStream << "Iteration: " << i << "\t";

        if(PRINT) std::cout << "Market PriceStock before transaction: " << i << std::endl;

        vFoodValues.push_back(pPricesInfo->GetPriceOfProduct(def::eProduct::PROD_FOOD,def::eProduct::PROD_GOLD));
        vClothValues.push_back(pPricesInfo->GetPriceOfProduct(def::eProduct::PROD_CLOTH,def::eProduct::PROD_GOLD));

        if(PRINT) priceFofstream << pPricesInfo->GetPriceOfProduct(def::eProduct::PROD_FOOD,def::eProduct::PROD_GOLD) << "\n";
        if(PRINT) priceCofstream << pPricesInfo->GetPriceOfProduct(def::eProduct::PROD_CLOTH,def::eProduct::PROD_GOLD) << "\n";
        if(PRINT) priceGofstream << pPricesInfo->GetPriceOfProduct(def::eProduct::PROD_GOLD,def::eProduct::PROD_GOLD) << "\n";

        if(PRINT) myOfStream << "F$S " << pPricesInfo->GetStockForPrices().GetAmount(def::eProduct::PROD_FOOD) << "$\t";
        if(PRINT) myOfStream << "C$S " << pPricesInfo->GetStockForPrices().GetAmount(def::eProduct::PROD_CLOTH) << "$\t";
        if(PRINT) myOfStream << "G$S " << pPricesInfo->GetStockForPrices().GetAmount(def::eProduct::PROD_GOLD) << "$\t";

        //pPricesInfo->PrintInfo();
        if(PRINT) pPricesInfo->GetStockForPrices().PrintInfo();
        //mirar esto mejor

        auto endLoop1 = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds_Loop1 = endLoop1-startLoop1;
        elapsed_seconds_Loop1Total=elapsed_seconds_Loop1Total+elapsed_seconds_Loop1;

        auto startLoop2 = std::chrono::system_clock::now();

        double dGenerateTransaction = 0.0;
        std::unique_ptr<CTransactionInfo> upTrans = CMarket::CalculateTransactionInfoWithParticipant(pMarket, pParticipant_1,pPricesInfo,&dGenerateTransaction);
        dMethodPerformance+=dGenerateTransaction;

        auto endLoop2 = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds_Loop2 = endLoop2-startLoop2;
        elapsed_seconds_Loop2Total=elapsed_seconds_Loop2Total+elapsed_seconds_Loop2;

        CStock ostockOriginalSumOfProdInOutsOfMarket = upTrans->GetSentToMarket() - upTrans->GetReceivedFromMarket();

        for(auto & pairProd_Name : def::mapeProductNames)
        {
            double dProdOldAmount = ostockOldSumOfProdInOutsOfMarket.GetAmount(pairProd_Name.first);
            double dProdOriginalAmount = ostockOriginalSumOfProdInOutsOfMarket.GetAmount(pairProd_Name.first);

            bool bChangeOfTendency =false;
            if(dProdOldAmount*dProdOriginalAmount<0)
                bChangeOfTendency = true;

            if(false==bChangeOfTendency)
            {
                mapProd_StepsWithoutChangeOfTendency[pairProd_Name.first]++;

                if(def::PROD_CLOTH == pairProd_Name.first)
                {
                    bChangeInTendencyCloth = true;
                    nNumChangesInTendencyCloth++;
                }
                if(def::PROD_FOOD == pairProd_Name.first)
                {
                    bChangeInTendencyFood = true;
                    nNumChangesInTendencyFood++;
                }
                if(def::PROD_GOLD == pairProd_Name.first)
                {
                    bChangeInTendencyGold = true;
                    nNumChangesInTendencyGold++;
                }
            }
            else
            {
                mapProd_StepsWithoutChangeOfTendency[pairProd_Name.first]=0;
            }


            if(def::PROD_CLOTH == pairProd_Name.first)
            {
                vStepsBeoreClothChange.push_back(mapProd_StepsWithoutChangeOfTendency[pairProd_Name.first]);
            }
            if(def::PROD_FOOD== pairProd_Name.first)
            {
                vStepsBeoreFoodChange.push_back(mapProd_StepsWithoutChangeOfTendency[pairProd_Name.first]);
            }
            if(def::PROD_GOLD== pairProd_Name.first)
            {
                vStepsBeoreGoldChange.push_back(mapProd_StepsWithoutChangeOfTendency[pairProd_Name.first]);
            }
        }

        ostockOldSumOfProdInOutsOfMarket = ostockOriginalSumOfProdInOutsOfMarket;

        if(PRINT) myOfStream << "FSent " << upTrans->GetSentToMarket().GetAmount(def::eProduct::PROD_FOOD) << "$\t";
        if(PRINT) myOfStream << "CSent " << upTrans->GetSentToMarket().GetAmount(def::eProduct::PROD_CLOTH) << "$\t";
        if(PRINT) myOfStream << "GSent " << upTrans->GetSentToMarket().GetAmount(def::eProduct::PROD_GOLD) << "$\t";

        if(PRINT) myOfStream << "FRece " << upTrans->GetReceivedFromMarket().GetAmount(def::eProduct::PROD_FOOD) << "$\t";
        if(PRINT) myOfStream << "CRece " << upTrans->GetReceivedFromMarket().GetAmount(def::eProduct::PROD_CLOTH) << "$\t";
        if(PRINT) myOfStream << "GRece " << upTrans->GetReceivedFromMarket().GetAmount(def::eProduct::PROD_GOLD) << "$\t";

        if(PRINT) std::cout << "InOutSum of transaction: " << i << std::endl;
        if(PRINT) ostockOriginalSumOfProdInOutsOfMarket.PrintInfo();

        double dMaxModule=ostockOriginalSumOfProdInOutsOfMarket.GetMaxModule();

        CStock ostockNormalizedSumOfProdInOutsOfMarket = ostockOriginalSumOfProdInOutsOfMarket/dMaxModule;

        bool bReduceGain = false;
//        for(auto & pairProd_Name : def::mapeProductNames)
//        {
//            if(0==mapProd_StepsWithoutChangeOfTendency[pairProd_Name.first])
//               bReduceGain = true;
//        }

//        if(bChangeInTendencyCloth &&
//           bChangeInTendencyFood &&
//           bChangeInTendencyGold)
//        {
//            if(dGain>0.00001)
//            {
//                bReduceGain = true;
//                bChangeInTendencyCloth = false;
//                bChangeInTendencyFood = false;
//                bChangeInTendencyGold = false;
//            }
//        }


        double dParamMinGain=0.0001;
        if(nNumChangesInTendencyCloth > 1 &&
           nNumChangesInTendencyFood > 1 &&
           nNumChangesInTendencyGold > 1)
        {
            if(dGain>dParamMinGain)
            {
                bReduceGain = true;
                nNumChangesInTendencyCloth = 0;
                nNumChangesInTendencyFood = 0;
                nNumChangesInTendencyGold = 0;
            }
            else
            {
                std::cout << "Exiting loop in cycle: " << i << std::endl;
                break;
            }
        }

        if(bReduceGain)
            dGain = dGain/2.0;

        if(PRINT) std::cout << "dGain: " << dGain << std::endl;
        vGain.push_back(dGain);

        CStock ostockBalancedSumOfProdInOutsOfMarket = ostockOriginalSumOfProdInOutsOfMarket*dGain;

        //pPricesInfo->SetStockForPrices((*pPricesInfo->GetStockForPricesRef()) + ostockOriginalSumOfProdInOutsOfMarket);
        pPricesInfo->SetStockForPrices(pPricesInfo->GetStockForPrices() + ostockBalancedSumOfProdInOutsOfMarket);

        if(PRINT) std::cout << "Market PriceStock after transaction: " << i << std::endl;
        //pPricesInfo->PrintInfo();
        if(PRINT) pPricesInfo->GetStockForPrices().PrintInfo();

        double dMinOfNewPricesStock = pPricesInfo->GetStockForPrices().GetMin();
        //double dParamMinimumForPriceStock = 20.0+i;
        double dParamMinimumForPriceStock = 1.0;
        if(dMinOfNewPricesStock < dParamMinimumForPriceStock)
        {
            if(dMinOfNewPricesStock > 0.0)
            {
                //pPricesInfo->GetStockForPricesRef()->AddForEachPriceStockProd(20.0);
                CStock stockForPrices = pPricesInfo->GetStockForPrices();
                stockForPrices = stockForPrices*(dParamMinimumForPriceStock/dMinOfNewPricesStock);
                pPricesInfo->SetStockForPrices(stockForPrices);
            }
            else
                pPricesInfo->GetStockForPrices().AddAmountForEachProd(std::fabs(dMinOfNewPricesStock)+dParamMinimumForPriceStock);
        }

        if(PRINT)
        {
            std::cout << "Market PriceStock after AddForEachPriceStockProd: " << i << std::endl;

            pPricesInfo->GetStockForPrices().PrintInfo();
        }
    }

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;

    //std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    std::cout << "elapsed_seconds_Loop1Total: " << elapsed_seconds_Loop1Total.count() << "s\n";
    std::cout << "elapsed_seconds_Loop2Total: " << elapsed_seconds_Loop2Total.count() << "s\n";

    std::cout << "dMethodPerformance: " << dMethodPerformance << "s\n";

    for(auto & pairNameTime:Test::g_mapName_Time)
    {
        std::cout << pairNameTime.first << pairNameTime.second << "s\n";
    }

    myOfStream.close();

    priceFofstream.close();
    priceCofstream.close();
    priceGofstream.close();

    ChangePointsForColons("PriceF.txt","PriceFChanged.txt");
    ChangePointsForColons("PriceC.txt","PriceCChanged.txt");
    ChangePointsForColons("PriceG.txt","PriceGChanged.txt");

    std::map<std::string, std::vector<double> > mapStrVDouble;
    mapStrVDouble.insert(std::make_pair("Food",vFoodValues));
    mapStrVDouble.insert(std::make_pair("Cloth",vClothValues));
    mapStrVDouble.insert(std::make_pair("ClothChange",vStepsBeoreClothChange));
    mapStrVDouble.insert(std::make_pair("FoodChange",vStepsBeoreFoodChange));
    mapStrVDouble.insert(std::make_pair("GoldChange",vStepsBeoreGoldChange));
    mapStrVDouble.insert(std::make_pair("Gain",vGain));


    Test::DrawGraphics(&mapStrVDouble,1.0);

}

