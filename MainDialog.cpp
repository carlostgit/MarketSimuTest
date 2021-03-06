#include "MainDialog.h"

#include <iostream>
#include <set>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Scroll.H>
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

CMainDialog::StructCalculator::TestMarketWithParticipant::TestMarketWithParticipant(CMainDialog::StructCalculator* pStructCalc):
    m_pParticipant(nullptr),
    m_pMarket(nullptr)
{
    m_pMarket = new CMarket();

    m_pParticipant = new CParticipant();

    m_pMarket->AddParticipant(m_pParticipant);

    std::map<def::eProduct,double> mapPart_1_eProd_dAmount;
    mapPart_1_eProd_dAmount[def::PROD_CLOTH] = pStructCalc->m_mapProd_CounterStock.at(def::PROD_CLOTH)->value();
    mapPart_1_eProd_dAmount[def::PROD_FOOD] = pStructCalc->m_mapProd_CounterStock.at(def::PROD_FOOD)->value();
    mapPart_1_eProd_dAmount[def::PROD_GOLD] = pStructCalc->m_mapProd_CounterStock.at(def::PROD_GOLD)->value();
    CStock stockPart1(mapPart_1_eProd_dAmount);
    m_pParticipant->SetStock(stockPart1);

    std::map<def::eProduct,double> mapeProd_dAmount;
    mapeProd_dAmount[def::PROD_CLOTH] = pStructCalc->m_mapProd_CounterPrices.at(def::PROD_CLOTH)->value();
    mapeProd_dAmount[def::PROD_FOOD] = pStructCalc->m_mapProd_CounterPrices.at(def::PROD_FOOD)->value();
    mapeProd_dAmount[def::PROD_GOLD] = pStructCalc->m_mapProd_CounterPrices.at(def::PROD_GOLD)->value();

    CStock stock(mapeProd_dAmount);

    CPricesInfo pricesInfo;
    pricesInfo.SetStockForPrices(stock);
    m_pMarket->SetPricesInfo(pricesInfo);

    std::map<def::eProduct, double> mapeProd_dSatisf;
    mapeProd_dSatisf[def::PROD_CLOTH] = pStructCalc->m_mapProd_CounterSatisfaction.at(def::PROD_CLOTH)->value();
    mapeProd_dSatisf[def::PROD_FOOD] = pStructCalc->m_mapProd_CounterSatisfaction.at(def::PROD_FOOD)->value();
    mapeProd_dSatisf[def::PROD_GOLD] = pStructCalc->m_mapProd_CounterSatisfaction.at(def::PROD_GOLD)->value();
    m_pParticipant->SetProductSatisfactionMap(mapeProd_dSatisf);

    std::map<def::eProduct, double> mapeProd_dConsumpt;
    mapeProd_dConsumpt[def::PROD_CLOTH] = pStructCalc->m_mapProd_CounterConsumption.at(def::PROD_CLOTH)->value();
    mapeProd_dConsumpt[def::PROD_FOOD] = pStructCalc->m_mapProd_CounterConsumption.at(def::PROD_FOOD)->value();
    mapeProd_dConsumpt[def::PROD_GOLD] = pStructCalc->m_mapProd_CounterConsumption.at(def::PROD_GOLD)->value();
    m_pParticipant->SetProductConsumptionMap(mapeProd_dConsumpt);

    std::map<def::eProduct, double> mapeProd_dProduction;
    mapeProd_dProduction[def::PROD_CLOTH] = pStructCalc->m_mapProd_CounterProduction.at(def::PROD_CLOTH)->value();
    mapeProd_dProduction[def::PROD_FOOD] = pStructCalc->m_mapProd_CounterProduction.at(def::PROD_FOOD)->value();
    mapeProd_dProduction[def::PROD_GOLD] = pStructCalc->m_mapProd_CounterProduction.at(def::PROD_GOLD)->value();
    m_pParticipant->SetProductPrCapacityMap(mapeProd_dProduction);
    //Fin de carga de datos
}
CMainDialog::StructCalculator::TestMarketWithParticipant::~TestMarketWithParticipant()
{

    delete m_pParticipant;
    delete m_pMarket;
}

void CMainDialog::StructCalculator::CalcSatisf(Fl_Widget* w, void* pvoidStructCalculator)
{
    std::cout << "static CalcSatisf" << std::endl;
    StructCalculator* pStructCalculator = static_cast<StructCalculator*>(pvoidStructCalculator);
    pStructCalculator->CalcSatisf();
}
void CMainDialog::StructCalculator::CalcSatisf()
{
    std::cout << "non static CalcSatisf" << std::endl;

    //Carga de datos
    TestMarketWithParticipant testMarket(this);
    m_pBoxOfResults->label("TODO: Tengo que cambiar lo de GetSatisfactionOfProduct");

    CStock stock;
    for (auto & pairProd_Counter:m_mapProd_CounterStock)
    {
        def::eProduct eProd = pairProd_Counter.first;
        double dValue = pairProd_Counter.second->value();

        stock.AddAmount(eProd,dValue);
    }

    std::map<def::eProduct, double> mapeProd_dSatisf;
    for (auto & pairProd_Counter:m_mapProd_CounterSatisfaction)
    {
        def::eProduct eProd = pairProd_Counter.first;
        double dValue = pairProd_Counter.second->value();
        mapeProd_dSatisf[eProd]=dValue;

    }
    double dSatisfOfStock = CParticipant::GetSatisfactionOfStock(&stock,mapeProd_dSatisf);
    double dSatisfOfStockCateg = CParticipant::GetSatisfactionOfStockWithCategories(&stock,mapeProd_dSatisf);

    double dSatisfOfStockCateg2 = CParticipant::GetSatisfactionOfStockWithCategories2(&stock,mapeProd_dSatisf);

//    stock.PrintInfo();
//    for (auto& pairP_Sat:mapeProd_dSatisf)
//    {
//        std::cout<<def::mapeProductNames.at(pairP_Sat.first)<<std::endl;
//        std::cout<<std::to_string(pairP_Sat.second)<<std::endl;
//    }

    static std::string sSatisfOfProd = "0";
    //sSatisfOfProd = std::to_string(dSatisfOfStock);
    sSatisfOfProd = std::to_string(dSatisfOfStockCateg2);
    m_pBoxOfResults->label(sSatisfOfProd.c_str());
}

