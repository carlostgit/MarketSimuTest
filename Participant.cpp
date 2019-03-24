#include "Participant.h"

#include "Stock.h"
#include "PricesInfo.h"

#include <algorithm>

#include <iostream>
#include <fstream>

#include "Tester.h"

const bool PRINT = false;


class FMaxSatisfactinPerCost
{
    FMaxSatisfactinPerCost(const CParticipant* pParticipant, const CPricesInfo* pricinfoPrices):
        m_pParticipant{pParticipant},
        m_pricinfoPrices{pricinfoPrices}
         {;}
    bool operator()(def::eProduct prodA,def::eProduct prodB)
    {
            double dValueA = m_pParticipant->GetSatisfactionOfProduct(prodA);
            double dValueB = m_pParticipant->GetSatisfactionOfProduct(prodB);

            double dPriceA = m_pricinfoPrices->GetPrice(def::PROD_GOLD,prodA); //En el futuro habrá que añadir la posibilidad de usar otra moneda
            double dPriceB = m_pricinfoPrices->GetPrice(def::PROD_GOLD,prodB);
            double dValuePerPriceA = dValueA/dPriceA;
            double dValuePerPriceB = dValueB/dPriceB;

            return dValuePerPriceA<dValuePerPriceB;
    }
    const CParticipant* m_pParticipant;
    const CPricesInfo* m_pricinfoPrices;

};

class FMaxSatisfactinPerProduction
{
public:
    FMaxSatisfactinPerProduction(const CParticipant* pParticipant, const CPricesInfo* pricinfoPrices):
        m_pParticipant{pParticipant},
        m_pricinfoPrices{pricinfoPrices}
         {;}
    bool operator()(def::eProduct prodA,def::eProduct prodB)
    {
            double dValueA = m_pParticipant->GetSatisfactionOfProduct(prodA);
            double dValueB = m_pParticipant->GetSatisfactionOfProduct(prodB);

            double dPriceA = m_pricinfoPrices->GetPrice(def::PROD_GOLD,prodA); //En el futuro habrá que añadir la posibilidad de usar otra moneda
            double dPriceB = m_pricinfoPrices->GetPrice(def::PROD_GOLD,prodB);
            double dValuePerPriceA = dValueA/dPriceA;
            double dValuePerPriceB = dValueB/dPriceB;

            double dProdCapacityA = m_pParticipant->GetPrCapacityOfProduct(prodA);
            double dProdCapacityB = m_pParticipant->GetPrCapacityOfProduct(prodB);

            double dValuePerProductionA = dValuePerPriceA*dProdCapacityA;
            double dValuePerProductionB = dValuePerPriceB*dProdCapacityB;

            return dValuePerProductionA<dValuePerProductionB;
    }
    const CParticipant* m_pParticipant;
    const CPricesInfo* m_pricinfoPrices;

};

long CParticipant::nObjectCounter = 0;
CParticipant::CParticipant():m_nId(0),m_nStep(0)
{
    nObjectCounter++;
    m_nId=nObjectCounter;
    //ctor

    //Default satisfaction is 1 for every product
    for(int i=0;i<def::NUM_OF_PROD_TYPES;i++)
    {
        m_mapeProd_dSatisf[static_cast<def::eProduct>(i)]=1.0;
    }

    //Default production capacity is 0 for every product
    for(int i=0;i<def::NUM_OF_PROD_TYPES;i++)
    {
        m_mapeProd_dPrCapacity[static_cast<def::eProduct>(i)]=0.0;
    }

    //Default consumption is 0 for every product
    for(int i=0;i<def::NUM_OF_PROD_TYPES;i++)
    {
        m_mapeProd_dConsumption[static_cast<def::eProduct>(i)]=0.0;
    }
}

CParticipant::~CParticipant()
{
    //dtor
}

CStock CParticipant::OfferProducts(const CPricesInfo* pricinfoPrices, const CStock* pStockIniOfPartic, def::eProduct* pProdSaleCurrency) const
{
    //Versión simple:
    //Primero se venden siempre todos los productos
    (*pProdSaleCurrency) = def::PROD_GOLD;

    if(PRINT)
    {
        std::ofstream myOfStream;
        myOfStream.open("CParticipant_OfferProducts.txt");

        myOfStream << "FInput " << (*pStockIniOfPartic).GetAmount(def::eProduct::PROD_FOOD) << "$\t";
        myOfStream << "CInput " << (*pStockIniOfPartic).GetAmount(def::eProduct::PROD_CLOTH) << "$\t";
        myOfStream << "GInput " << (*pStockIniOfPartic).GetAmount(def::eProduct::PROD_GOLD) << "$\t";

        myOfStream.close();
    }

    return (*pStockIniOfPartic);
}

