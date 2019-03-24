#include "Products.h"

//const def::eProduct CProducts::m_arrayOfProdTypes[def::NUM_OF_PROD_TYPES] = {def::PROD_FOOD,def::PROD_CLOTH,def::PROD_GOLD};
const std::vector<def::eProduct> CProducts::m_vOfProdTypes = {def::PROD_GOLD,def::PROD_FOOD,def::PROD_CLOTH};

CProducts::CProducts()
{
    //ctor
}

CProducts::~CProducts()
{
    //dtor
}

std::vector<def::eProduct> CProducts::GetProductTypes()
{
    //std::vector<def::eProduct> vProds(m_arrayOfProdTypes, m_arrayOfProdTypes + def::NUM_OF_PROD_TYPES);
    return m_vOfProdTypes;
}