void CMainDialog::StructCalculator::CalcConsumpt(Fl_Widget* w, void* pvoidStructCalculator)
{
    std::cout << "static CalcConsumpt" << std::endl;
    StructCalculator* pStructCalculator = static_cast<StructCalculator*>(pvoidStructCalculator);
    pStructCalculator->CalcConsumpt();
}
void CMainDialog::StructCalculator::CalcConsumpt()
{
    std::cout << "non static CalcConsumpt" << std::endl;

    TestMarketWithParticipant testMarket(this);
    CStock stock = testMarket.GetParticipantRef()->CalculateConsumption();
    static std::string resultsLabel=""; //chapu. deber�a hacer otra cosa aqu�. Quiz� usar una variable miembro


    resultsLabel="";
    for (auto & prod:def::vProducts)
    {
        resultsLabel=resultsLabel+" "+std::to_string(stock.GetAmount(prod));

    }

    m_pBoxOfResults->label(resultsLabel.c_str());
}

void CMainDialog::StructCalculator::CalcProduction(Fl_Widget* w, void* pvoidStructCalculator)
{
    std::cout << "static CalcProduction" << std::endl;
    StructCalculator* pStructCalculator = static_cast<StructCalculator*>(pvoidStructCalculator);
    pStructCalculator->CalcProduction();
}


void CMainDialog::StructCalculator::CalcProduction()
{
    std::cout << "non static CalcProduction" << std::endl;

    TestMarketWithParticipant testMarket(this);
    CPricesInfo pricesInfo = testMarket.GetMarketRef()->GetPricesInfo();
    CStock stock = testMarket.GetParticipantRef()->CalculateProduction(&pricesInfo);

    static std::string resultsLabel=""; //chapu. deber�a hacer otra cosa aqu�. Quiz� usar una variable miembro


    resultsLabel="";
    for (auto & prod:def::vProducts)
    {
        resultsLabel=resultsLabel+" "+std::to_string(stock.GetAmount(prod));

    }

    m_pBoxOfResults->label(resultsLabel.c_str());

}


CMainDialog::StructCalculator::StructCalculator():
    Fl_Window(20,20,500,500)
{

    long nProdCounter=0;

    long nLastY = this->y();

    Fl_Box* pboxNumOfPart = new Fl_Box(this->x(),nLastY,100,30);
    static std::string sNumOfPart = "CParticipant Tests";
    pboxNumOfPart ->label(sNumOfPart.c_str());
    nLastY =pboxNumOfPart->y()+pboxNumOfPart->h();

    for (auto & prod:def::vProducts)
    {
        m_mapProd_CounterPrices[prod] = new Fl_Counter(nProdCounter*100,nLastY + 10,100,30);
        m_mapProd_CounterPrices.at(prod)->value(1.0);
        m_mapProd_CounterPrices.at(prod)->label(def::mapeProductNames.at(prod).c_str());
        m_pBoxPrices = new Fl_Box(300,nLastY + 10,200,30);
        m_pBoxPrices->label("box 1: Market Prices");

        m_mapProd_CounterStock[prod] = new Fl_Counter(nProdCounter*100,nLastY + 40,100,30);
        m_mapProd_CounterStock.at(prod)->label(def::mapeProductNames.at(prod).c_str());
        m_mapProd_CounterStock.at(prod)->value(1.0);
        m_pBoxStock = new Fl_Box(300,nLastY + 40,200,30);
        m_pBoxStock->label("box 2: Stock");

        m_mapProd_CounterProduction[prod] = new Fl_Counter(nProdCounter*100,nLastY + 70,100,30);
        m_mapProd_CounterProduction.at(prod)->label(def::mapeProductNames.at(prod).c_str());
        m_mapProd_CounterProduction.at(prod)->value(1.0);
        m_pBoxProduction = new Fl_Box(300,nLastY + 70,200,30);
        m_pBoxProduction->label("box 3: Production Matrix");

        m_mapProd_CounterConsumption[prod] = new Fl_Counter(nProdCounter*100,nLastY + 100,100,30);
        m_mapProd_CounterConsumption.at(prod)->label(def::mapeProductNames.at(prod).c_str());
        m_mapProd_CounterConsumption.at(prod)->value(1.0);
        m_pBoxConsumption = new Fl_Box(300,nLastY + 100,200,30);
        m_pBoxConsumption->label("box 4: Consumption Matrix");

        m_mapProd_CounterSatisfaction[prod] = new Fl_Counter(nProdCounter*100,nLastY + 130,100,30);
        m_mapProd_CounterSatisfaction.at(prod)->label(def::mapeProductNames.at(prod).c_str());
        m_mapProd_CounterSatisfaction.at(prod)->value(1.0);
        m_pBoxSatisfaction = new Fl_Box(300,nLastY + 130,200,30);
        m_pBoxSatisfaction->label("box 5: Satisfaction Matrix");

        m_mapProd_Counter6[prod] = new Fl_Counter(nProdCounter*100,nLastY + 160,100,30);
        m_mapProd_Counter6.at(prod)->label(def::mapeProductNames.at(prod).c_str());
        m_pBox6 = new Fl_Box(300,nLastY + 160,200,30);
        m_pBox6->label("box 6");

        nProdCounter++;
    }

    nLastY=m_mapProd_Counter6.at(def::vProducts.front())->y()+m_mapProd_Counter6.at(def::vProducts.front())->h();


    m_pBoxOfResults = new Fl_Box(10,nLastY,400,100);
    m_pBoxOfResults->label("Resultados bla blallldlk alkd ald la ldkalfa�jf 5245");
    nLastY += m_pBoxOfResults->h();
    m_butCalcSatisf = new Fl_Button(50,nLastY,200,20,"Calc. Satisfaction");
    m_butCalcSatisf->callback((Fl_Callback*)CMainDialog::StructCalculator::CalcSatisf, (void*)this);

    nLastY += m_butCalcSatisf->h();
    m_butCalcConsumpt = new Fl_Button(50,nLastY,200,20,"Calc. Consumption");
    m_butCalcConsumpt->callback((Fl_Callback*)CMainDialog::StructCalculator::CalcConsumpt, (void*)this);

    nLastY += m_butCalcConsumpt->h();
    m_butCalcProduction = new Fl_Button(50,nLastY,200,20,"Calc. Production");
    m_butCalcProduction->callback((Fl_Callback*)CMainDialog::StructCalculator::CalcProduction, (void*)this);

}

