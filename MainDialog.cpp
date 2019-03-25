#include "MainDialog.h"

#include <iostream>
#include <set>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.h>

#include "Market.h"
#include "Participant.h"
#include "PricesInfo.h"
#include "Ledger.h"



CMainDialog::CMainDialog():
    m_pMarketPricesChartGroup(nullptr),m_pWindow(nullptr),
    //m_dMinChartBound(0.0),
    //m_dMaxChartBound(0.0),
    m_sChartMin(""),
    m_sChartMax(""),
    m_pCounterZoomValue(nullptr),
    m_pCounterFirstStep(nullptr),
    m_pCounterLastStep(nullptr),
    m_pPartValuesGroup(nullptr)
{
    //ctor
}

CMainDialog::~CMainDialog()
{
    //dtor
}


void CMainDialog::NumPartUpdated(Fl_Widget* w, void* pvoidMainDialog)
{
    CMainDialog* pMainDialog = static_cast<CMainDialog*>(pvoidMainDialog);
    pMainDialog->NumPartUpdated(w);
}

void CMainDialog::NumPartUpdated(Fl_Widget* w)
{
    std::cout << "NumPartUpdated" << std::endl;


    Fl_Group* pPartValuesGroupNew = new Fl_Group(700,20,200,800);


    std::map<std::pair<int,def::eProduct>,Fl_Counter*> newmapPart_Prod_InitStockCounter;
    std::map<std::pair<int,def::eProduct>,Fl_Counter*> newmapPart_Prod_ProductionCounter;

    long nYLast=20;
    int nPart=m_pCounterNumPartValue->value();
    for(int iPart=0;iPart<nPart;iPart++)
    {

        for(auto& prod:def::vProducts)
        {


            //Init Amounts
            Fl_Counter* pCountInit = new Fl_Counter(700,nYLast,100,20);

            pCountInit->value(2.0);

            if(m_mapPart_Prod_InitStockCounter.end()!=m_mapPart_Prod_InitStockCounter.find(std::make_pair(iPart,static_cast<def::eProduct>(prod))))
            {
                Fl_Counter* pCountOld = m_mapPart_Prod_InitStockCounter.at(std::make_pair(iPart,static_cast<def::eProduct>(prod)));
                pCountInit->value(pCountOld->value());
            }

            std::string sName=std::to_string(iPart)+ " " +def::mapeProductNames.at(prod);
            m_map_pairPartProd_Name[std::make_pair(iPart,prod)]=sName;
            pCountInit->label(m_map_pairPartProd_Name.at(std::make_pair(iPart,prod)).c_str());

            pPartValuesGroupNew->add(pCountInit);

            newmapPart_Prod_InitStockCounter[std::make_pair(iPart,static_cast<def::eProduct>(prod))] = pCountInit;
            //

            //Production Amounts
            Fl_Counter* pCountProd = new Fl_Counter(800,nYLast,100,20);
            //nYLast = pCountProd->y()+pCountProd->h()+20;
            pCountProd->value(2.0);

            if(m_mapPart_Prod_ProductionCounter.end()!=m_mapPart_Prod_ProductionCounter.find(std::make_pair(iPart,static_cast<def::eProduct>(prod))))
            {
                Fl_Counter* pCountOld = m_mapPart_Prod_ProductionCounter.at(std::make_pair(iPart,static_cast<def::eProduct>(prod)));
                pCountProd->value(pCountOld->value());
            }

            //std::string sName=std::to_string(iPart)+ " " +def::mapeProductNames.at(prod);
            //m_map_pairPartProd_Name[std::make_pair(iPart,prod)]=sName;
            pCountProd->label(m_map_pairPartProd_Name.at(std::make_pair(iPart,prod)).c_str());

            pPartValuesGroupNew->add(pCountProd);

            newmapPart_Prod_ProductionCounter[std::make_pair(iPart,static_cast<def::eProduct>(prod))] = pCountProd;
            //


            nYLast = pCountInit->y()+pCountInit->h()+20;


        }

    }

    m_mapPart_Prod_InitStockCounter = newmapPart_Prod_InitStockCounter;
    this->m_mapPart_Prod_ProductionCounter = newmapPart_Prod_ProductionCounter;

    std::cout << "2 m_mapPart_Prod_InitStockCounter.size()" << m_mapPart_Prod_InitStockCounter.size() << std::endl;

    m_pWindow->add(pPartValuesGroupNew);
    std::cout << "NumPartUpdated antes del clear" << std::endl;

    if (m_pPartValuesGroup)
    {
        m_pPartValuesGroup->hide();
        m_pPartValuesGroup->show();
        m_pPartValuesGroup->clear();
        m_pPartValuesGroup = nullptr;
    }

    std::cout << "NumPartUpdated despues del clear" << std::endl;



    m_pPartValuesGroup = pPartValuesGroupNew;

    //m_pWindow->add(pPartValuesGroupNew);

    std::cout << "m_pWindow->add(pPartValuesGroupNew);" << std::endl;

}



