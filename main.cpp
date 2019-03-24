#include <iostream>

#include "defines.h"
#include "Market.h"
#include "Participant.h"
#include "PricesInfo.h"
//#include "Ledger.h"
#include "Tester.h"

#include "MainDialog.h"

//using namespace std;

void PlayScenario_1()
{
    std::cout << "Inside PlayScenario1()" << std::endl;

    CMarket* pMarket = new CMarket();
    CParticipant* pParticipant_1 = new CParticipant();
    CParticipant* pParticipant_2 = new CParticipant();
    //CLedger* pLedger = new CLedger();
    //pMarket->SetLedger(pLedger);
    pMarket->AddParticipant(pParticipant_1);
    pMarket->AddParticipant(pParticipant_2);

    std::map<def::eProduct,double> mapPart_1_eProd_dAmount;
    mapPart_1_eProd_dAmount[def::PROD_CLOTH]=1.0;
    mapPart_1_eProd_dAmount[def::PROD_FOOD]=1.0;
    mapPart_1_eProd_dAmount[def::PROD_GOLD]=1.0;
    CStock stockPart1(mapPart_1_eProd_dAmount);
    pParticipant_1->SetStock(stockPart1);

    std::map<def::eProduct,double> mapPart_2_eProd_dAmount;
    mapPart_2_eProd_dAmount[def::PROD_CLOTH]=1.0;
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

    for (int i=0;i<2;i++)
    {
        pMarket->Step();
    }


    //Logs
    pMarket->GetLedgerRef()->DrawInfo();
    pParticipant_1->DrawInfo();
    pParticipant_2->DrawInfo();


    delete pMarket;
    delete pParticipant_1;
    delete pParticipant_2;
    //delete pLedger;

}


void RunTests()
{
    std::cout << "Inside RunTests()" << std::endl;

    CTester tester=CTester();
    //tester.TestStock();
    tester.TestParticipantOfferAndDemand_2();

}


void RunDialog()
{
    CMainDialog mainDialog;
    mainDialog.run();

    system("pause");
}

int main()
{
    std::cout << "Hello world!" << std::endl;

    //PlayScenario_1();

    //RunTests();

    RunDialog();


    return 0;
}
