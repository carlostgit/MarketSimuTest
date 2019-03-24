#include "Market.h"
#include "PricesInfo.h"
#include "Products.h"
#include "Stock.h"
#include "Prices.h"
#include "Ledger.h"
#include "TransactionInfo.h"
#include "Participant.h"

#include "Tester.h"

#include <limits>
#include <assert.h>
#include <iostream>
#include <cmath>
#include <chrono>

long CMarket::nObectCounter=0;
CMarket::CMarket():m_nId(0),m_nStep(0)
{
    //ctor
    nObectCounter++;
    m_nId=nObectCounter;
}

CMarket::~CMarket()
{
    //dtor
}

const bool PRINT = false;

void CMarket::Step()
{
    m_nStep++;
//    //double dGainNumerator=1000000;
//    double dMaxGain=100;
//
//    CPricesInfo pricinfoCurrent = this->GetPricesInfo();
//    CPricesInfo pricinfoNew = this->GetPricesInfo();
//    long nMaxCount=1000;
//    for (long nCount=1; nCount<=nMaxCount; nCount++)
//    {
//        double dGain = dMaxGain/((double)nCount);
//        AdjustPrices(dGain, &pricinfoCurrent,&pricinfoNew);
//        pricinfoCurrent = pricinfoNew;
//    }
//    this->SetPricesInfo(pricinfoNew);
//    MakeTransactions();


    //CLedger m_Ledger;
    //CPricesInfo m_PricesInfo;
    //std::vector<CParticipant*> m_vpParticipants;


    this->CalculatePrice(m_vpParticipants,this,&m_PricesInfo);
    MakeTransactions();

    for(auto & participant:m_vpParticipants)
    {
        participant->StepAfterTransaction(&m_PricesInfo);
    }
}