void CMainDialog::Destroy(Fl_Widget* w, void* pvoidMarket)
{
    std::cout << "Destroy" << std::endl;
    std::shared_ptr<CMarket>* pupMarket = static_cast<std::shared_ptr<CMarket>*>(pvoidMarket);
    (*pupMarket).reset();
}



void CMainDialog::Reset(Fl_Widget* w, void* pvoidMarket)
{
    std::cout << "Reset" << std::endl;
    std::shared_ptr<CMarket>* pupMarket = static_cast<std::shared_ptr<CMarket>*>(pvoidMarket);
    (*pupMarket).reset(new CMarket());
}


void CMainDialog::Step(Fl_Widget* w, void* pvoidMarket)
{
    std::cout << "Step" << std::endl;
    std::shared_ptr<CMarket>* pupMarket = static_cast<std::shared_ptr<CMarket>*>(pvoidMarket);

    CMarket* pMarket=pupMarket->get();

    if(pMarket)
        pMarket->Step();
}


void CMainDialog::LaunchEditedScenario(Fl_Widget* w, void* pvoidMainDialog)
{
    CMainDialog* pMainDialog = static_cast<CMainDialog*>(pvoidMainDialog);
    pMainDialog->LaunchEditedScenario(w);
}

void CMainDialog::LaunchEditedScenario(Fl_Widget* w)
{

    //std::shared_ptr<CMarket>
    m_pMarket = std::shared_ptr<CMarket>(new CMarket());

//    struct struct_Info
//    {
//        long iPart;
//        def::eProduct eProd;
//        double dNumOfProd;
//    } strInfo;

    double debugSize = m_mapPart_Prod_InitStockCounter.size();
    std::cout << "4 m_mapPart_Prod_InitStockCounter.size()" << m_mapPart_Prod_InitStockCounter.size() << std::endl;

    std::set<int> setPartIndex;
    for (auto & pairPartProd_cbShow:m_mapPart_Prod_InitStockCounter)
    {
        setPartIndex.insert(pairPartProd_cbShow.first.first);
    }

    for (auto & iPart:setPartIndex)
    {
        std::map<def::eProduct,double> mapProd_Amount;
        //long iPart = pairPartIndex_Info.first;
        for (auto & pairPartProd_cbShow:m_mapPart_Prod_InitStockCounter)
        {
            std::pair<int,def::eProduct> pairPartProd = pairPartProd_cbShow.first;
            Fl_Counter* pflCounter = pairPartProd_cbShow.second;
            double dAmount = pflCounter->value();
            mapProd_Amount[pairPartProd.second]=dAmount;
        }


        CParticipant* pParticipant = new CParticipant();

        CStock stockPart(mapProd_Amount);
        pParticipant->SetStock(stockPart);
        pParticipant->SetProductSatisfactionMap(mapProd_Amount);
        pParticipant->SetProductConsumptionMap(mapProd_Amount);
        pParticipant->SetProductPrCapacityMap(mapProd_Amount);

        m_pMarket->AddParticipant(pParticipant);
    }

    std::map<def::eProduct,double> mapeProd_dAmount;
    mapeProd_dAmount[def::PROD_CLOTH]=1.0;
    mapeProd_dAmount[def::PROD_FOOD]=1.0;
    mapeProd_dAmount[def::PROD_GOLD]=1.0;
    CStock stock(mapeProd_dAmount);

    CPricesInfo pricesInfo;
    pricesInfo.SetStockForPrices(stock);
    m_pMarket->SetPricesInfo(pricesInfo);


}

