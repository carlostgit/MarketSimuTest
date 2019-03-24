#ifndef CPRODUCTS_H
#define CPRODUCTS_H

#include "defines.h"
#include <vector>

class CProducts
{
public:


public:
    CProducts();
    virtual ~CProducts();

    static std::vector<def::eProduct> GetProductTypes();

protected:

private:
    //static const def::eProduct m_arrayOfProdTypes[def::NUM_OF_PROD_TYPES];
    static const std::vector<def::eProduct> m_vOfProdTypes;
    //const int a[] = {1,2,3};
};



#endif // CPRODUCTS_H

