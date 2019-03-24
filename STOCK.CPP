#include "Stock.h"

#include <limits>
#include <assert.h>
#include <iostream>
#include <cmath>

CStock::CStock()
{
    //Inicializo un mapa con todos los productos en cantidad 0.0
    for(int i=0;i<def::NUM_OF_PROD_TYPES;i++)
    {
        m_mapeProd_dAmount[static_cast<def::eProduct>(i)]=0.0;
    }
    //ctor
}

CStock::~CStock()
{
    //dtor
}

CStock::CStock(std::map<def::eProduct,double> mapeProd_dAmount)
{
    m_mapeProd_dAmount=mapeProd_dAmount;

    for(int i=0;i<def::NUM_OF_PROD_TYPES;i++)
    {
        if(m_mapeProd_dAmount.find(static_cast<def::eProduct>(i))==m_mapeProd_dAmount.end())
        {
            m_mapeProd_dAmount[static_cast<def::eProduct>(i)]=0.0;
        }
    }
}

CStock::CStock(const CStock& otherStock)
{
    this->SetProductAmountMap(otherStock.GetProductAmountMap());
}

double CStock::GetAmount(def::eProduct eProd) const
{
    if(m_mapeProd_dAmount.end() != m_mapeProd_dAmount.find(eProd))
    {
        return m_mapeProd_dAmount.at(eProd);
    }
    else
    {
        return 0.0;
    }
}

std::vector<def::eProduct> CStock::GetProducts() const
{
    std::vector<def::eProduct> veProds;
    for( auto & paireProd_dAmount:m_mapeProd_dAmount)
    {
        veProds.push_back(paireProd_dAmount.first);
    }
    return veProds;
}

double CStock::GetMin()
{
    if(m_mapeProd_dAmount.empty())
    {
        assert(false=="stock vacio");
        return 0.0;
    }

    double dMin=std::numeric_limits<double>::max();
    std::vector<def::eProduct> veProds;
    for( auto & paireProd_dAmount:m_mapeProd_dAmount)
    {
        if(paireProd_dAmount.second<dMin)
            dMin = paireProd_dAmount.second;
    }
    return dMin;
}

double CStock::GetSumOfAbs()
{
    double dSum=0.0;
    for(auto & paireProd_dAmount:m_mapeProd_dAmount)
    {
        dSum += fabs(paireProd_dAmount.second);
    }
    return dSum;
}

void CStock::SetProductAmountMap(std::map<def::eProduct,double> mapeProd_dAmount)
{
    m_mapeProd_dAmount = mapeProd_dAmount;

    for(int i=0;i<def::NUM_OF_PROD_TYPES;i++)
    {
        if(m_mapeProd_dAmount.find(static_cast<def::eProduct>(i))==m_mapeProd_dAmount.end())
        {
            m_mapeProd_dAmount[static_cast<def::eProduct>(i)]=0.0;
        }
    }
}

CStock CStock::operator-(const CStock& stockOnRight)
{
    std::map<def::eProduct,double> map_thisProdAmountCopy = this->GetProductAmountMap();

    std::map<def::eProduct,double> map_OtherProdAmountCopy = stockOnRight.GetProductAmountMap();

    for(auto & pairOtherProdAm : map_OtherProdAmountCopy)
    {
        map_thisProdAmountCopy[pairOtherProdAm.first]=map_thisProdAmountCopy[pairOtherProdAm.first]-pairOtherProdAm.second;
    }

    CStock returnStock(map_thisProdAmountCopy);

    return returnStock;
}

CStock CStock::operator+(const CStock& stockOnRight)
{
    std::map<def::eProduct,double> map_thisProdAmountCopy = this->GetProductAmountMap();

    std::map<def::eProduct,double> map_OtherProdAmountCopy = stockOnRight.GetProductAmountMap();

    for(auto & pairOtherProdAm : map_OtherProdAmountCopy)
    {
        map_thisProdAmountCopy[pairOtherProdAm.first]=map_thisProdAmountCopy[pairOtherProdAm.first]+pairOtherProdAm.second;
    }

    for(auto & pairThisProdAm : map_thisProdAmountCopy)
    {
        if (map_OtherProdAmountCopy.find(pairThisProdAm.first)==map_OtherProdAmountCopy.end())
        {
            for (auto & debugProdAmount:map_OtherProdAmountCopy)
            {
                std::cout << "debugProdAmount.first" << debugProdAmount.first << "debugProdAmount.second" << debugProdAmount.second << std::endl;
            }
            assert(false=="Este metodo esta sin terminar. Tendria q servir para stocks donde falten productos");
        }
    }

    CStock returnStock(map_thisProdAmountCopy);

    return returnStock;
}
CStock& CStock::operator+=(const CStock& stockOnRight)
{
    std::map<def::eProduct,double> map_thisProdAmountCopy = this->GetProductAmountMap();

    std::map<def::eProduct,double> map_OtherProdAmountCopy = stockOnRight.GetProductAmountMap();

    for(auto & pairOtherProdAm : map_OtherProdAmountCopy)
    {
        map_thisProdAmountCopy[pairOtherProdAm.first]=map_thisProdAmountCopy[pairOtherProdAm.first]+pairOtherProdAm.second;
    }

    this->SetProductAmountMap(map_thisProdAmountCopy);

    return (*this);
}