void CMainDialog::LaunchScenario_1(Fl_Widget* w, void* pvoidMarket)
{
    std::cout << "LaunchScenario_1" << std::endl;
    std::shared_ptr<CMarket>* pupMarket = static_cast<std::shared_ptr<CMarket>*>(pvoidMarket);


    std::shared_ptr<CMarket> upMarketCopy = *pupMarket;

    CMarket* pMarket=upMarketCopy.get();

    if(!pMarket)
        return;

    //pMarket = new CMarket();
    CParticipant* pParticipant_1 = new CParticipant();
    CParticipant* pParticipant_2 = new CParticipant();
    //CLedger* pLedger = new CLedger();
    //pMarket->SetLedger(pLedger);
    pMarket->AddParticipant(pParticipant_1);
    pMarket->AddParticipant(pParticipant_2);

    std::map<def::eProduct,double> mapPart_1_eProd_dAmount;
    mapPart_1_eProd_dAmount[def::PROD_CLOTH]=1.0;
    mapPart_1_eProd_dAmount[def::PROD_FOOD]=2.0;
    mapPart_1_eProd_dAmount[def::PROD_GOLD]=1.0;
    CStock stockPart1(mapPart_1_eProd_dAmount);
    pParticipant_1->SetStock(stockPart1);

    std::map<def::eProduct,double> mapPart_2_eProd_dAmount;
    mapPart_2_eProd_dAmount[def::PROD_CLOTH]=1.2;
    mapPart_2_eProd_dAmount[def::PROD_FOOD]=1.0;
    mapPart_2_eProd_dAmount[def::PROD_GOLD]=1.0;
    CStock stockPart2(mapPart_2_eProd_dAmount);
    pParticipant_2->SetStock(stockPart2);

    std::map<def::eProduct,double> mapeProd_dAmount;
    mapeProd_dAmount[def::PROD_CLOTH]=1.0;
    mapeProd_dAmount[def::PROD_FOOD]=1.0;
    mapeProd_dAmount[def::PROD_GOLD]=1.0;

    CStock stock(mapeProd_dAmount);

    CPricesInfo pricesInfo;
    pricesInfo.SetStockForPrices(stock);
    pMarket->SetPricesInfo(pricesInfo);

    std::map<def::eProduct, double> mapeProd_dSatisf;
    mapeProd_dSatisf[def::PROD_CLOTH]=1.0;
    mapeProd_dSatisf[def::PROD_FOOD]=1.0;
    mapeProd_dSatisf[def::PROD_GOLD]=1.0;

    pParticipant_1->SetProductSatisfactionMap(mapeProd_dSatisf);
    pParticipant_2->SetProductSatisfactionMap(mapeProd_dSatisf);

    std::map<def::eProduct, double> mapeProd_dConsumpt;
    mapeProd_dConsumpt[def::PROD_CLOTH]=0.1;
    mapeProd_dConsumpt[def::PROD_FOOD]=0.1;
    mapeProd_dConsumpt[def::PROD_GOLD]=0.01;

    pParticipant_1->SetProductConsumptionMap(mapeProd_dConsumpt);
    pParticipant_2->SetProductConsumptionMap(mapeProd_dConsumpt);

    std::map<def::eProduct, double> mapeProd_dProduction;
    mapeProd_dProduction[def::PROD_CLOTH]=0.2;
    mapeProd_dProduction[def::PROD_FOOD]=0.3;
    mapeProd_dProduction[def::PROD_GOLD]=0.01;

    pParticipant_1->SetProductPrCapacityMap(mapeProd_dProduction);
    pParticipant_2->SetProductPrCapacityMap(mapeProd_dProduction);

    for (int i=0;i<1;i++)
    {
        pMarket->Step();
    }


    //Este método no hará el delete
//    delete pMarket;
//    delete pParticipant_1;
//    delete pParticipant_2;
    //delete pLedger;


    //Test::DrawGraphics((std::map< std::string, std::vector<double> >*)v, 2.0);
}


