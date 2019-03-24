#ifndef CPRICESINFO_H
#define CPRICESINFO_H

#include "Defines.h"
#include "Stock.h"

class CPrices;
//class CStock;

class CPricesInfo
{
public:
    CPricesInfo();
    virtual ~CPricesInfo();

    double GetPrice(def::eProduct eprodCurrency,def::eProduct prod) const;
    double GetPrice(def::eProduct prod) const;
    CPrices GetPrices(def::eProduct eProdCurrency) const;
    void SetStockForPrices(CStock stockForPrices);
    static double GetPriceOfProduct(def::eProduct eProd, def::eProduct eProdCurrency, const CStock* stockForPrices);
    double GetPriceOfProduct(def::eProduct eProd, def::eProduct eProdCurrency) const;

    //CStock* GetStockForPricesRef(){return &m_stockForPrices;}
    CStock GetStockForPrices() const {return m_stockForPrices;}

    CStock GetPriceOfStock(CStock stock, def::eProduct prodCurrency) const;

    //Printing methods
    const void PrintInfo();

protected:
private:

    CStock m_stockForPrices;
};

#endif // CPRICESINFO_H