CStock CParticipant::DemandProducts(const CPricesInfo* pricinfoPrices, const CStock* pStockAfterSaleOfPartic, def::eProduct* pProdPurchaseCurrency) const
{
    //Versión simple:
    //Maximizar el valor total de satisfacción

    (*pProdPurchaseCurrency) = def::PROD_GOLD;

    std::vector<def::eProduct> vProds = (*pStockAfterSaleOfPartic).GetProducts();

    std::ofstream myOfStream;
    if(PRINT)
    {
        myOfStream.open("CParticipant_DemandProducts.txt");
    }

    def::eProduct eprodToBuy = *(std::max_element(vProds.begin(),vProds.end(),[&](def::eProduct peprodA, def::eProduct peprodB)
        {//Max satisfaction per $ function
            def::eProduct prodA=(peprodA);
            def::eProduct prodB=(peprodB);
            double dValueA = this->GetSatisfactionOfProduct(prodA);
            double dValueB = this->GetSatisfactionOfProduct(prodB);
            double dPriceA = pricinfoPrices->GetPrice((*pProdPurchaseCurrency),prodA);
            double dPriceB = pricinfoPrices->GetPrice((*pProdPurchaseCurrency),prodB);
            double dValuePerPriceA = dValueA/dPriceA;
            double dValuePerPriceB = dValueB/dPriceB;

            if (PRINT)
            {
                myOfStream << "dValuePerPriceA " << def::mapeProductNames.at(prodA) << " " <<dValuePerPriceA<< "\n";
                myOfStream << "dValuePerPriceB " << def::mapeProductNames.at(prodB) << " " <<dValuePerPriceB<< "\n";
            }

            return dValuePerPriceA<dValuePerPriceB;
        }
        ));

        //TODO: sustituir lo anterior por algo como lo siguiente
        //FMaxSatisfactinPerCost functMaxSati(this, pricinfoPrices);
    //def::eProduct eprodToBuy = *(std::max_element(vProds.begin(),vProds.end(),functMaxSati));

    double dPriceOfProdToBuy = pricinfoPrices->GetPrice((*pProdPurchaseCurrency),eprodToBuy);
    double dAmountOfMoney = pStockAfterSaleOfPartic->GetAmount(*pProdPurchaseCurrency);

    long nAmountOfProductsToBuy = dAmountOfMoney/dPriceOfProdToBuy;

    CStock stockToBuy;
    stockToBuy.Set(eprodToBuy, nAmountOfProductsToBuy);

    if(PRINT)
    {
        myOfStream << "eprodToBuy " << def::mapeProductNames.at(eprodToBuy) << " " <<nAmountOfProductsToBuy<< "\n";

        myOfStream.close();
    }

    //TODO: En el futuro el método debería ir haciendo sl stock demandado unidad a unidad,
    //porque el método GetSatisfactionOfProduct dependerá del Stock. (El Stock actual + el stock que se está demandando)

    return stockToBuy;
}

double CParticipant::GetSatisfactionOfProduct(def::eProduct eprod) const
{
    //std::map<def::eProduct, double> m_mapeProd_dSatisf;
    //std::cout << "GetSatisfactionOfProduct" << std::endl;
    //std::cout << "m_mapeProd_dSatisf->size() " << m_mapeProd_dSatisf.size() << std::endl;


    if(m_mapeProd_dSatisf.end()!=m_mapeProd_dSatisf.find(eprod))
    {
       return m_mapeProd_dSatisf.at(eprod);
    }
    else
    {
        return 0.0;
    }
}

void CParticipant::AddProductsForNextCycle(CStock stock)
{
    m_Stock += stock;
}

void CParticipant::RemoveProductsForNextCycle(CStock stock)
{
    m_Stock -= stock;
}

CStock CParticipant::CalculateProduction(const CPricesInfo* pricinfoPrices) const
{
    CStock stockProduced;

    std::vector<def::eProduct> vProds = def::vProducts;

    FMaxSatisfactinPerProduction functMaxSati(this, pricinfoPrices);
    def::eProduct eprodToProduce = *(std::max_element(vProds.begin(),vProds.end(),functMaxSati));

    double dAmountProduced=eprodToProduce*this->GetPrCapacityOfProduct(eprodToProduce);

    stockProduced.AddAmount(eprodToProduce,dAmountProduced); //faltaba este metodo

    return stockProduced;
}

double CParticipant::GetPrCapacityOfProduct(def::eProduct eprod) const
{
    if(m_mapeProd_dPrCapacity.end()!=m_mapeProd_dPrCapacity.find(eprod))
    {
       return m_mapeProd_dPrCapacity.at(eprod);
    }
    else
    {
        return 0.0;
    }
}

void CParticipant::StepAfterTransaction(const CPricesInfo* pricinfoPrices)
{
    m_nStep++;

    m_mapLogStockIni[m_nStep]=m_Stock;

    Consume();
    Produce(pricinfoPrices);

    m_mapLogStockFin[m_nStep]=m_Stock;


}

void CParticipant::Produce(const CPricesInfo* pricinfoPrices)
{
    CStock stockToBeProduced = CalculateProduction(pricinfoPrices);
    AddProductsForNextCycle(stockToBeProduced);

    this->m_mapLogProduction[m_nStep]=stockToBeProduced;
}