std::map<def::eProduct, double> CMainDialog::StructInitPrices::GetInitPricesMap()
{
    std::map<def::eProduct, double> rInitPricesMap;
    //std::map<def::eProduct,Fl_Check_Button*> m_mapProd_cInitPrice;
    for(auto & pairProd_FlCheckBut:m_mapProd_cInitPrice)
    {
        rInitPricesMap[pairProd_FlCheckBut.first] = pairProd_FlCheckBut.second->value();
    }

    return rInitPricesMap;
}

void CMainDialog::StructPartiValuesGroup::InitPartiValuesControls(StructPartiValuesGroup* pPartValuesGroupNew,
                                        long nNumOfParticipants,
                                        StructPartiValuesGroup* pPartValuesGroupOld
                                        )
{
    long nYLast=120;

    Fl_Box* pBox1 = new Fl_Box(700,nYLast,100,20);
    pBox1->label("Stock Ini");
    Fl_Box* pBox2 = new Fl_Box(800,nYLast,100,20);
    pBox2->label("Product Ini");
    Fl_Box* pBox3 = new Fl_Box(900,nYLast,100,20);
    pBox3->label("Satisfaction Ini");
    Fl_Box* pBox4 = new Fl_Box(1000,nYLast,100,20);
    pBox4->label("Consumption Ini");

    nYLast = pBox1->y()+pBox1->h();

    int nPart=nNumOfParticipants;

    for(int iPart=0;iPart<nPart;iPart++)
    {

        for(auto& prod:def::vProducts)
        {
            //Init Amounts
            Fl_Counter* pCountInit = new Fl_Counter(700,nYLast,100,20);

            pCountInit->value(2.0);

            if(pPartValuesGroupOld && pPartValuesGroupOld->m_mapPart_Prod_InitStockCounter.end()!=pPartValuesGroupOld->m_mapPart_Prod_InitStockCounter.find(std::make_pair(iPart,static_cast<def::eProduct>(prod))))
            {
                Fl_Counter* pCountOld = pPartValuesGroupOld->m_mapPart_Prod_InitStockCounter.at(std::make_pair(iPart,static_cast<def::eProduct>(prod)));
                pCountInit->value(pCountOld->value());
            }

            std::string sName=std::to_string(iPart)+ " " +def::mapeProductNames.at(prod);
            pPartValuesGroupNew->m_map_pairPartProd_Name[std::make_pair(iPart,prod)]=sName;
            pCountInit->label(pPartValuesGroupNew->m_map_pairPartProd_Name.at(std::make_pair(iPart,prod)).c_str());

            pPartValuesGroupNew->add(pCountInit);

            pPartValuesGroupNew->m_mapPart_Prod_InitStockCounter[std::make_pair(iPart,static_cast<def::eProduct>(prod))] = pCountInit;



            //Production Amounts
            Fl_Counter* pCountProd = new Fl_Counter(800,nYLast,100,20);
            //nYLast = pCountProd->y()+pCountProd->h()+20;
            pCountProd->value(2.0);

            if(pPartValuesGroupOld && pPartValuesGroupOld->m_mapPart_Prod_ProductionCounter.end()!=pPartValuesGroupOld->m_mapPart_Prod_ProductionCounter.find(std::make_pair(iPart,static_cast<def::eProduct>(prod))))
            {
                Fl_Counter* pCountOld = pPartValuesGroupOld->m_mapPart_Prod_ProductionCounter.at(std::make_pair(iPart,static_cast<def::eProduct>(prod)));
                pCountProd->value(pCountOld->value());
            }

            //std::string sName=std::to_string(iPart)+ " " +def::mapeProductNames.at(prod);
            //m_map_pairPartProd_Name[std::make_pair(iPart,prod)]=sName;
            pCountProd->label(pPartValuesGroupNew->m_map_pairPartProd_Name.at(std::make_pair(iPart,prod)).c_str());

            pPartValuesGroupNew->add(pCountProd);

            pPartValuesGroupNew->m_mapPart_Prod_ProductionCounter[std::make_pair(iPart,static_cast<def::eProduct>(prod))] = pCountProd;
            //


            //Satisfaction Amounts
            Fl_Counter* pCountSatisf = new Fl_Counter(900,nYLast,100,20);
            //nYLast = pCountProd->y()+pCountProd->h()+20;
            pCountSatisf->value(2.0);

            if(pPartValuesGroupOld && pPartValuesGroupOld->m_mapPart_Prod_SatisfactionCounter.end()!=pPartValuesGroupOld->m_mapPart_Prod_SatisfactionCounter.find(std::make_pair(iPart,static_cast<def::eProduct>(prod))))
            {
                Fl_Counter* pCountOld = pPartValuesGroupOld->m_mapPart_Prod_SatisfactionCounter.at(std::make_pair(iPart,static_cast<def::eProduct>(prod)));
                pCountSatisf->value(pCountOld->value());
            }

            //std::string sName=std::to_string(iPart)+ " " +def::mapeProductNames.at(prod);
            //m_map_pairPartProd_Name[std::make_pair(iPart,prod)]=sName;
            pCountSatisf->label(pPartValuesGroupNew->m_map_pairPartProd_Name.at(std::make_pair(iPart,prod)).c_str());

            pPartValuesGroupNew->add(pCountSatisf);

            pPartValuesGroupNew->m_mapPart_Prod_SatisfactionCounter[std::make_pair(iPart,static_cast<def::eProduct>(prod))] = pCountSatisf;
            //


            //Consumption Amounts
            Fl_Counter* pCountConsumpt = new Fl_Counter(1000,nYLast,100,20);
            //nYLast = pCountProd->y()+pCountProd->h()+20;
            pCountConsumpt->value(2.0);

            if(pPartValuesGroupOld && pPartValuesGroupOld->m_mapPart_Prod_ConsumptionCounter.end()!=pPartValuesGroupOld->m_mapPart_Prod_ConsumptionCounter.find(std::make_pair(iPart,static_cast<def::eProduct>(prod))))
            {
                Fl_Counter* pCountOld = pPartValuesGroupOld->m_mapPart_Prod_ConsumptionCounter.at(std::make_pair(iPart,static_cast<def::eProduct>(prod)));
                pCountConsumpt->value(pCountOld->value());
            }

            //std::string sName=std::to_string(iPart)+ " " +def::mapeProductNames.at(prod);
            //m_map_pairPartProd_Name[std::make_pair(iPart,prod)]=sName;
            pCountConsumpt->label(pPartValuesGroupNew->m_map_pairPartProd_Name.at(std::make_pair(iPart,prod)).c_str());

            pPartValuesGroupNew->add(pCountConsumpt);

            pPartValuesGroupNew->m_mapPart_Prod_ConsumptionCounter[std::make_pair(iPart,static_cast<def::eProduct>(prod))] = pCountConsumpt;
            //




            nYLast = pCountInit->y()+pCountInit->h()+20;

        }
    }
}


