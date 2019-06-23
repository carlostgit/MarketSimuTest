#include "Participant.h"

#include "Stock.h"
#include "PricesInfo.h"

#include <algorithm>

#include <iostream>
#include <fstream>
#include <cmath>
#include <functional>

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

    //Esto creo que es un bug. Creo que debería usarse def::vProducts en vez de vProds

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


//

CStock CParticipant::DemandProducts2(const CPricesInfo* pricinfoPrices, const CStock* pStockAfterSaleOfPartic, def::eProduct* pProdPurchaseCurrency) const
{
    //Se maximizará: Satisf = satisfA + satisfB + satisfC + (satisfA*satisfB + satisfA*satisfC + satisfB*satisfC)/(satisfA + satisfB + satisfC);
    //Donde: satisfA = numProdA*satisfPerA;
    //Todo: Una siguiente idea podría ser agrupar los productos en categorías (categorías A, B, o C), y límitar el número de productos por categoria,
    //de tal forma que si sobran los recursos se tienda a demandar productos de más categoría para maximizar la satisfacción
    //

    //De momento, defino aquí la función de satisfacción que voy a usar en el algoritmo
    //std::function<double (const CParticipant&,const CStock*)> GetSatisfOfStockFunction = &CParticipant::GetSatisfactionOfStockWithCategories2;
    std::function<double (const CStock*)> GetSatisfOfStockFunction = [&](const CStock* stock){
        return this->GetSatisfactionOfStockWithCategories2(stock);
    };
    //

    CStock stockToBuy;

    def::eProduct epPurchaseCurrency=def::PROD_GOLD;
    (*pProdPurchaseCurrency) = def::PROD_GOLD;

    std::vector<def::eProduct> vInitProds = (*pStockAfterSaleOfPartic).GetProducts();

    //std::vector<long> vnCategories = def::GetCategoriesOfStock();

    std::vector<def::eProduct> vProductsToBuy;
    CStock stockProductsToBuy;
    double dAmountOfMoney = pStockAfterSaleOfPartic->GetAmount(*pProdPurchaseCurrency);
    double dRemainingMoney=dAmountOfMoney;
    bool bMoneyFinished=false;
    while (!bMoneyFinished)
    {
        //def::eProduct eProdCandidate = def::vProducts.front();
        bool bCanBuyAnyProduct=false;

        double dMaxSatisfPerCost=0.0;
        def::eProduct epSelectectProd=def::NON_IDENTIFIED_PRODUCT;
        for(auto & prod: def::vProducts)
        {
            bool bCanBuyProduct=false;
            double dPriceOfProd = pricinfoPrices->GetPrice(epPurchaseCurrency,prod);
            if(dPriceOfProd<=dRemainingMoney)
            {
                bCanBuyProduct=true;
                bCanBuyAnyProduct=true;
            }

            if (bCanBuyProduct)
            {
                //satisf per cost
                double dProdSatisf = m_mapeProd_dSatisf.at(prod);
                const CStock& stockWithoutCandidate = stockProductsToBuy;
                CStock stockWithCandidate=stockProductsToBuy;
                stockWithCandidate.AddAmount(prod,1.0);

                //todo

                double dSatisfWithoutCandidate = GetSatisfOfStockFunction(&stockWithoutCandidate);
                double dSatisfWithCandidate = GetSatisfOfStockFunction(&stockWithCandidate);
                double dSatisfPerCost = (dSatisfWithCandidate-dSatisfWithoutCandidate)/dPriceOfProd;

                if (dSatisfPerCost>dMaxSatisfPerCost)
                {
                    dMaxSatisfPerCost = dSatisfPerCost;
                    epSelectectProd=prod;
                }

            }

        }

        if (false==bCanBuyAnyProduct)
        {
            bMoneyFinished=true;

            CStock stockWithCandidate=stockProductsToBuy;
        }
        else
        {
            stockProductsToBuy.AddAmount(epSelectectProd,1.0);
        }

    }

    CStock stockBeforeChange = stockToBuy;

    //Metodo de cambiar unos productos por otros para intentar mejorar la satisfacción

    CStock stockWithChange = stockBeforeChange;

    //Ahora se prueba a cambiar unos productos por otros
    bool bAnyGoodChangeFound = false;
    do
    {
        const CStock stockWithChangeConst = stockWithChange;

        //dRemainingMoney
        for(auto & prodToBeAdded: def::vProducts)
        {
            //std::map<def::eProduct,dAmount> mapProd_AmountToRemove;

            double dSatisfWithoutChange = GetSatisfOfStockFunction(&stockWithChangeConst);

            CStock stockWithProd = stockWithChangeConst;
            stockWithProd.AddAmount(prodToBeAdded,1.00);

            const double dPriceOfProdToBeAdded = pricinfoPrices->GetPrice(epPurchaseCurrency,prodToBeAdded);
            const double dRemainingMoneyWithProd = dRemainingMoney - dPriceOfProdToBeAdded; //Tendrá q ser negativo

            const double dSatisfWithProd = GetSatisfOfStockFunction(&stockWithProd);

            for(auto & prodToBeRemoved: def::vProducts)
            {
                if(prodToBeAdded==prodToBeRemoved)
                    continue;

                if(stockWithChange.GetAmount(prodToBeRemoved) < 1.0)
                    continue;

                const double dPriceOfProdToBeRemoved = pricinfoPrices->GetPrice(epPurchaseCurrency,prodToBeRemoved);

                CStock stockWithChangeTemp = stockWithChange;
                double dRemainingMoneyWithChange = dRemainingMoneyWithProd;
                while (dRemainingMoneyWithChange < 0 && stockWithChangeTemp.GetAmount(prodToBeRemoved) > 1.0)
                {
                    stockWithChangeTemp = stockWithChange.AddAmount(prodToBeRemoved,-1.0);
                    dRemainingMoneyWithChange = dRemainingMoneyWithProd + dPriceOfProdToBeRemoved;
                }

                if(dRemainingMoneyWithChange<0.0)
                    continue; //
                else
                {
                    double dSatisfWithChange = GetSatisfOfStockFunction(&stockWithChangeTemp);
                    if (dSatisfWithChange > dSatisfWithoutChange)
                    {
                        stockWithChange = stockWithChangeTemp;
                        bAnyGoodChangeFound = true;
                        break;
                    }
                }
            }
            if(bAnyGoodChangeFound)
                break;
        }
        if(bAnyGoodChangeFound)
            break;

    }while ( bAnyGoodChangeFound );


    return stockToBuy;
}
//


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