void CMarket::CalculatePrice(std::vector<CParticipant*>& vpParticipants, CMarket* pMarket, CPricesInfo* pPricesInfo)
{
    //convertir este método en static
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
        for(auto const pParticipant:vpParticipants)
        {
            //CParticipant* pParticipant= pupParticipant.get();
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

//    for (auto & val:vFoodValues)
//        std::cout<<val<<"\n";
//    std::cout<< std::endl;

    //Test::DrawGraphics(&mapStrVDouble,1.0);

}


//void CMarket::AdjustPrices(double dGain,CPricesInfo* pOriginalPricesInfo, CPricesInfo* pAdjustedPricesInfo)
//{
//
//    //Resultado por defecto
//    (*pAdjustedPricesInfo)=(*pOriginalPricesInfo);
//
//    if(m_vpParticipants.size()<2)
//    {
//        assert(false=="Too few participants in CMarket::AdjustPrices");
//        return;
//    }
//
//    long nCount=0;
//    long nLastIterationMaxPriceIncreasing=0;
//    long nLastIterationMinPriceDecreasing=0;
//
////    if(0==pOriginalPricesInfo->GetStockForPricesRef())
////    {
////        assert(false=="No StockForPrices in OriginalPricesInfo in CMarket::AdjustPrices");
////    }
//
//    if(pOriginalPricesInfo->GetStockForPrices().GetProducts().empty())
//    {
//        assert(false=="No products in CMarket::AdjustPrices");
//    }
//
//    def::eProduct eprodCurrency = pOriginalPricesInfo->GetStockForPrices().GetProducts().front();
//
//    //CPrices podría ser un std::map<CProduct,double> pero con información extra de la moneda
//    CPrices pricesMax = pOriginalPricesInfo->GetPrices(eprodCurrency);
//    CPrices pricesMin = pOriginalPricesInfo->GetPrices(eprodCurrency);
//
////    std::vector<CStock> vstockForPricesLog;
////    long nMaxStockForPricesLogSize=10;
////    vstockForPricesLog.reserve(nMaxStockForPricesLogSize);
////    vstockForPricesLog.push_back(pOriginalPricesInfo->GetStockForPrices());
//
//    CPricesInfo pricinfoCurrent;
//    pricinfoCurrent.SetStockForPrices(pOriginalPricesInfo->GetStockForPrices());
//
//    if(PRINT)
//    {
//        std::cout << "1 pricinfoCurrent:" << std::endl;
//        pricinfoCurrent.PrintInfo();
//    }
//
//    CStock ostockCurrentSumOfProdInOutsOfMarket;
//    CStock ostockOriginalSumOfProdInOutsOfMarket;
//    bool bExit=false;
//    do
//    {
//        //sum of transactions with current prices
//        for (auto& pPartic : (this->m_vpParticipants))
//        {
//            std::unique_ptr<CTransactionInfo> upTrans = GenerateTransactionInfoWithParticipant(this, pPartic,&pricinfoCurrent);
//            ostockCurrentSumOfProdInOutsOfMarket += upTrans->GetSentToMarket() - upTrans->GetReceivedFromMarket();
//        }
//
//        if(0==nCount)
//            ostockOriginalSumOfProdInOutsOfMarket=ostockCurrentSumOfProdInOutsOfMarket;
//
//
//
//        if(PRINT)
//        {
//            std::cout << "2 ostockCurrentSumOfProdInOutsOfMarket:" << std::endl;
//            ostockCurrentSumOfProdInOutsOfMarket.PrintInfo();
//        }
//        //Normalizo, y amplifico los cambios según el valor dGain
//        double dMaxAbsChangeInStock = ostockCurrentSumOfProdInOutsOfMarket.GetMaxModule();
//        CStock ostockNormalizedSumOfProdInOutsOfMarket = ostockCurrentSumOfProdInOutsOfMarket/dMaxAbsChangeInStock;
//
//
//
//        if(PRINT)
//        {
//            std::cout << "3 ostockNormalizedSumOfProdInOutsOfMarket:" << std::endl;
//            ostockNormalizedSumOfProdInOutsOfMarket.PrintInfo();
//
//            std::cout << "5 pricinfoCurrent:" << std::endl;
//            pricinfoCurrent.PrintInfo();
//            std::cout << "6 ostockNormalizedSumOfProdInOutsOfMarket:" << std::endl;
//            ostockNormalizedSumOfProdInOutsOfMarket.PrintInfo();
//
//            std::cout << "dGain" << std::endl;
//            std::cout << dGain << std::endl;
//
//        }
//        CStock ostockForNewPrices = (pricinfoCurrent.GetStockForPrices()) + (ostockNormalizedSumOfProdInOutsOfMarket*dGain);
//
//
//
//        if(PRINT)
//        {
//            std::cout << "7 ostockForNewPrices:" << std::endl;
//            ostockForNewPrices.PrintInfo();
//        }
//
//        //Sumo o resto para que el producto con menos cantidad en el stock para precios tenga una cantidad de 1
//        double dMinOfNewPricesStock = std::abs(ostockForNewPrices.GetMin());
//        ostockForNewPrices.AddAmountForEachProd(dMinOfNewPricesStock+1.0);
//
//        //Actualizo prinfoCurrent
//        pricinfoCurrent.SetStockForPrices(ostockForNewPrices);
//
//
//
//        if(PRINT)
//        {
//            //Printing traces
//            std::cout << "pOriginalPricesInfo: " << std::endl;
//            pOriginalPricesInfo->PrintInfo();
//            std::cout << "pricinfoCurrent: " << std::endl;
//            pricinfoCurrent.PrintInfo();
//            //
//        }
//
//        //Actualizo stockMax y stockMin
//        std::vector<def::eProduct> vProds = pOriginalPricesInfo->GetStockForPrices().GetProducts();
//        for(auto& prod : vProds)
//        {
//            if(pricinfoCurrent.GetPrice(eprodCurrency,prod) > pricesMax.GetPrice(prod))
//            {
//                pricesMax.SetPrice(prod,pricinfoCurrent.GetPrice(eprodCurrency,prod));
//                nLastIterationMaxPriceIncreasing=nCount;
//            }
//            if(pricinfoCurrent.GetPrice(eprodCurrency,prod) < pricesMin.GetPrice(prod))
//            {
//                pricesMin.SetPrice(prod,pricinfoCurrent.GetPrice(eprodCurrency,prod));
//                nLastIterationMinPriceDecreasing=nCount;
//            }
//        }
//
//        nCount++;
//
//        //Loop exit conditions:
//        //Con demasiadas iteraciones salimos del loop sacando un assert
//        if (nCount>100)
//        {
//            assert(false=="Too many iterations in CMarket::AdjustPrices");
//            return;
//        }
//
//        //Si todos los precios dejan de crecer continuamoent o disminuir continuamente, entonces salimos del bucle
//        bool bTooManyIteratWithinBoundaries=false;
//        long nMaxNumOfIteratWithinBoundaries=10;
//        long nNumOfIteratWithoutMaxPriceIncrement = nCount-nLastIterationMaxPriceIncreasing;
//        long nNumOfIteratWithoutMinPriceDecrement = nCount-nLastIterationMaxPriceIncreasing;
//        if(nNumOfIteratWithoutMaxPriceIncrement > nMaxNumOfIteratWithinBoundaries &&
//                nNumOfIteratWithoutMinPriceDecrement > nMaxNumOfIteratWithinBoundaries)
//        {
//            bTooManyIteratWithinBoundaries=true;
//        }
//
//        bool bExit=false;
//        if(bTooManyIteratWithinBoundaries)
//        {
//            bExit=true;
//        }
//
//        if(bExit)
//        {
//            //Solo si el nuevo stock tiene menos error, me quedo con el nuevo
//            if(ostockCurrentSumOfProdInOutsOfMarket.GetSumOfAbs()<ostockOriginalSumOfProdInOutsOfMarket.GetSumOfAbs())
//            {
//                pAdjustedPricesInfo->SetStockForPrices(ostockForNewPrices);
//            }
//            else
//            {
//                (*pAdjustedPricesInfo)=(*pOriginalPricesInfo);
//            }
//        }
//    }
//    while (false==bExit);
//
//}

std::unique_ptr<CTransactionInfo> CMarket::CalculateTransactionInfoWithParticipant(CMarket* pMarket, CParticipant* pPartic, CPricesInfo* pPricesInfo, double* dPerformanceSeconds)
{
    auto startLoop1 = std::chrono::system_clock::now();

    CStock* pstockIniOfPartic = pPartic->GetStockRef();

    if(PRINT)
    {
        std::cout << "pstockIniOfPartic:" << std::endl;
        pstockIniOfPartic->PrintInfo();
    }
    //Sale
    def::eProduct prodSaleCurrency=def::NON_IDENTIFIED_PRODUCT;
    CStock ostockProdsToOffer = pPartic->OfferProducts(pPricesInfo,pstockIniOfPartic ,&prodSaleCurrency);
    assert(pstockIniOfPartic->IsEnoughToSell(&ostockProdsToOffer));

    if(PRINT)
    {
        std::cout << "ostockProdsToOffer:" << std::endl;
        ostockProdsToOffer.PrintInfo();
    }

    CStock ostockMoneyGotFromSale = pPricesInfo->GetPriceOfStock(ostockProdsToOffer,prodSaleCurrency);
    CStock ostockAfterSaleOfPartic = (*pstockIniOfPartic) - ostockProdsToOffer + ostockMoneyGotFromSale;

    if(PRINT)
    {
        std::cout << "ostockMoneyGotFromSale:" << std::endl;
        ostockMoneyGotFromSale.PrintInfo();

        std::cout << "ostockAfterSaleOfPartic:" << std::endl;
        ostockAfterSaleOfPartic.PrintInfo();
    }

    auto endLoop1 = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds_Loop1 = endLoop1-startLoop1;
    double elapsed_seconds = elapsed_seconds_Loop1.count();

    Test::AddTime("CMarket::GenerateTransactionInfoWithParticipant: ", elapsed_seconds);
    //Purchase
    def::eProduct prodPurchaseCurrency=def::NON_IDENTIFIED_PRODUCT;
    CStock ostockProdsToBuy = pPartic->DemandProducts(pPricesInfo,(&ostockAfterSaleOfPartic),&prodPurchaseCurrency);

    if(PRINT)
    {
        std::cout << "ostockProdsToBuy:" << std::endl;
        ostockProdsToBuy.PrintInfo();
    }

    CStock ostockMoneyUsedToPurchase = pPricesInfo->GetPriceOfStock(ostockProdsToBuy,prodPurchaseCurrency);
    assert(ostockAfterSaleOfPartic.IsEnoughToSell(&ostockMoneyUsedToPurchase));

    //Reg transaction
    CStock ostockSentToMarket = ostockProdsToOffer + ostockMoneyUsedToPurchase;
    CStock ostockGotFromMarket = ostockMoneyGotFromSale + ostockProdsToBuy;



    if (dPerformanceSeconds)
    {
        (*dPerformanceSeconds)=elapsed_seconds;
    }

    return std::unique_ptr<CTransactionInfo>(new CTransactionInfo(pMarket,
                                                                    pPartic,
                                                                    ostockSentToMarket,
                                                                    ostockGotFromMarket));

}

void CMarket::MakeTransactions()
{
    for (auto& pPartic : m_vpParticipants)
    {
        CPricesInfo pricesInfo = GetPricesInfo();

        //Transactions with market
        std::unique_ptr<CTransactionInfo> upTransInfo = CalculateTransactionInfoWithParticipant(this, pPartic,&pricesInfo);

        pPartic->AddProductsForNextCycle(upTransInfo->GetReceivedFromMarket());
        pPartic->RemoveProductsForNextCycle(upTransInfo->GetSentToMarket());

        m_Ledger.AddTransaction(std::move(upTransInfo));
    }
}