CStock CMainDialog::StructPartiValuesGroup::GetInitStock(int nPartIndex)
{
    std::map<def::eProduct,double> mapProd_InitStockAmount;
    for (auto & pairPartProd_InitStock:m_mapPart_Prod_InitStockCounter)
    {
        std::pair<int,def::eProduct> pairPartProd = pairPartProd_InitStock.first;
        if(nPartIndex==pairPartProd.first)
        {
            Fl_Counter* pflCounter = pairPartProd_InitStock.second;
            double dAmount = pflCounter->value();
            mapProd_InitStockAmount[pairPartProd.second]=dAmount;
        }
    }

    CStock stockPart(mapProd_InitStockAmount);

    return stockPart;
}

//std::map<def::eProduct, double> GetPrCapacityMap
std::map<def::eProduct, double> CMainDialog::StructPartiValuesGroup::GetPrCapacityMap(int nPartIndex)
{
    std::map<def::eProduct,double> mapProd_ProductionAmount;
    for (auto & pairPartProd_Production:m_mapPart_Prod_ProductionCounter)
    {
        std::pair<int,def::eProduct> pairPartProd = pairPartProd_Production.first;
        if(nPartIndex==pairPartProd.first)
        {
            Fl_Counter* pflCounter = pairPartProd_Production.second;
            double dAmount = pflCounter->value();
            mapProd_ProductionAmount[pairPartProd.second]=dAmount;
        }
    }
    //std::map<std::pair<int,def::eProduct>,Fl_Counter*> m_mapPart_Prod_ProductionCounter;

    return mapProd_ProductionAmount;
}

//std::map<def::eProduct, double> GetSatisfactionMap
std::map<def::eProduct, double> CMainDialog::StructPartiValuesGroup::GetSatisfactionMap(int nPartIndex)
{
    std::map<def::eProduct,double> mapProd_SatisfactionAmount;
    for (auto & pairPartProd_Satisfaction:m_mapPart_Prod_SatisfactionCounter)
    {
        std::pair<int,def::eProduct> pairPartProd = pairPartProd_Satisfaction.first;
        if(nPartIndex==pairPartProd.first)
        {
            Fl_Counter* pflCounter = pairPartProd_Satisfaction.second;
            double dAmount = pflCounter->value();
            mapProd_SatisfactionAmount[pairPartProd.second]=dAmount;
        }
    }
    //std::map<std::pair<int,def::eProduct>,Fl_Counter*> m_mapPart_Prod_ProductionCounter;

    return mapProd_SatisfactionAmount;
}


//std::map<def::eProduct, double> GetConsumptionMap
std::map<def::eProduct, double> CMainDialog::StructPartiValuesGroup::GetConsumptionMap(int nPartIndex)
{
    std::map<def::eProduct,double> mapProd_ConsumptionAmount;
    for (auto & pairPartProd_Consumption:m_mapPart_Prod_ConsumptionCounter)
    {
        std::pair<int,def::eProduct> pairPartProd = pairPartProd_Consumption.first;
        if(nPartIndex==pairPartProd.first)
        {
            Fl_Counter* pflCounter = pairPartProd_Consumption.second;
            double dAmount = pflCounter->value();
            mapProd_ConsumptionAmount[pairPartProd.second]=dAmount;
        }
    }
    //std::map<std::pair<int,def::eProduct>,Fl_Counter*> m_mapPart_Prod_ProductionCounter;

    return mapProd_ConsumptionAmount;
}


//CMainDialog::StructPartiChartGroup::StructPartiChartGroup():Fl_Group(10,10,10,10,nullptr){}
CMainDialog::StructPartiChartGroup::StructPartiChartGroup():Fl_Scroll(10,10,10,10,nullptr){}
//CMainDialog::StructPartiChartGroup::StructPartiChartGroup(int x, int y, int w, int h, const char* c):Fl_Group(x,y,w,h,c){}
CMainDialog::StructPartiChartGroup::StructPartiChartGroup(int x, int y, int w, int h, const char* c):Fl_Scroll(x,y,w,h,c){}

bool CMainDialog::StructPartiChartGroup::ViewParticipant(int nPartId) const
{
    return this->sCheckButtons.IsChecked(nPartId);
}
std::string CMainDialog::StructPartiChartGroup::GetNameOfParticipant(int nPartId)
{
    return this->sCheckButtons.GetName(nPartId);
}