void CMainDialog::ViewResults(Fl_Widget* w, void* pvoidMainDialog)
{
    CMainDialog* pMainDialog = static_cast<CMainDialog*>(pvoidMainDialog);
    pMainDialog->ViewResults(w);
}

void CMainDialog::ViewResults(Fl_Widget* w)
{
    ViewResultsOfMarketPlace();
    ViewResultsOfParticipants();
}

void CMainDialog::ViewResultsOfMarketPlace()
{

    std::cout << "ViewResults" << std::endl;

    //std::cout << "typeid of v: " << typeid(v).name() << std::endl;

    m_pMarketPricesChartGroup->clear();

    m_pMarketPricesChartGroup->hide();
    m_pMarketPricesChartGroup->show();


    double dMinBound=0.0;
    double dMaxBound=0.0;
    std::vector<Fl_Chart*> vCharts;

    std::vector<std::string> vStrings;

    //std::vector<SLedgerInfo> vsLedgerInfo = m_pMarket->GetLedgerRef()->GetInfo();

    std::vector<CPricesInfo*> vPricesInfo = m_pMarket->GetLedgerRef()->GetPricesOfEachStep();

    if(m_pCounterLastStep->value()>vPricesInfo.size())
        m_pCounterLastStep->value(vPricesInfo.size()-1);

    int nCount=0;

    for(auto & prod:def::vProducts)
    {
        if(false==m_mapProd_cbShow.at(prod)->value())
            continue;


        Fl_Chart* pChart = new Fl_Chart(m_pMarketPricesChartGroup->x(),m_pMarketPricesChartGroup->y(),m_pMarketPricesChartGroup->w(),m_pMarketPricesChartGroup->h());
        m_pMarketPricesChartGroup->add(pChart );
        vCharts.push_back(pChart);

        pChart->type(FL_LINE_CHART);
        pChart->box(FL_NO_BOX);

        def::eProduct eProd = prod;
        std::string strName = def::mapeProductNames.at(eProd);

        //pChart->color(color);
        nCount++;
        unsigned long color=FL_BLUE+10*nCount;

        int nStep=-1;
        for(auto & pricesInfo:vPricesInfo)
        {
            nStep++;
            if(nStep<m_pCounterFirstStep->value() ||
               nStep>m_pCounterLastStep->value()
               )
            {
                continue;
            }

            double dValue=pricesInfo->GetPrice(eProd);
            if(dValue>dMaxBound)
                dMaxBound=dValue;

            pChart->add(pricesInfo->GetPrice(eProd),def::mapeProductNames.at(eProd).c_str(),color);
        }
    }

    //Fl_Counter* m_pCounterFirstStep;
    //Fl_Counter* m_pCounterLastStep;

    double dMinChartBoundAdjusted=dMinBound;
    double dMaxChartBoundAdjusted=dMaxBound/m_pCounterZoomValue->value();

    for(auto & pChart:vCharts)
    {
        pChart->bounds(dMinChartBoundAdjusted,dMaxChartBoundAdjusted);
    }

    Fl_Box *pBoxMin = new Fl_Box(m_pMarketPricesChartGroup->x()-10,
                                  m_pMarketPricesChartGroup->y()+m_pMarketPricesChartGroup->h(),
                                  20, 20 );
    Fl_Box *pBoxMax = new Fl_Box(m_pMarketPricesChartGroup->x()-10,
                                  m_pMarketPricesChartGroup->y()-20,
                                  20, 20 );


    m_sChartMin=std::to_string(dMinChartBoundAdjusted);
    m_sChartMax=std::to_string(dMaxChartBoundAdjusted);

    pBoxMin->label(m_sChartMin.c_str());
    pBoxMax->label(m_sChartMax.c_str());

    m_pMarketPricesChartGroup->add(pBoxMin);
    m_pMarketPricesChartGroup->add(pBoxMax);

    m_pMarketPricesChartGroup->end();
    std::cout<< "nCount: "<<nCount<<"\n";
}

