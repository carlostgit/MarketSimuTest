#include "PricesInfo.h"

#include <limits>
#include <iostream>

#include "Prices.h"


CPricesInfo::CPricesInfo()
{
    //ctor
    m_stockForPrices.SetAmountForEachProd(1.0);

}

CPricesInfo::~CPricesInfo()
{
    //dtor
}

CPrices CPricesInfo::GetPrices(def::eProduct eProdCurrency) const
{
    CPrices objPrices(eProdCurrency);
    for (auto & pairProdType_Amount : m_stockForPrices.GetProductAmountMap())
    {
        def::eProduct eProdType = pairProdType_Amount.first;
        //double dProdAmount = pairProdType_Amount.second;
        double dPrice = GetPriceOfProduct(eProdType,eProdCurrency);
        objPrices.SetPrice(eProdType,dPrice);
    }

    return objPrices;
}

double CPricesInfo::GetPrice(def::eProduct eprodCurrency,def::eProduct prod) const
{
    double dPrice = this->GetPrices(eprodCurrency).GetPrice(prod);

    return dPrice;
}

double CPricesInfo::GetPrice(def::eProduct prod) const
{
    double dPrice = this->GetPrices(def::PROD_GOLD).GetPrice(prod);

    return dPrice;
}

void CPricesInfo::SetStockForPrices(CStock stockForPrices)
{
    m_stockForPrices=stockForPrices;
}

double CPricesInfo::GetPriceOfProduct(def::eProduct eProd, def::eProduct eProdCurrency, const CStock* pstockForPrices)
{
    if (pstockForPrices->GetAmount(eProd) <=0)
    {
        return std::numeric_limits<double>::max();;
    }
    else
    {
        return pstockForPrices->GetAmount(eProdCurrency)/pstockForPrices->GetAmount(eProd);
    }
}

double CPricesInfo::GetPriceOfProduct(def::eProduct eProd, def::eProduct eProdCurrency) const
{
    return CPricesInfo::GetPriceOfProduct(eProd,eProdCurrency,&m_stockForPrices);
}

CStock CPricesInfo::GetPriceOfStock(CStock stock, def::eProduct prodCurrency) const
{
    std::map<def::eProduct, double> mapReturnPriceProd_Amount;
    mapReturnPriceProd_Amount[prodCurrency] = 0.0;
    for (auto & pairProduct_Amount : stock.GetProductAmountMap())
    {
        def::eProduct eProd = pairProduct_Amount.first;
        double dAmount = pairProduct_Amount.second;

        double dPriceOfProd = this->GetPriceOfProduct(eProd,prodCurrency);
        mapReturnPriceProd_Amount[prodCurrency] +=  dPriceOfProd*dAmount;
    }

    CStock returnStock(mapReturnPriceProd_Amount);
    return returnStock;
}

const void CPricesInfo::PrintInfo()
{
    std::cout << "Printing PricesInfo info: "<< std::endl;
    //std::map<def::eProduct,double> m_mapeProd_dAmount
    std::cout << "StockForPrices: "<< std::endl;
    m_stockForPrices.PrintInfo();
    std::cout << "Prices in Gold: "<< std::endl;
    for(auto & prod : m_stockForPrices.GetProducts())
    {
        if(def::mapeProductNames.find(prod)!=def::mapeProductNames.end())
        {

            std::cout<< "Price of " << (*def::mapeProductNames.find(prod)).second <<": "<<GetPriceOfProduct(prod,def::PROD_GOLD)<< std::endl;
        }

    }
}
