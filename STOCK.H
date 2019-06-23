#ifndef CSTOCK_H
#define CSTOCK_H

#include "Defines.h"

#include <map>
#include <vector>

class CStock
{

public:
    CStock();
    CStock(std::map<def::eProduct,double> mapeProd_dAmount);//{m_mapeProd_dAmount=mapeProd_dAmount;}
    virtual ~CStock();
    CStock(const CStock& otherStock);
    CStock operator-(const CStock& stockOnRight);
    CStock& operator-=(const CStock& stockOnRight);
    CStock operator+(const CStock& stockOnRight);
    CStock& operator+=(const CStock& stockOnRight);
    CStock operator/(const double dDivisor);
    CStock operator*(const double dMultiplier);

    double GetAmount(def::eProduct eProd) const;

    std::map<def::eProduct,double> GetProductAmountMap() const
    {
        return m_mapeProd_dAmount;
    }
    void SetProductAmountMap(std::map<def::eProduct,double> mapeProd_dAmount);
    std::vector<def::eProduct> GetProducts() const;
    double GetMin();
    double GetMax();
    double GetMaxModule();
    double GetSumOfAbs();
    void Set(def::eProduct eprod, double dAmount){m_mapeProd_dAmount[eprod]=dAmount;}
    void AddAmountForEachProd(double dAmount);
    CStock& AddAmount(def::eProduct product, double dAmount);
    void SetAmountForEachProd(double dAmount);
    void SetAmount(def::eProduct product, double dAmount);

    bool IsEnoughToSell(CStock* ostockProdsToOffer);

    std::map<def::eCategory, double> GetAmountPerCategory() const;
    double GetAmountOfCategory(def::eCategory eCat) const;

    //Printing methods
    const void PrintInfo();

protected:
private:

    std::map<def::eProduct,double> m_mapeProd_dAmount;
};

#endif // CSTOCK_H