void CMainDialog::ViewResultsOfParticipants()
{

    m_pParticipantsChartGroup->begin();

    m_pParticipantsChartGroup->clear();

    std::map<int,std::vector<CStock>> mapPartId_vStocksSent =
                this->m_pMarket->GetLedgerRef()->GetSentToMarketForEachStep();


    double dMinBound=0.0;
    double dMaxBound=0.0;


    std::vector<Fl_Chart*> vCharts;

    long nCount=0;

    std::map<std::pair<int,int>, std::map<def::eProduct, std::vector<double>>> mapPart_Elem_Prod_Values = GetParticGraphInfo();

    for (auto & pairPart_Prod_Values:mapPart_Elem_Prod_Values)
    {
        int nPartId=pairPart_Prod_Values.first.first;
        int nInfoType=pairPart_Prod_Values.first.second;

        for (auto & prod:def::vProducts)
        {
            std::map<def::eProduct, std::vector<double>> mapProdValues = pairPart_Prod_Values.second;
            if(mapProdValues.end()==mapProdValues.find(prod))
                continue;

            if(false==m_mapProd_cbShow.at(prod)->value())
                continue;


            std::vector<double> vValues = mapProdValues.at(prod);

            std::cout<<"GraphInf Prod: "<< prod << std::endl;
            std::cout<<"GraphInf nInfoType: "<< nInfoType << std::endl;
            std::cout<<"GraphInf nPartId: "<< nPartId << std::endl;

            if(m_mapPartId_CheckB.end()!=m_mapPartId_CheckB.find(nPartId))
            {
                Fl_Check_Button* pCheckButton = m_mapPartId_CheckB.at(nPartId);
                if(pCheckButton->value()==0)
                    continue;
            }

            if(m_mapInfo_cbShow.end()!=m_mapInfo_cbShow.find(nInfoType))
            {
                Fl_Check_Button* pCheckButton = m_mapInfo_cbShow.at(nInfoType);
                if(pCheckButton->value()==0)
                    continue;
            }

            Fl_Chart* pChart = new Fl_Chart(m_pParticipantsChartGroup->x(),m_pParticipantsChartGroup->y(),
                                            m_pParticipantsChartGroup->w(),m_pParticipantsChartGroup->h());
            pChart->type(FL_LINE_CHART);
            pChart->box(FL_NO_BOX);

            vCharts.push_back(pChart);

            for (auto & value:vValues)
            {
                unsigned long color=FL_RED+10*nCount;
                nCount++;

                //std::cout<<"vStocks.size(): "<< vStocks.size()<<std::endl;
                double dValue=value;
                if(dValue>dMaxBound)
                    dMaxBound=dValue;

                pChart->add(dValue,def::mapeProductNames.at(prod).c_str(),color);
            }
        }
    }

    for (auto& pChart:vCharts)
    {
        pChart->bounds(dMinBound,dMaxBound);
    }

    Fl_Box *pBoxMin = new Fl_Box(m_pParticipantsChartGroup->x()-10,
                                  m_pParticipantsChartGroup->y()+m_pParticipantsChartGroup->h(),
                                  20, 20 );
    Fl_Box *pBoxMax = new Fl_Box(m_pParticipantsChartGroup->x()-10,
                                  m_pParticipantsChartGroup->y()-20,
                                  20, 20 );


    m_sPartChartMin = std::to_string(dMinBound);
    m_sPartChartMax = std::to_string(dMaxBound);

    pBoxMin->label(m_sPartChartMin.c_str());
    pBoxMax->label(m_sPartChartMax.c_str());


    int nCountPart=0;
    for(auto& part:m_pMarket->GetParticipants())
    {

        int nPartId = part->GetId();

        std::cout << "nPartId " << nPartId << std::endl;
        std::cout << "nCountPart " << nCountPart << std::endl;

        Fl_Check_Button* pCheckButton = new Fl_Check_Button(m_pParticipantsChartGroup->x()+nCountPart*100,
                                                       m_pParticipantsChartGroup->y()+m_pParticipantsChartGroup->h()+20,
                                                       100,20);


        pCheckButton->callback((Fl_Callback*)CMainDialog::ViewResults, (void*)this);
        //static std::string nsId = std::to_string(nPartId);

        if(m_mapPartId_CheckB.end()==m_mapPartId_CheckB.find(nPartId))
        {

            m_mapPartId_sName[nPartId]=std::to_string(nPartId);

            pCheckButton->label(m_mapPartId_sName.at(nPartId).c_str());

            this->m_pWindow->add(pCheckButton);

            m_mapPartId_CheckB[nPartId] = pCheckButton;

            pCheckButton->value(1);
        }

        //pCheckButton->label(m_mapPartId_CheckB.)

        nCountPart++;
    }


    m_pParticipantsChartGroup->end();

}

