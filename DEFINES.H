#ifndef DEFINES_H
#define DEFINES_H

#include <map>
#include <string>
#include <vector>

namespace def
{
    enum eProduct
    {
        PROD_FOOD = 0,
        PROD_CLOTH,
        PROD_GOLD,
        NUM_OF_PROD_TYPES,
        NON_IDENTIFIED_PRODUCT
    };

    //const std::map<long, std::string> mapeProductNames = {{0,"Food"}, {1,"Cloth"}, {2,"Gold"}, {3,"Num of prod types"}, {4,"Non identified prod"}};
    const std::map<eProduct, std::string> mapeProductNames = {{PROD_FOOD,"Food"}, {PROD_CLOTH,"Cloth"}, {PROD_GOLD,"Gold"}, {NUM_OF_PROD_TYPES,"Num of prod types"}, {NON_IDENTIFIED_PRODUCT,"Non identified prod"}};
    const std::vector<eProduct> vProducts = {PROD_CLOTH,PROD_FOOD,PROD_GOLD};
};


#endif // DEFINES_H