std::vector<double> CMainDialog::StructPartiChartGroup::GetValuesFor(CMarket* pMarket,int nPartId, int nTypeInfo,def::eProduct eProduct)
{
    std::vector<double> vValues;

    auto lfAddMapStepStock = [&eProduct](std::vector<double>& vVal, std::map<long,CStock> mapStepStock)
    {
        for(auto & pairStep_Stock:mapStepStock)
        {
            double dAmount = pairStep_Stock.second.GetAmount(eProduct);
            vVal.push_back(dAmount);
        }
    };

    if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_INI))
    {
        for(auto & pPart:pMarket->GetParticipants())
        {
            if (pPart->GetId()==nPartId)
                lfAddMapStepStock(vValues,pPart->GetStockIniMap());
        }
    }
    else if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_FIN))
    {
        for(auto & pPart:pMarket->GetParticipants())
        {
            if (pPart->GetId()==nPartId)
                lfAddMapStepStock(vValues,pPart->GetStockFinMap());
        }
    }
    else if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_CONSUM))
    {
        for(auto & pPart:pMarket->GetParticipants())
        {
            if (pPart->GetId()==nPartId)
                lfAddMapStepStock(vValues,pPart->GetLogConsume());
        }
    }
    else if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_PROD))
    {
        for(auto & pPart:pMarket->GetParticipants())
        {
            if (pPart->GetId()==nPartId)
                lfAddMapStepStock(vValues,pPart->GetLogProduction());
        }
    }
    else if (nTypeInfo == static_cast<int>(eInfoToShow::SATISF))
    {
        for(auto & pPart:pMarket->GetParticipants())
        {
            if (pPart->GetId()==nPartId)
                lfAddMapStepStock(vValues,pPart->GetLogSatisfaction());
        }
    }

    else if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_SENT))
    {
        std::map<int,std::vector<CStock>> mapPartId_vStocksSent =
                pMarket->GetLedgerRef()->GetSentToMarketForEachStep();

        for(auto & pairPartId_vStockSent:mapPartId_vStocksSent)
        {
            for(auto& stock:pairPartId_vStockSent.second)
            {
                double dAmount = stock.GetAmount(eProduct);
                vValues.push_back(dAmount);
            }
        }
    }
    else if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_RECEIVED))
    {
        std::map<int,std::vector<CStock>> mapPartId_vStocksReceived =
                pMarket->GetLedgerRef()->GetReceivedFromMarketEachStep();

        for(auto & pairPartId_vStockRec:mapPartId_vStocksReceived)
        {
            int nPartId = pairPartId_vStockRec.first;
            for(auto& stock:pairPartId_vStockRec.second)
            {
                double dAmount = stock.GetAmount(eProduct);
                vValues.push_back(dAmount);
            }
        }
    }


    return vValues;
}

void CMainDialog::StructPartiChartGroup::InitCharts(
                           const std::map<int,Fl_Check_Button*> mapInfo_cbShow,
                            const std::map<int,Fl_Check_Button*> mapProd_cbShow,
                            const StructPartiChartGroup* psPartiChartGroupOld,
                            const std::map<def::eProduct,std::string>& mapProdNames,
                            CMarket* pMarket,
                            const CMainDialog* pMainDialog
                           )
{

    StructPartiChartGroup* psPartiChartGroupNew = this;

    long nTotalGroupSize= psPartiChartGroupNew->h();



    //long nChartHeight = 200;
    long nParticipantCheckSize= 20;
    long nNumberOfParticipants = pMarket->GetParticipants().size();
    long nSpaceBetChartAndButtons = 40;
    //long nTotalGroupSize=nChartHeight+nSpaceBetChartAndButtons+nParticipantCheckSize*nNumberOfParticipants;

    long nChartHeight = nTotalGroupSize - nSpaceBetChartAndButtons - (nParticipantCheckSize*nNumberOfParticipants);

    //psPartiChartGroupNew->h(nTotalGroupSize);

    long nCount=0;

    for(auto & pairProd_cbShow:mapProd_cbShow)
    {
        if(pairProd_cbShow.second->value()!=0)
        {
            for(auto & pairTypeinfo_cbShow:mapInfo_cbShow)
            {
                if(pairTypeinfo_cbShow.second->value()!=0)
                {
                    for(auto part:pMarket->GetParticipants())
                    {
                        int nPartId = part->GetId();
                        if(nullptr==psPartiChartGroupOld|| psPartiChartGroupOld->ViewParticipant(nPartId))
                        {
                            def::eProduct eProduct=static_cast<def::eProduct>(pairProd_cbShow.first);
                            int nTypeInfo=pairTypeinfo_cbShow.first;

                            Fl_Chart* pChart = new Fl_Chart(psPartiChartGroupNew->x(),psPartiChartGroupNew->y(),
                                        psPartiChartGroupNew->w(),nChartHeight);
                            pChart->type(FL_LINE_CHART);
                            pChart->box(FL_NO_BOX);

                            vCharts.push_back(pChart);

                            std::vector<double> vValues = GetValuesFor(pMarket, nPartId, nTypeInfo, eProduct);
                            for (auto & value:vValues)
                            {
                                unsigned long color=FL_RED+10*nCount;
                                nCount++;

                                //std::cout<<"vStocks.size(): "<< vStocks.size()<<std::endl;
                                double dValue=value;
                                if(dValue>dMaxBound)
                                    dMaxBound=dValue;

                                pChart->add(dValue,def::mapeProductNames.at(eProduct).c_str(),color);
                            }
                        }
                    }
                }
            }
        }
    }


    for (auto& pChart:vCharts)
    {
        pChart->bounds(dMinBound,dMaxBound);
    }



    Fl_Box *pBoxMin = new Fl_Box(psPartiChartGroupNew->x()-20,
                                  psPartiChartGroupNew->y()+nChartHeight,
                                  100, 20 );
    Fl_Box *pBoxMax = new Fl_Box(psPartiChartGroupNew->x()-20,
                                  psPartiChartGroupNew->y()-20,
                                  100, 20 );


    psPartiChartGroupNew->sChartMin = std::to_string(psPartiChartGroupNew->dMinBound);
    psPartiChartGroupNew->sChartMax = std::to_string(psPartiChartGroupNew->dMaxBound);

    pBoxMin->label(psPartiChartGroupNew->sChartMin.c_str());
    pBoxMax->label(psPartiChartGroupNew->sChartMax.c_str());

    std::map<int,Fl_Check_Button*> mapPartId_CheckBNew;
    int nCountPart=0;
    for(auto& part:pMarket->GetParticipants())
    {

        int nPartId = part->GetId();

        std::cout << "nPartId " << nPartId << std::endl;
        std::cout << "nCountPart " << nCountPart << std::endl;


        //static std::string nsId = std::to_string(nPartId);

        Fl_Check_Button* pCheckButton = new Fl_Check_Button(psPartiChartGroupNew->x(),
                                                       psPartiChartGroupNew->y() + nChartHeight + nSpaceBetChartAndButtons+ nCountPart*nParticipantCheckSize,
                                                       100,20);

        pCheckButton->callback((Fl_Callback*)CMainDialog::ViewResults, (void*)pMainDialog);

        psPartiChartGroupNew->sCheckButtons.mapPartId_sName[nPartId]=std::to_string(nPartId);

        pCheckButton->label(psPartiChartGroupNew->sCheckButtons.mapPartId_sName.at(nPartId).c_str());

        mapPartId_CheckBNew[nPartId] = pCheckButton;

        if(nullptr == psPartiChartGroupOld || psPartiChartGroupOld->sCheckButtons.mapPartId_CheckB.end()==psPartiChartGroupOld->sCheckButtons.mapPartId_CheckB.find(nPartId))
        {
            pCheckButton->value(1);
        }
        else
        {
            pCheckButton->value(psPartiChartGroupOld->sCheckButtons.mapPartId_CheckB.at(nPartId)->value());
        }

        nCountPart++;
    }


    psPartiChartGroupNew->sCheckButtons.mapPartId_CheckB = mapPartId_CheckBNew;

    for (auto & pairId_Check:psPartiChartGroupNew->sCheckButtons.mapPartId_CheckB)
    {
        Fl_Check_Button* pCheckButtonNew=pairId_Check.second;
        psPartiChartGroupNew->add(pCheckButtonNew);
    }
}

