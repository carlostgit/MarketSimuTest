#include "Prices.h"

#include <assert.h>

CPrices::CPrices(def::eProduct eProdCurrency)
{
    m_eProdCurrency=eProdCurrency;
    //ctor
}

CPrices::~CPrices()
{
    //dtor
}

double CPrices::GetPrice(def::eProduct eProd) const
//puede que sea mejor cambiar esto para que siempre devuelva un parPrecio_Moneda pair<double,eProduct>
{
    if (m_mapeProd_Price.end() != m_mapeProd_Price.find(eProd))
    {
        return m_mapeProd_Price.at(eProd);
    }
    else
    {
        assert(false=="Not price for this product");
        return 0.0;
    }
}

void CPrices::SetPrice(def::eProduct eProd, double dPrice)
{
    m_mapeProd_Price[eProd] = dPrice;
}

