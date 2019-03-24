#ifndef CPRICES_H
#define CPRICES_H

#include <map>

#include "Defines.h"

class CPrices
{
public:
    CPrices(def::eProduct eProdCurrency);
    virtual ~CPrices();

    //puede que sea mejor cambiar esto para que siempre devuelva un parPrecio_Moneda pair<double,eProduct>
    double GetPrice(def::eProduct eProd) const;

    void SetPrice(def::eProduct eProd, double dPrice);

protected:
private:

    def::eProduct m_eProdCurrency;
    std::map<def::eProduct,double> m_mapeProd_Price;
};

#endif // CPRICES_H