//double CParticipant::GetSatisfactionOfStock(const CStock* pStock) const
//{
//    //Use: m_mapeProd_dSatisf
//
//    if(pStock)
//    {
//        double dTotalSatisfaction=0.0;
//        std::map<def::eProduct,double> mapProd_Amount = pStock->GetProductAmountMap();
//
//        for (auto& pairProd_Amount:mapProd_Amount)
//        {
//            double dProdSatisfaction = m_mapeProd_dSatisf.at(pairProd_Amount.first)*pairProd_Amount.second;
//            dTotalSatisfaction+=dProdSatisfaction;
//
//        }
//        return dTotalSatisfaction;
//    }
//    else
//    {
//        return 0.0;
//    }
//}

double CParticipant::GetSatisfactionOfStock(const CStock* pStock) const
{
    return GetSatisfactionOfStock(pStock,m_mapeProd_dSatisf);
}

double CParticipant::GetSatisfactionOfStock(const CStock* pStock, std::map<def::eProduct, double> mapeProd_dSatisf)
{
    double dSatisf=0.0;

    std::vector<std::pair<def::eProduct,def::eProduct>> vpairProdA_ProdB;

    for (int indexA=0;indexA<def::vProducts.size();indexA++)
    {
        def::eProduct prodA=def::vProducts.at(indexA);

        for (int indexB=indexA+1;indexB<def::vProducts.size();indexB++)
        {
            def::eProduct prodB=def::vProducts.at(indexB);
            vpairProdA_ProdB.push_back(std::make_pair(prodA,prodB));
        }
    }

    std::cout<<"vpairProdA_ProdB.size()"<<vpairProdA_ProdB.size() << std::endl;
    for (auto & pairA_B:vpairProdA_ProdB)
    {
        std::cout<<"pairA_B.first"<<pairA_B.first << std::endl;
        std::cout<<"pairA_B.second"<<pairA_B.second << std::endl;
    }

    std::map<def::eProduct, double> mapProd_Satisf;
    for (auto & prod:def::vProducts)
    {
        mapProd_Satisf[prod]=pStock->GetAmount(prod)*mapeProd_dSatisf.at(prod);
    }

    double dSatisfSum=0.0;
    for (auto & pairProdSatisf:mapProd_Satisf)
    {
        dSatisfSum += pairProdSatisf.second;
    }

    std::cout<<"dSatisfSum"<<dSatisfSum << std::endl;

    double dSatisfSumOfPairProducts=0.0;
    for(auto& pairProdA_ProdB:vpairProdA_ProdB)
    {
        auto prodA = pairProdA_ProdB.first;
        double dSatisfPerA = mapeProd_dSatisf.at(prodA);
        auto prodB = pairProdA_ProdB.second;
        double dSatisfPerB = mapeProd_dSatisf.at(prodB);

        dSatisfSumOfPairProducts += pStock->GetAmount(prodA)*dSatisfPerA*pStock->GetAmount(prodB)*dSatisfPerB;
    }

    std::cout<<"dSatisfSumOfPairProducts"<<dSatisfSumOfPairProducts<< std::endl;


    dSatisf = dSatisfSum + dSatisfSumOfPairProducts/dSatisfSum;

    std::cout<<"dSatisf"<<dSatisf<< std::endl;

    return dSatisf;

    //Satisf = satisfA + satisfB + satisfC + (satisfA*satisfB + satisfA*satisfC + satisfB*satisfC)/(satisfA + satisfB + satisfC);
    //Satisf = satisfSum + satisfSumOfPairProducts/satisfSum

//    }
}