bool CMainDialog::StructPartiChartGroup::StructCheckButtons::IsChecked(int nPartId) const
{
    std::cout << "mapPartId_CheckB.size()" << mapPartId_CheckB.size() << std::endl;
    if(this->mapPartId_CheckB.end()!=this->mapPartId_CheckB.find(nPartId))
    {
        Fl_Check_Button* pCheckButton = this->mapPartId_CheckB.at(nPartId);
        if(pCheckButton->value()==0)
            return false;
        else
            return true;
    }
    return true;
}
std::string CMainDialog::StructPartiChartGroup::StructCheckButtons::GetName(int nPartId)
{
    std::string str;
    if(this->mapPartId_sName.end()!=this->mapPartId_sName.find(nPartId))
    {
        str = this->mapPartId_sName.at(nPartId);
    }
    return str;
}



CMainDialog::CMainDialog():
    m_pMarketPricesChartGroup(nullptr),m_pWindow(nullptr),
    //m_dMinChartBound(0.0),
    //m_dMaxChartBound(0.0),
    m_sChartMin(""),
    m_sChartMax(""),
    m_pCounterZoomValue(nullptr),
    m_pCounterFirstStep(nullptr),
    m_pCounterLastStep(nullptr),
    //m_pPartValuesGroup(nullptr),
    m_psPartiChartGroup(nullptr),
    m_psPartiValuesGroup(nullptr),
    m_psCalculator(nullptr)
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

    StructPartiValuesGroup* pPartValuesGroupNew = new StructPartiValuesGroup(700,100,500,1100);

    //TODO:Cambiar todo esto por un m�todo dentro de StructPartiValuesGroup:

    long nNumOfParticipants=m_pCounterNumPartValue->value();
    StructPartiValuesGroup* pPartValuesGroupOld = m_psPartiValuesGroup;

    pPartValuesGroupNew->InitPartiValuesControls(pPartValuesGroupNew,
                                        nNumOfParticipants,
                                        pPartValuesGroupOld
                                        );

    /*
    long nYLast=20;

    Fl_Box* pBox1 = new Fl_Box(700,nYLast,100,20);
    pBox1->label("Stock Ini");
    Fl_Box* pBox2 = new Fl_Box(800,nYLast,100,20);
    pBox2->label("Production Ini");

    nYLast = pBox1->y()+pBox1->h();

    int nPart=m_pCounterNumPartValue->value();
    for(int iPart=0;iPart<nPart;iPart++)
    {

        for(auto& prod:def::vProducts)
        {


            //Init Amounts
            Fl_Counter* pCountInit = new Fl_Counter(700,nYLast,100,20);

            pCountInit->value(2.0);

            if(m_psPartiValuesGroup && m_psPartiValuesGroup->m_mapPart_Prod_InitStockCounter.end()!=m_psPartiValuesGroup->m_mapPart_Prod_InitStockCounter.find(std::make_pair(iPart,static_cast<def::eProduct>(prod))))
            {
                Fl_Counter* pCountOld = m_psPartiValuesGroup->m_mapPart_Prod_InitStockCounter.at(std::make_pair(iPart,static_cast<def::eProduct>(prod)));
                pCountInit->value(pCountOld->value());
            }

            std::string sName=std::to_string(iPart)+ " " +def::mapeProductNames.at(prod);
            pPartValuesGroupNew->m_map_pairPartProd_Name[std::make_pair(iPart,prod)]=sName;
            pCountInit->label(pPartValuesGroupNew->m_map_pairPartProd_Name.at(std::make_pair(iPart,prod)).c_str());

            pPartValuesGroupNew->add(pCountInit);

            pPartValuesGroupNew->m_mapPart_Prod_InitStockCounter[std::make_pair(iPart,static_cast<def::eProduct>(prod))] = pCountInit;
            //

            //Production Amounts
            Fl_Counter* pCountProd = new Fl_Counter(800,nYLast,100,20);
            //nYLast = pCountProd->y()+pCountProd->h()+20;
            pCountProd->value(2.0);

            if(m_psPartiValuesGroup && m_psPartiValuesGroup->m_mapPart_Prod_ProductionCounter.end()!=m_psPartiValuesGroup->m_mapPart_Prod_ProductionCounter.find(std::make_pair(iPart,static_cast<def::eProduct>(prod))))
            {
                Fl_Counter* pCountOld = m_psPartiValuesGroup->m_mapPart_Prod_ProductionCounter.at(std::make_pair(iPart,static_cast<def::eProduct>(prod)));
                pCountProd->value(pCountOld->value());
            }

            //std::string sName=std::to_string(iPart)+ " " +def::mapeProductNames.at(prod);
            //m_map_pairPartProd_Name[std::make_pair(iPart,prod)]=sName;
            pCountProd->label(pPartValuesGroupNew->m_map_pairPartProd_Name.at(std::make_pair(iPart,prod)).c_str());

            pPartValuesGroupNew->add(pCountProd);

            pPartValuesGroupNew->m_mapPart_Prod_ProductionCounter[std::make_pair(iPart,static_cast<def::eProduct>(prod))] = pCountProd;
            //


            nYLast = pCountInit->y()+pCountInit->h()+20;


        }

    }
    */
    //m_mapPart_Prod_InitStockCounter = newmapPart_Prod_InitStockCounter;
    //this->m_mapPart_Prod_ProductionCounter = newmapPart_Prod_ProductionCounter;

    //std::cout << "2 m_mapPart_Prod_InitStockCounter.size()" << m_mapPart_Prod_InitStockCounter.size() << std::endl;

    m_pScroll->add(pPartValuesGroupNew);
    //std::cout << "NumPartUpdated antes del clear" << std::endl;

    if (m_psPartiValuesGroup)
    {
        m_psPartiValuesGroup->hide();
        m_psPartiValuesGroup->show();
        m_psPartiValuesGroup->clear();
        m_psPartiValuesGroup = nullptr;
    }

    m_psPartiValuesGroup = pPartValuesGroupNew;

    std::cout << "NumPartUpdated despues del clear" << std::endl;


}