std::map<std::pair<int,int>, std::map<def::eProduct, std::vector<double>>> CMainDialog::GetParticGraphInfo()
{
    std::map<std::pair<int,int>, std::map<def::eProduct, std::vector<double>>> mapPart_Element_Prod_Values;

    std::map<int,std::vector<CStock>> mapPartId_vStocksSent =
                this->m_pMarket->GetLedgerRef()->GetSentToMarketForEachStep();

    std::map<int,std::vector<CStock>> mapPartId_vStocksReceived =
                this->m_pMarket->GetLedgerRef()->GetReceivedFromMarketEachStep();

    auto lfMapValuesAdder = [&mapPart_Element_Prod_Values,this](def::eProduct eProd, int nElem, int nPart, double dValue)
    {
        //std::string sName = "Part " + std::to_string(pairPartId_vStockSent.first)+def::mapeProductNames.at(prod)+" Sent";
        //std::string sName = sGraphName;
        int nPartId=nPart;
        m_mapPartId_sName[nPartId]=std::to_string(nPartId);
        if(mapPart_Element_Prod_Values.end()==mapPart_Element_Prod_Values.find(std::make_pair(nPart,nElem)))
        {
//            std::cout<<"newPartElem"<< std::endl;
            std::map<def::eProduct, std::vector<double>> mapProd_Values;
            mapProd_Values[eProd]=std::vector<double> {dValue};
            mapPart_Element_Prod_Values[std::make_pair(nPart,nElem)]= mapProd_Values;
        }
        else
        {
            std::map<def::eProduct, std::vector<double>> mapProd_Values = mapPart_Element_Prod_Values.at(std::make_pair(nPart,nElem));

            if(mapProd_Values.end()==mapProd_Values.find(eProd))
            {
//                std::cout<<"newProd"<< eProd << std::endl;
                mapProd_Values[eProd]=std::vector<double> {dValue};
                mapPart_Element_Prod_Values[std::make_pair(nPart,nElem)]= mapProd_Values;

            }
            else
            {
//                std::cout<<"adding values to prod"<< eProd << std::endl;
                mapProd_Values.at(eProd).push_back(dValue);
                mapPart_Element_Prod_Values[std::make_pair(nPart,nElem)]= mapProd_Values;
            }
        }

    };

    for(auto & prod:def::vProducts)
    {
        for(auto & pairPartId_vStockSent:mapPartId_vStocksSent)
        {
            for(auto& stock:pairPartId_vStockSent.second)
            {
                double dAmount = stock.GetAmount(prod);
                //std::string sName = "Part " + std::to_string(pairPartId_vStockSent.first)+def::mapeProductNames.at(prod)+" Sent";
                lfMapValuesAdder(prod,eInfoToShow::STOCK_SENT, pairPartId_vStockSent.first,dAmount);
            }
        }
        for(auto & pairPartId_vStockRec:mapPartId_vStocksReceived)
        {
            int nPartId = pairPartId_vStockRec.first;
            for(auto& stock:pairPartId_vStockRec.second)
            {
                double dAmount = stock.GetAmount(prod);
                //std::string sName = "Part " + std::to_string(nPartId)+def::mapeProductNames.at(prod)+" Rec";
                lfMapValuesAdder(prod,eInfoToShow::STOCK_RECEIVED,nPartId,dAmount);
            }
        }


        auto lfAddMapStepStock = [&lfMapValuesAdder,this](int nPartId, int nElem, def::eProduct eProd, std::map<long,CStock> mapStepStock)
        {
            for(auto & pairStep_Stock:mapStepStock)
            {
                double dAmount = pairStep_Stock.second.GetAmount(eProd);
                //std::string sName = "Part " +
                //        std::to_string(nPartId)+
                //        def::mapeProductNames.at(eProd)+mapconstInfo_Name.at(nElem);
                lfMapValuesAdder(eProd,nElem,nPartId,dAmount);

            }
        };

        for(auto & pPart:m_pMarket->GetParticipants())
        {
            int nPartId = pPart->GetId();
            lfAddMapStepStock(nPartId,eInfoToShow::STOCK_INI,prod,pPart->GetStockIniMap());

            lfAddMapStepStock(nPartId,eInfoToShow::STOCK_FIN,prod,pPart->GetStockFinMap());
            lfAddMapStepStock(nPartId,eInfoToShow::STOCK_CONSUM,prod,pPart->GetLogConsume());
            lfAddMapStepStock(nPartId,eInfoToShow::STOCK_PROD,prod,pPart->GetLogProduction());
            lfAddMapStepStock(nPartId,eInfoToShow::SATISF,prod,pPart->GetLogSatisfaction());

//            std::cout << "mapStepStock.size()" << mapStepStock.size() << std::endl;
//            std::cout << "mapStepStockFin.size()" << mapStepStockFin.size() << std::endl;
//            std::cout << "mapStepStockConsume.size()" << mapStepStockConsume.size() << std::endl;
//            std::cout << "mapStepStockProd.size()" << mapStepStockProd.size() << std::endl;
//            std::cout << "mapStepStockSati.size()" << mapStepStockSati.size() << std::endl;

        }
    }

    return mapPart_Element_Prod_Values;
}