double CParticipant::GetDiminishingReturnsFactor(double dQuantity)
{
    //Este método es candidato a meterse en una clase aparte
    //Voy a llamar al termino "1-(1/(0.25*x+1)^2)" Diminishing Returns Factor
    double dDenominatorSquareRoot = (0.25*dQuantity+1.0);
    double dDenominator = dDenominatorSquareRoot*dDenominatorSquareRoot;
    double dResult = 1.0 - (1.0/dDenominator);
    return dResult;
}

double CParticipant::GetSatisfactionOfStockWithCategories2(const CStock* pStock) const
{
    return GetSatisfactionOfStockWithCategories2(pStock,m_mapeProd_dSatisf);
}

double CParticipant::GetSatisfactionOfStockWithCategories(const CStock* pStock) const
{
    return GetSatisfactionOfStockWithCategories(pStock,m_mapeProd_dSatisf);
}

double CParticipant::GetSatisfactionOfStockWithCategories2(const CStock* pStock,std::map<def::eProduct, double> mapeProd_dSatisf)
{

    //Mejor: satisf = 1-(1/(0.25*x+1)^2)
    //donde x sería la cantidad. Esta ecuación tendría un máximo en 1, y tendría pendiente 1 en 0
    //Es como una ecuación y = x, pero que se va haciendo más y más horizontal hasta q ya no crece la y

    //Voy a llamar al termino "1-(1/(0.25*x+1)^2)" Diminishing Returns Factor
    //DiminishingReturnsFactor = DimRetFactor

    //QuantSatisfCatA = QuantCteCatA*(1-(1/(0.25*NumProdCatA+1)^2))
    //O sea: QuantSatisfCatA = QuantCteCatA*DimRetFactor
    //QualSatisfCatA = QualCte*((NumProd1CatA*SatisfProd1CatA+NumProd2CatA*SatisfProd2Cata)/NumProdCatA)*(1-(1/(0.25*NumProdCatA+1)^2))
    //Voy a llamar al termino "(NumProd1CatA*SatisfProd1CatA+NumProd2CatA*SatisfProd2Cata)/NumProdCatA" Average Quality Factor
    //Average Quality Factor = AvQualFactor
    //O sea: QualSatisfCatA = QualCte*AvQualFactor*DimRetFactor

    //Quantity satisfaction:
    std::set<double> setQuantSatisfPerCat;
    std::map<def::eCategory, double> mapCateg_dQuantSatisf;
    for(auto & eCat:def::vCategories)
    {
        double dQuantConstOfCat = 100.0; //En el futuro dependerá del Participant y de la Category
        double dAmountOfCat = pStock->GetAmountOfCategory(eCat);
        mapCateg_dQuantSatisf[eCat] = dQuantConstOfCat*GetDiminishingReturnsFactor(dAmountOfCat);
    }

    double dQuantSatisf = 0.0;
    for(auto & eCat:def::vCategories)
    {
        dQuantSatisf += mapCateg_dQuantSatisf.at(eCat);
    }

    //Quality satisfaction:

    //Average Quality Factor = AvQualFactor

    //"(NumProd1CatA*SatisfProd1CatA+NumProd2CatA*SatisfProd2Cata)/NumProdCatA"

    std::map<def::eCategory,double> mapCat_QualSatisf;

    for(auto & eCat:def::vCategories)
    {
        double dSumatoryOfSatisfOfProds = 0.0;
        for (auto & eProd:def::mapCat_setProds.at(eCat))
        {
            double dSatisfOfProd = mapeProd_dSatisf.at(eProd);
            double dAmountOfProd = pStock->GetAmount(eProd);
            double dAmount_x_SatisfOfProd = dAmountOfProd*dSatisfOfProd;
            dSumatoryOfSatisfOfProds += dAmount_x_SatisfOfProd;
        }
        double dAmountOfCat = pStock->GetAmountOfCategory(eCat);
        double dAvQualFactor = dSumatoryOfSatisfOfProds/dAmountOfCat;
        double dDimRetFactor = GetDiminishingReturnsFactor(dAmountOfCat);
        double dQualConst = 100.0;
        double dQualSatisfOfCat = dQualConst*dAvQualFactor*dDimRetFactor;

        mapCat_QualSatisf[eCat] = dQualSatisfOfCat;
    }

    double dQualSatisf = 0.0;
    for(auto & pairCat_QualSatisf:mapCat_QualSatisf)
    {
        dQualSatisf += pairCat_QualSatisf.second;
    }

    //Sum QuantitySatisf + QualSatisf

    double dTotalSatisfaction = dQuantSatisf + dQualSatisf;

    return dTotalSatisfaction;
}