CStock& CStock::operator-=(const CStock& stockOnRight)
{
    std::map<def::eProduct,double> map_thisProdAmountCopy = this->GetProductAmountMap();

    std::map<def::eProduct,double> map_OtherProdAmountCopy = stockOnRight.GetProductAmountMap();

    for(auto & pairOtherProdAm : map_OtherProdAmountCopy)
    {
        map_thisProdAmountCopy[pairOtherProdAm.first]=map_thisProdAmountCopy[pairOtherProdAm.first]-pairOtherProdAm.second;

        assert(map_thisProdAmountCopy[pairOtherProdAm.first]>=0.0);
    }

    this->SetProductAmountMap(map_thisProdAmountCopy);

    return (*this);
}

double CStock::GetMax()
{
    if(m_mapeProd_dAmount.empty())
    {
        assert(false=="stock vacio");
        return 0.0;
    }

    double dMax=0.0;
    std::vector<def::eProduct> veProds;
    for( auto & paireProd_dAmount:m_mapeProd_dAmount)
    {
        if(paireProd_dAmount.second>dMax)
            dMax = paireProd_dAmount.second;
    }
    return dMax;
}

double CStock::GetMaxModule()
{
    if(m_mapeProd_dAmount.empty())
    {
        assert(false=="stock vacio");
        return 0.0;
    }

    double dMax=0.0;
    std::vector<def::eProduct> veProds;
    for( auto & paireProd_dAmount:m_mapeProd_dAmount)
    {
        double dAbsAmount=std::abs(paireProd_dAmount.second);
        if(dAbsAmount>dMax)
            dMax = dAbsAmount;
    }
    return dMax;
}

CStock CStock::operator/(const double dDivisor)
{
    std::map<def::eProduct,double> map_thisProdAmountCopy = this->GetProductAmountMap();

    std::map<def::eProduct,double> map_returnProdAmount;
    for(auto & pairOtherProdAm : map_thisProdAmountCopy)
    {
        map_returnProdAmount[pairOtherProdAm.first]=pairOtherProdAm.second/dDivisor;
    }

    CStock returnStock(map_returnProdAmount);

    return returnStock;
}

CStock CStock::operator*(const double dMultiplier)
{
    std::map<def::eProduct,double> map_thisProdAmountCopy = this->GetProductAmountMap();

    std::map<def::eProduct,double> map_returnProdAmount;
    for(auto & pairOtherProdAm : map_thisProdAmountCopy)
    {
        map_returnProdAmount[pairOtherProdAm.first]=pairOtherProdAm.second*dMultiplier;
    }

    CStock returnStock(map_returnProdAmount);

    return returnStock;
}

void CStock::AddAmountForEachProd(double dAmount)
{
    for(auto & pairOtherProdAm : m_mapeProd_dAmount)
    {
        pairOtherProdAm.second+=dAmount;
    }
}

void CStock::AddAmount(def::eProduct product, double dAmount)
{
    assert(m_mapeProd_dAmount.end()!=m_mapeProd_dAmount.find(product));
    m_mapeProd_dAmount.at(product)+=dAmount;
}


void CStock::SetAmountForEachProd(double dAmount)
{
    for(auto & pairOtherProdAm : m_mapeProd_dAmount)
    {
        pairOtherProdAm.second = dAmount;
    }
}

void CStock::SetAmount(def::eProduct product, double dAmount)
{
    assert(m_mapeProd_dAmount.end()!=m_mapeProd_dAmount.find(product));
    m_mapeProd_dAmount.at(product)=dAmount;
}

bool CStock::IsEnoughToSell(CStock* ostockProdsToOffer)
{
    CStock stock=(*this)-(*ostockProdsToOffer);
    double dMin = stock.GetMin();
    if(dMin<0.0)
        return false;
    else
        return true;
}

const void CStock::PrintInfo()
{
    std::cout << "Printing Stock info: "<< std::endl;
    //std::map<def::eProduct,double> m_mapeProd_dAmount
    for(auto & pairProdAm : m_mapeProd_dAmount)
    {
        def::eProduct eProd = pairProdAm.first;
        double dAmount = pairProdAm.second;

        auto prodSearch_it = def::mapeProductNames.find(eProd);
        if(def::mapeProductNames.end() != prodSearch_it)
        {
            std::string strProdName = (*prodSearch_it).second;

            std::cout << "eProd: "<< strProdName << std::endl;
            std::cout << "amount: "<< dAmount << std::endl;
        }
    }

    std::cout << std::endl;
}
