#ifndef DEFINES_H
#define DEFINES_H

#include <map>
#include <string>
#include <vector>
#include <set>

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

    //Prueba Categories
    enum eCategory
    {
        CAT_FOOD = 0,
        CAT_CLOTH,
        CAT_GOLD,
        NUM_OF_CAT_TYPES,
        NON_IDENTIFIED_CATEGORY
    };

    const std::vector<eCategory> vCategories = {CAT_CLOTH,CAT_FOOD,CAT_GOLD};

    const std::map<eCategory, std::set<eProduct>> mapCat_setProds = {{CAT_FOOD,{PROD_FOOD}},
                                                                    {CAT_CLOTH,{PROD_CLOTH}},
                                                                     {CAT_GOLD,{PROD_GOLD}},
                                                                     };

    const std::map<eProduct, eCategory> mapProduct_Cat = {{PROD_FOOD, CAT_FOOD},
                                                        {PROD_CLOTH, CAT_CLOTH},
                                                        {PROD_GOLD,CAT_GOLD}
    };

    const std::map<eCategory, std::string> mapCategoryNames = {{CAT_FOOD,"CatFood"}, {CAT_CLOTH,"CatCloth"}, {CAT_GOLD,"CatGold"}, {NUM_OF_CAT_TYPES,"Num of cat types"}, {NON_IDENTIFIED_CATEGORY,"Non identified cat"}};
    //

};


#endif // DEFINES_H