double CParticipant::GetSatisfactionOfStockWithCategories(const CStock* pStock,std::map<def::eProduct, double> mapeProd_dSatisf)
{
    //Primar cantidad sobre calidad hasta conseguir una determinada cantidad de una categoría

    //Con cantidades mayores primará la calidad sobre la cantidad, dentro de la categoría

    //Limite de satisfacción por cantidad. A partir de una cantidad (en una categoría) no puede aumentar la satisfacción

    //Implementación sencilla:
    //(Lo malo de esta implementación es que la satisfacción baja a partir de cierto número de productos, en vez de mantenerse constante)

    //SatisfBecauseQuantity = CatASatisfQuantity + CatBSatisfQuantity;
    //CatASatisfQuantity = QuantCteCatA*NumProdCatA - (NumProdCatA^2)

    //SatisfBecauseQuality = satisfCatA + satisfCatB + satisfCatC + (satisfCatA*satisfCatB + satisfCatA*satisfCatC + satisfCatB*satisfCatC)/(satisfCatA + satisfCatB + satisfCatC);
    //satisfCatA = QualCteCatA(NumProd1CatA*satisfProd1CatA + prod2CatA*satisfProd2CatA) - (NumProdCatA^2)

    //Cambios para que los máximos se mantengan en vez de empezar a bajar:

    //Buscar máximo de CatASatisfQuantity:
    //El máximo creo que está con
    //0 = QuantCteCatA - 2*NumProdCatA
    //NumProdCatA=QuantCteCatA/2

    //Buscar máximo de CatASatisfQuality para prod1 de CatA
    //0 = QualCteCatA*satisfProd1CatA - 2*NumProdCatA
    //NumProdCatA = QualCteCatA*satisfProd1CatA/2


    //Investigar: satisf = 0.5-(1/(1+(x+1)^2))
    //donde x sería la cantidad. Esta ecuación tendría un máximo en el 0.5

    //Mejor: satisf = 1-(1/(0.25*x+1)^2)
    //donde x sería la cantidad. Esta ecuación tendría un máximo en 1, y tendría pendiente 1 en 0
    //Es como una ecuación y = x, pero que se va haciendo más y más horizontal hasta q ya no crece la y

    //Voy a llamar al termino "1-(1/(0.25*x+1)^2)" Diminishing Returns Factor
    //DiminishingReturnsFactor = DimRetFactor

    //QuantSatisfCatA = QuantCteCatA*(1-(1/(0.25*NumProdCatA+1)^2))
    //O sea: QuantSatisfCatA = QuantCteCatA*DimRetFactor
    //QualSatisfCatA = QualCte*((NumProd1CatA*SatisfProd1CatA+NumProd2CatA*SatisfProd2Cata)/NumProdCatA)*(1-(1/(0.25*NumProdCatA+1)^2))
    //Voy a llamar al termino "(NumProd1CatA*SatisfProd1CatA+NumProd2CatA*SatisfProd2Cata)/NumProdCatA" Average Quality Factor
    //Median Quality Factor = AvQualFactor
    //O sea: QualSatisfCatA = QualCte*AvQualFactor*DimRetFactor

    //Todo: Cambiar el cálculo de satisfacción para hacerlo usando las formulas de DimRetFactor y AvQualFactor

    //Amount of products per Category
    std::map<def::eCategory, double> mapCateg_dAmountOfProd;
    for(auto & eCat:def::vCategories)
    {
        double dAmount = 0.0;
        for (auto & eProd:def::mapCat_setProds.at(eCat))
        {
            dAmount += pStock->GetAmount(eProd);
        }
        mapCateg_dAmountOfProd[eCat] = dAmount;
    }

    //amount of prods per cat
    std::cout << "Amount of prods per category:" << std::endl;
    for (auto & pairCatAmount:mapCateg_dAmountOfProd)
        std::cout << def::mapCategoryNames.at(pairCatAmount.first) << ": " << pairCatAmount.second << std::endl;
    //

    //First: Quantity Satisfaction
    double dQuantitySatisfaction = 0.0;
    {
        //Param of quantity satisfaction per category
        std::map<def::eCategory, double> mapCateg_dQuantSatisfCte;
        for(auto & eCat:def::vCategories)
        {
            mapCateg_dQuantSatisfCte[eCat] = 100;
        }

        //Quantity satisfaction calculation per category
        std::map<def::eCategory, double> mapCateg_dQuantSatisf;
        for(auto & eCat:def::vCategories)
        {
            double dCatQuantSatisf = 0.0;
            double dAmountPow2 = mapCateg_dAmountOfProd.at(eCat)*mapCateg_dAmountOfProd.at(eCat);
            dCatQuantSatisf = mapCateg_dQuantSatisfCte.at(eCat)*mapCateg_dAmountOfProd.at(eCat) - dAmountPow2;

            if (dCatQuantSatisf > 0.0)
                mapCateg_dQuantSatisf[eCat] = dCatQuantSatisf;
            else
                mapCateg_dQuantSatisf[eCat] = 0.0;
        }

        //quant satisf per cat
        std::cout << "Quant satisf per category:" << std::endl;
        for (auto & pairCatAmount:mapCateg_dQuantSatisf)
            std::cout << def::mapCategoryNames.at(pairCatAmount.first) << ": " << pairCatAmount.second << std::endl;
        //

        //Quantity satisfaction:
        for (auto & pairCat_QuantSatisf:mapCateg_dQuantSatisf)
        {
            dQuantitySatisfaction += pairCat_QuantSatisf.second;
        }

        //qual satisf per cat
        std::cout << "dQuantitySatisfaction: " << dQuantitySatisfaction << std::endl;

        //
        //
    }


    //Second: Quality Satisfaction
    double dQualitySatisfaction = 0.0;
    {


        //satisfCatA = QualCteCatA(NumProd1CatA*satisfProd1CatA + prod2CatA*satisfProd2CatA) - (NumProdCatA^2)

        //Param of quality satisfaction per category
        std::map<def::eCategory, double> mapCateg_dQualSatisfCte;
        for(auto & eCat:def::vCategories)
        {
            mapCateg_dQualSatisfCte[eCat] = 100;
        }

        //Quality satisfaction per category
        std::map<def::eCategory, double> mapCateg_dQualSatisf;
        for(auto & eCat:def::vCategories)
        {
            double dParamQualCatCte = mapCateg_dQualSatisfCte.at(eCat);
            double dSumProdAmountXSatisfOfProd = 0.0;
            for (auto & eProd:def::mapCat_setProds.at(eCat))
            {
                dSumProdAmountXSatisfOfProd += pStock->GetAmount(eProd)*mapeProd_dSatisf.at(eProd);
            }

            double dCatQualSatisf = 0.0;
            double dAmountPow2 = mapCateg_dAmountOfProd.at(eCat)*mapCateg_dAmountOfProd.at(eCat);

            dCatQualSatisf  = dParamQualCatCte*dSumProdAmountXSatisfOfProd - dAmountPow2;

            if (dCatQualSatisf > 0.0)
                mapCateg_dQualSatisf[eCat] = dCatQualSatisf;
            else
                mapCateg_dQualSatisf[eCat] = 0.0;
        }

        //qual satisf per cat
        std::cout << "Qual satisf per category:" << std::endl;
        for (auto & pairCatAmount:mapCateg_dQualSatisf)
            std::cout << def::mapCategoryNames.at(pairCatAmount.first) << ": " << pairCatAmount.second << std::endl;
        //

        //Quality satisfaction
        //SatisfBecauseQuality = satisfCatA + satisfCatB + satisfCatC + (satisfCatA*satisfCatB + satisfCatA*satisfCatC + satisfCatB*satisfCatC)/(satisfCatA + satisfCatB + satisfCatC);
        //SatisfBecauseQuality = dSumQualitySatisf + (dSumOfPairCatProdsOfQualitySatisf)/dSumQualitySatisf;

        double dSumQualitySaisf=0.0;
        for (auto & pairCat_QualSatisf:mapCateg_dQualSatisf)
        {
            dSumQualitySaisf += pairCat_QualSatisf.second;
        }

        //Esto igual podría estar precalculado en el defines o así:
        std::vector<std::pair<def::eCategory,def::eCategory>> vpairCatA_CatB;
        for (int indexA=0;indexA<def::vCategories.size();indexA++)
        {
            def::eCategory catA=def::vCategories.at(indexA);

            for (int indexB=indexA+1;indexB<def::vCategories.size();indexB++)
            {
                def::eCategory catB=def::vCategories.at(indexB);
                vpairCatA_CatB.push_back(std::make_pair(catA,catB));
            }
        }

        double dSumOfCatASatisf_X_CatBSatisf = 0.0;
        for (auto & pairCatA_CatB:vpairCatA_CatB)
        {
            def::eCategory catA = pairCatA_CatB.first;
            def::eCategory catB = pairCatA_CatB.second;

            dSumOfCatASatisf_X_CatBSatisf += mapCateg_dQualSatisf[catA]*mapCateg_dQualSatisf[catB];
        }

        dQualitySatisfaction = dSumQualitySaisf + dSumOfCatASatisf_X_CatBSatisf/dSumQualitySaisf;

        //qual satisf per cat
        std::cout << "dQualitySatisfaction: " << dQualitySatisfaction << std::endl;



    }


    double dTotalSatisfaction = dQuantitySatisfaction + dQualitySatisfaction;

    return dTotalSatisfaction;
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

CStock CParticipant::CalculateConsumption() const
{
    return CalculateConsumption(&this->m_Stock);
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