//Consumption
CStock CParticipant::CalculateConsumption(const CStock* pownedStock) const
{
    CStock stockToBeConsumed;

    for (auto & pairProdConsumption:this->m_mapeProd_dConsumption)
    {
        def::eProduct prod=pairProdConsumption.first;
        double dConsumption = pairProdConsumption.second;

        double dOwnedAmount = pownedStock->GetAmount(prod);

        if(dConsumption<dOwnedAmount)
            stockToBeConsumed.SetAmount(prod, dConsumption);
        else
            stockToBeConsumed.SetAmount(prod, dOwnedAmount);
    }

    return stockToBeConsumed;
}
double CParticipant::GetConsumptionOfProduct(def::eProduct eprod) const
{
    if(m_mapeProd_dConsumption.end()!=m_mapeProd_dConsumption.find(eprod))
    {
       return m_mapeProd_dConsumption.at(eprod);
    }
    else
    {
        return 0.0;
    }
}

void CParticipant::Consume()
{
    CStock stockToBeConsumed = CalculateConsumption(&this->m_Stock);
    RemoveProductsForNextCycle(stockToBeConsumed);

    this->m_mapLogConsume[m_nStep]=stockToBeConsumed;
}

void CParticipant::DrawInfo()
{
//    std::map<long,CStock> m_mapLogStockIni; //Inicial y final?
//    std::map<long,CStock> m_mapLogStockFin; //Inicial y final?
//    std::map<long,CStock> m_mapLogConsume;
//    std::map<long,CStock> m_mapLogProduction;
//    std::map<long,CStock> m_mapLogSatisfaction; //TODO


    std::map<std::string, std::vector<double> > mapStrVDouble;

    auto lf_addMapLogToDrawer = [&mapStrVDouble](std::map<long,CStock> mapLogStock,
                                                 std::string sName)
    {
        for(auto & pairStep_Stock:mapLogStock)
        {
    //        std::cout<< "Step: " << pairStep_Stock.first;

            for(auto & prod:def::vProducts)
            {
                std::string str = def::mapeProductNames.at(prod);
                std::string strNameOfGraph= str+" "+sName;
                CStock stock = pairStep_Stock.second;
    //            std::cout << "Stock: " ;
    //            stock.PrintInfo();
                double dAmount = stock.GetAmount(prod);
                if(mapStrVDouble.end()==mapStrVDouble.find(strNameOfGraph))
                {
                    std::vector<double> vValues;
                    vValues.push_back(dAmount);
                    mapStrVDouble.insert(std::make_pair(strNameOfGraph,vValues));
                }
                else
                {
                    mapStrVDouble.at(strNameOfGraph).push_back(dAmount);
                }
            }
        }
    };

    lf_addMapLogToDrawer(m_mapLogStockIni,"StockIni");
    lf_addMapLogToDrawer(m_mapLogStockFin,"StockFin");
    lf_addMapLogToDrawer(m_mapLogConsume,"Consume");
    lf_addMapLogToDrawer(m_mapLogProduction,"Production");
    lf_addMapLogToDrawer(m_mapLogSatisfaction,"Satisfaction");

//        for(auto & pairStep_StockIni:m_mapLogStockIni)
//        {
//    //        std::cout<< "Step: " << pairStep_StockIni.first;
//
//            for(auto & prod:def::vProducts)
//            {
//                std::string str = def::mapeProductNames.at(prod);
//
//                //Amount Ini
//
//                std::string strStockIni= str+" StockIni";
//                CStock stockIni = pairStep_StockIni.second;
//    //            std::cout << "Stock: " ;
//    //            stockIni.PrintInfo();
//                double dAmountIni = stockIni.GetAmount(prod);
//                if(mapStrVDouble.end()==mapStrVDouble.find(strStockIni))
//                {
//                    std::vector<double> vValues;
//                    vValues.push_back(dAmountIni);
//                    mapStrVDouble.insert(std::make_pair(strStockIni,vValues));
//                }
//                else
//                {
//                    mapStrVDouble.at(strStockIni).push_back(dAmountIni);
//                }
//            }
//        }
//
//    for(auto & pairStep_StockFin:m_mapLogStockFin)
//    {
////        std::cout<< "Step: " << pairStep_StockFin.first;
//
//        for(auto & prod:def::vProducts)
//        {
//            std::string str = def::mapeProductNames.at(prod);
//
//            //Amount Fin
//
//            std::string strStockFin= str+" StockFin";
//            CStock stockFin = pairStep_StockFin.second;
////            std::cout << "Stock: " ;
////            stockFin.PrintInfo();
//            double dAmountFin = stockFin.GetAmount(prod);
//            if(mapStrVDouble.end()==mapStrVDouble.find(strStockFin))
//            {
//                std::vector<double> vValues;
//                vValues.push_back(dAmountFin);
//                mapStrVDouble.insert(std::make_pair(strStockFin,vValues));
//            }
//            else
//            {
//                mapStrVDouble.at(strStockFin).push_back(dAmountFin);
//            }
//        }
//    }


    //TODO: Añadir el resto de logs

//    for (auto & val:vFoodValues)
//        std::cout<<val<<"\n";
//    std::cout<< std::endl;

    Test::DrawGraphics(&mapStrVDouble,1.0);
}