void CMainDialog::Destroy(Fl_Widget* w, void* pvoidMarket)
{
    std::cout << "Destroy" << std::endl;
    std::shared_ptr<CMarket>* pupMarket = static_cast<std::shared_ptr<CMarket>*>(pvoidMarket);
    (*pupMarket).reset();
}

void CMainDialog::Calculator(Fl_Widget* w, void* pvoidMainDialog)
{
    std::cout << "Calculator" << std::endl;
    CMainDialog* pMainDialog = static_cast<CMainDialog*>(pvoidMainDialog);
    pMainDialog->Calculator(w);
}

void CMainDialog::Calculator(Fl_Widget* w)
{
    std::cout << "Calculator non static" << std::endl;

    m_pScroll->end();
    m_pWindow->end(); //Si no pongo esto, la nueva ventana queda integrada en la antigua

    if(m_pMarket.get())
    {
        m_psCalculator = new StructCalculator();

        m_psCalculator->show();
    }

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

    //double debugSize = m_mapPart_Prod_InitStockCounter.size();
    //std::cout << "4 m_mapPart_Prod_InitStockCounter.size()" << m_mapPart_Prod_InitStockCounter.size() << std::endl;

    if(nullptr==m_psPartiValuesGroup)
        return;

    std::set<int> setPartIndex;
    for (auto & pairPartProd_cbShow:m_psPartiValuesGroup->m_mapPart_Prod_InitStockCounter)
    {
        setPartIndex.insert(pairPartProd_cbShow.first.first);
    }

    for (auto & iPart:setPartIndex)
    {


        CParticipant* pParticipant = new CParticipant();

        //CStock stockPart(mapProd_InitStockAmount);
        CStock stockInitPart = m_psPartiValuesGroup->GetInitStock(iPart);
        pParticipant->SetStock(stockInitPart);

        pParticipant->SetProductPrCapacityMap(m_psPartiValuesGroup->GetPrCapacityMap(iPart));
        pParticipant->SetProductSatisfactionMap(m_psPartiValuesGroup->GetSatisfactionMap(iPart));
        pParticipant->SetProductConsumptionMap(m_psPartiValuesGroup->GetConsumptionMap(iPart));

        m_pMarket->AddParticipant(pParticipant);
    }


//    std::map<def::eProduct,double> mapeProd_dAmount;
//    mapeProd_dAmount[def::PROD_CLOTH]=1.0;
//    mapeProd_dAmount[def::PROD_FOOD]=1.0;
//    mapeProd_dAmount[def::PROD_GOLD]=1.0;

    CStock stock(m_sInitPrices.GetInitPricesMap());

    //TODO: Acordarme de probar a compilar y lanzar en el PC de sobremesa el proyecto con las librer�as de 32 bits
    //TODO: Acordarme de crear 2 proyectos. 1 con librer�as externas para 32 bits, y otro para librer�as para 64
    //La soluci�n puede ser a�adir m�s build targets, (unos con Release y Debug para 32 bits y otros para 64S bits), con librer�as en distintos directorios

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


    //Este m�todo no har� el delete
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

    StructPartiChartGroup* psPartiChartGroupNew = new StructPartiChartGroup(350,300,300,270);

    //lfPaintCharts(m_mapInfo_cbShow,m_mapProd_cbShow,m_psPartiChartGroup,def::mapeProductNames);
    psPartiChartGroupNew->InitCharts(
                            m_mapInfo_cbShow,
                            m_mapProd_cbShow,
                            m_psPartiChartGroup, //Old
                            def::mapeProductNames,
                            m_pMarket.get(),
                            this
                           );


    psPartiChartGroupNew->end();



    if(this->m_psPartiChartGroup)
    {
        m_psPartiChartGroup->clear();
        delete(this->m_psPartiChartGroup);
        this->m_psPartiChartGroup=nullptr;
    }

    m_psPartiChartGroup = psPartiChartGroupNew;
}

std::vector<double> CMainDialog::GetValuesFor(int nPartId, int nTypeInfo,def::eProduct eProduct)
{
    std::vector<double> vValues;

    auto lfAddMapStepStock = [&eProduct](std::vector<double>& vVal, std::map<long,CStock> mapStepStock)
    {
        for(auto & pairStep_Stock:mapStepStock)
        {
            double dAmount = pairStep_Stock.second.GetAmount(eProduct);
            vVal.push_back(dAmount);
        }
    };

    if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_INI))
    {
        for(auto & pPart:m_pMarket->GetParticipants())
        {
            if (pPart->GetId()==nPartId)
                lfAddMapStepStock(vValues,pPart->GetStockIniMap());
        }
    }
    else if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_FIN))
    {
        for(auto & pPart:m_pMarket->GetParticipants())
        {
            if (pPart->GetId()==nPartId)
                lfAddMapStepStock(vValues,pPart->GetStockFinMap());
        }
    }
    else if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_CONSUM))
    {
        for(auto & pPart:m_pMarket->GetParticipants())
        {
            if (pPart->GetId()==nPartId)
                lfAddMapStepStock(vValues,pPart->GetLogConsume());
        }
    }
    else if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_PROD))
    {
        for(auto & pPart:m_pMarket->GetParticipants())
        {
            if (pPart->GetId()==nPartId)
                lfAddMapStepStock(vValues,pPart->GetLogProduction());
        }
    }
    else if (nTypeInfo == static_cast<int>(eInfoToShow::SATISF))
    {
        for(auto & pPart:m_pMarket->GetParticipants())
        {
            if (pPart->GetId()==nPartId)
                lfAddMapStepStock(vValues,pPart->GetLogSatisfaction());
        }
    }

    else if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_SENT))
    {
        std::map<int,std::vector<CStock>> mapPartId_vStocksSent =
                this->m_pMarket->GetLedgerRef()->GetSentToMarketForEachStep();

        for(auto & pairPartId_vStockSent:mapPartId_vStocksSent)
        {
            for(auto& stock:pairPartId_vStockSent.second)
            {
                double dAmount = stock.GetAmount(eProduct);
                vValues.push_back(dAmount);
            }
        }
    }
    else if (nTypeInfo == static_cast<int>(eInfoToShow::STOCK_RECEIVED))
    {
        std::map<int,std::vector<CStock>> mapPartId_vStocksReceived =
                this->m_pMarket->GetLedgerRef()->GetReceivedFromMarketEachStep();

        for(auto & pairPartId_vStockRec:mapPartId_vStocksReceived)
        {
            int nPartId = pairPartId_vStockRec.first;
            for(auto& stock:pairPartId_vStockRec.second)
            {
                double dAmount = stock.GetAmount(eProduct);
                vValues.push_back(dAmount);
            }
        }
    }


    return vValues;
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
        //m_psPartiChartGroup->sCheckButtons.mapPartId_sName[nPartId]=std::to_string(nPartId);
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

        }
    }

    return mapPart_Element_Prod_Values;
}