void CMainDialog::run()
{
    long nYLast=20;

    this->m_pWindow = new Fl_Window(1100,580);
    this->m_pWindow->begin();

    std::shared_ptr<CMarket> upMarket(new CMarket);

    m_pMarket = std::move(upMarket);

    Fl_Box* pBoxRule1 = new Fl_Box(0,0,500,10);
    pBoxRule1->box(FL_BORDER_BOX);
    Fl_Box* pBoxRule2 = new Fl_Box(500,0,500,10);
    pBoxRule2->box(FL_BORDER_BOX);
    Fl_Box* pBoxRule3 = new Fl_Box(0,0,10,360);
    pBoxRule3->box(FL_BORDER_BOX);

    //
    this->m_pMarketPricesChartGroup = new Fl_Group(350,30,300,170);
    this->m_pWindow->add(m_pMarketPricesChartGroup);
    this->m_pMarketPricesChartGroup->end();

    Fl_Button* butRunScenario_1 = new Fl_Button(100,nYLast,100,20,"run scenario 1");
    nYLast=butRunScenario_1->y()+butRunScenario_1->h();


    butRunScenario_1->callback((Fl_Callback*)CMainDialog::LaunchScenario_1, &m_pMarket);
    this->m_pWindow->add(butRunScenario_1);

    Fl_Button* butRunEditedScenario= new Fl_Button(80,nYLast,130,20,"run edited scenario");
    nYLast=butRunEditedScenario->y()+butRunEditedScenario->h();

    butRunEditedScenario->callback((Fl_Callback*)CMainDialog::LaunchEditedScenario, (void*)this);
    this->m_pWindow->add(butRunEditedScenario);


    Fl_Button* butViewResults = new Fl_Button(100,nYLast,100,20,"view results");
    butViewResults->callback((Fl_Callback*)CMainDialog::ViewResults, (void*)this);
    this->m_pWindow->add(butViewResults);

    nYLast=butViewResults->y()+butViewResults->h();

    Fl_Button* butStep = new Fl_Button(100,nYLast,100,20,"Step");
    butStep->callback((Fl_Callback*)CMainDialog::Step, &m_pMarket);
    this->m_pWindow->add(butStep);

    nYLast=butStep->y()+butStep->h();

    Fl_Button* butReset = new Fl_Button(100,nYLast,100,20,"Reset");
    butReset->callback((Fl_Callback*)CMainDialog::Reset, &m_pMarket);
    this->m_pWindow->add(butReset);

    nYLast=butReset->y()+butReset->h();

    Fl_Button* butDestroy = new Fl_Button(100,nYLast,100,20,"Destroy");
    butDestroy->callback((Fl_Callback*)CMainDialog::Destroy, &m_pMarket);//todo
    this->m_pWindow->add(butDestroy);

    nYLast=butDestroy->y()+butDestroy->h();

    m_pCounterZoomValue = new Fl_Counter(350,230,100,20);
    m_pCounterZoomValue->value(1.0);
    m_pCounterZoomValue->label("Zoom");
    m_pCounterFirstStep = new Fl_Counter(450,230,100,20);
    m_pCounterFirstStep->value(0.0);
    m_pCounterFirstStep->label("First step");
    std::vector<SLedgerInfo> vsLedgerInfo = m_pMarket->GetLedgerRef()->GetInfo();
    m_pCounterLastStep = new Fl_Counter(550,230,100,20);
    m_pCounterLastStep->value(10.0);
    m_pCounterLastStep->label("Last step");
//

   //Participant Chart
    this->m_pParticipantsChartGroup = new Fl_Group(350,300,300,170);
    m_pWindow->add(m_pParticipantsChartGroup);
    //

    //Participants check_buttons
    int nCountInfo=0;

    for(auto & pairInfo_Name:this->mapconstInfo_Name)
    {
        Fl_Check_Button* pCheckButton = new Fl_Check_Button(100, nYLast, 100, 20);

        nYLast=pCheckButton->y()+pCheckButton->h();

        pCheckButton->value(1.0);

        pCheckButton->callback((Fl_Callback*)CMainDialog::ViewResults, (void*)this);

        pCheckButton->label(pairInfo_Name.second.c_str());
        int nInfo=static_cast<int>(pairInfo_Name.first);
        m_mapInfo_cbShow[nInfo]=pCheckButton;

        m_pWindow->add(pCheckButton);
        nCountInfo++;
    }

    int nCountProd=0;

    for (auto & prod:def::vProducts)
    {
        Fl_Check_Button* pCheckButton = new Fl_Check_Button(100, nYLast , 100, 20);

        pCheckButton->value(1.0);

        pCheckButton->callback((Fl_Callback*)CMainDialog::ViewResults, (void*)this);

        pCheckButton->label(def::mapeProductNames.at(prod).c_str());
        //int nInfo=static_cast<int>(pairInfo_Name.first);
        m_mapProd_cbShow[prod]=pCheckButton;

        m_pWindow->add(pCheckButton);

        nYLast=pCheckButton->y()+pCheckButton->h();

        nCountProd++;
    }
    //

    m_pCounterNumPartValue = new Fl_Counter(100,nYLast,100,20);
    m_pCounterNumPartValue->value(2.0);
    m_pCounterNumPartValue->label("Num Part");
    nYLast = m_pCounterNumPartValue->y()+m_pCounterNumPartValue->h();

    m_pCounterNumPartValue->callback((Fl_Callback*)CMainDialog::NumPartUpdated, (void*)this);
    m_pWindow->add(m_pCounterNumPartValue);
    m_pCounterNumPartValue->do_callback();

//    m_pPartValuesGroup = new Fl_Group(100,360,100,500);
//
//    int nPart=m_pCounterNumPartValue->value();
//    for(int iPart=0;iPart<nPart;iPart++)
//    {
//        for(auto& prod:def::vProducts)
//        {
//            Fl_Counter* pCount = new Fl_Counter(100,nYLast,100,20);
//            nYLast = pCount->y()+pCount->h()+20;
//            pCount->value(2.0);
//            pCount->label(def::mapeProductNames.at(prod).c_str());
//
//            m_pPartValuesGroup->add(pCount);
//            m_mapPart_Prod_cbShow[std::make_pair(iPart,static_cast<def::eProduct>(prod))] = pCount;
//        }
//    }

    m_pWindow->end();
    m_pWindow->show();

    std::cout << "Before run" << std::endl;
    Fl::run();
    std::cout << "After run" << std::endl;

}