void CMainDialog::run()
{
    long nYLast=20;

    this->m_pWindow = new Fl_Window(1100,580);

    this->m_pWindow->begin();

    this->m_pScroll = new Fl_Scroll(0,0,1100,580);
    this->m_pWindow->add(m_pScroll);


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
    this->m_pScroll->add(m_pMarketPricesChartGroup);
    this->m_pMarketPricesChartGroup->end();

    Fl_Button* butRunScenario_1 = new Fl_Button(100,nYLast,100,20,"run scenario 1");
    nYLast=butRunScenario_1->y()+butRunScenario_1->h();


    butRunScenario_1->callback((Fl_Callback*)CMainDialog::LaunchScenario_1, &m_pMarket);
    this->m_pScroll->add(butRunScenario_1);

    Fl_Button* butRunEditedScenario= new Fl_Button(80,nYLast,130,20,"run edited scenario");
    nYLast=butRunEditedScenario->y()+butRunEditedScenario->h();

    butRunEditedScenario->callback((Fl_Callback*)CMainDialog::LaunchEditedScenario, (void*)this);
    this->m_pScroll->add(butRunEditedScenario);


    Fl_Button* butViewResults = new Fl_Button(100,nYLast,100,20,"view results");
    butViewResults->callback((Fl_Callback*)CMainDialog::ViewResults, (void*)this);
    this->m_pScroll->add(butViewResults);

    nYLast=butViewResults->y()+butViewResults->h();

    Fl_Button* butStep = new Fl_Button(100,nYLast,100,20,"Step");
    butStep->callback((Fl_Callback*)CMainDialog::Step, &m_pMarket);
    this->m_pScroll->add(butStep);

    nYLast=butStep->y()+butStep->h();

    Fl_Button* butReset = new Fl_Button(100,nYLast,100,20,"Reset");
    butReset->callback((Fl_Callback*)CMainDialog::Reset, &m_pMarket);
    this->m_pScroll->add(butReset);

    nYLast=butReset->y()+butReset->h();

    Fl_Button* butDestroy = new Fl_Button(100,nYLast,100,20,"Destroy");
    butDestroy->callback((Fl_Callback*)CMainDialog::Destroy, &m_pMarket);//todo
    this->m_pScroll->add(butDestroy);

    nYLast=butDestroy->y()+butDestroy->h();

    Fl_Button* butCalculator = new Fl_Button(100,nYLast,100,20,"Calculator");
    butCalculator->callback((Fl_Callback*)CMainDialog::Calculator, (void*)this);
    this->m_pScroll->add(butCalculator);

    nYLast=butCalculator->y()+butCalculator->h();


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
//    this->m_psPartiChartGroup = new StructPartiChartGroup(350,300,300,170);
//    m_pWindow->add(m_psPartiChartGroup);
//    //

    //Participants check_buttons
    int nCountInfo=0;

    for(auto & pairInfo_Name:this->mapconstInfo_Name)
    {
        Fl_Check_Button* pCheckButton = new Fl_Check_Button(100, nYLast, 100, 20);

        nYLast=pCheckButton->y()+pCheckButton->h();

        pCheckButton->value(1.0);

        pCheckButton->callback((Fl_Callback*)CMainDialog::ViewResults,(void*)this);

        pCheckButton->label(pairInfo_Name.second.c_str());
        //pCheckButton->label("m_mapInfo_cbShow");
        int nInfo=static_cast<int>(pairInfo_Name.first);
        m_mapInfo_cbShow[nInfo]=pCheckButton;

        m_pScroll->add(pCheckButton);
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

        m_pScroll->add(pCheckButton);

        nYLast=pCheckButton->y()+pCheckButton->h();

        nCountProd++;
    }
    //

    m_pCounterNumPartValue = new Fl_Counter(100,nYLast,100,20);
    m_pCounterNumPartValue->value(2.0);
    m_pCounterNumPartValue->label("Num Part");
    nYLast = m_pCounterNumPartValue->y()+m_pCounterNumPartValue->h();

    m_pCounterNumPartValue->callback((Fl_Callback*)CMainDialog::NumPartUpdated, (void*)this);
    m_pScroll->add(m_pCounterNumPartValue);
    m_pCounterNumPartValue->do_callback();


    //Precios iniciales
    //std::map<def::eProduct,Fl_Check_Button*> m_mapProd_cInitPrice;
    Fl_Box* pInitPricesLabel = new Fl_Box(700,20,100,20,"Init Prices");
    long nProdCount=0;
    for (auto & prod:def::vProducts)
    {
        Fl_Counter* pCounter= new Fl_Counter(700+(nProdCount*100), 40 , 100, 20);
        pCounter->label(def::mapeProductNames.at(prod).c_str());
        pCounter->value(1.0);
        m_sInitPrices.m_mapProd_cInitPrice[prod] = pCounter;
        nProdCount++;

    }
    //

    m_pWindow->end();
    m_pWindow->show();

    std::cout << "Before run" << std::endl;
    Fl::run();
    std::cout << "After run" << std::endl;

}


