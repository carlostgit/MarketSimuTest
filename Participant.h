#ifndef CPARTICIPANT_H
#define CPARTICIPANT_H

#include "Defines.h"

#include "Stock.h"

#include <map>
#include <assert.h>


class CPricesInfo;
//class CStock;

class CParticipant
{
public:
    CParticipant();
    virtual ~CParticipant();

    CParticipant(CParticipant& otherPartic)= delete;
    CParticipant& operator=(CParticipant& otherPartic) = delete;


    CStock GetStock(){return m_Stock;}
    CStock* GetStockRef(){return &m_Stock;}

    //Changing stock
    void AddProductsForNextCycle(CStock stock);
    void RemoveProductsForNextCycle(CStock stock);

    void SetStock(CStock stock){m_Stock=stock;}


    //Satisfaction:
    double GetSatisfactionOfProduct(def::eProduct eprod) const;
    //TODO el método deberíaa tener un CStock te argumento en el futuro, porque la satisfacción de un producto dependerá de los que ya se tienen
    double GetSatisfactionOfStock(const CStock* pStock) const;

    void SetProductSatisfactionMap(std::map<def::eProduct, double> mapeProd_dSatisf)
    {
        assert(m_mapeProd_dSatisf.size()==mapeProd_dSatisf.size());
        assert(mapeProd_dSatisf.size()>0);

        m_mapeProd_dSatisf = mapeProd_dSatisf;
    }

    //Transaction:
    CStock OfferProducts(const CPricesInfo* pricinfoPrices, const CStock* pStockIniOfPartic, def::eProduct* pProdSaleCurrency) const;
    CStock DemandProducts(const CPricesInfo* pricinfoPrices, const CStock* pStockAfterSaleOfPartic, def::eProduct* pProdPurchaseCurrency) const;
    CStock DemandProducts2(const CPricesInfo* pricinfoPrices, const CStock* pStockAfterSaleOfPartic, def::eProduct* pProdPurchaseCurrency) const;

    //
    void StepAfterTransaction(const CPricesInfo* pricinfoPrices);

    //Production:
    CStock CalculateProduction(const CPricesInfo* pricinfoPrices) const;
    double GetPrCapacityOfProduct(def::eProduct eprod) const;
    void SetProductPrCapacityMap(std::map<def::eProduct, double> mapeProd_dPrCapacity)
    {
        assert(m_mapeProd_dPrCapacity.size()==mapeProd_dPrCapacity.size());
        assert(mapeProd_dPrCapacity.size()>0);

        m_mapeProd_dPrCapacity = mapeProd_dPrCapacity;
    }

    void Produce(const CPricesInfo* pricinfoPrices);

    //Consumption
    CStock CalculateConsumption(const CStock* ownedStock) const;
    CStock CalculateConsumption() const;
    double GetConsumptionOfProduct(def::eProduct eprod) const;
    void SetProductConsumptionMap(std::map<def::eProduct, double> mapeProd_dConsumption)
    {
        assert(m_mapeProd_dConsumption.size()==mapeProd_dConsumption.size());
        assert(mapeProd_dConsumption.size()>0);

        m_mapeProd_dConsumption = mapeProd_dConsumption;
    }

    void Consume();

    long GetId(){return m_nId;}

    //Logs
    void DrawInfo();

    std::map<long,CStock> GetStockIniMap(){return m_mapLogStockIni;} //Inicial y final?
    std::map<long,CStock> GetStockFinMap(){return m_mapLogStockFin;} //Inicial y final?
    std::map<long,CStock> GetLogConsume(){return m_mapLogConsume;}
    std::map<long,CStock> GetLogProduction(){return m_mapLogProduction;}
    std::map<long,CStock> GetLogSatisfaction(){return m_mapLogSatisfaction;} //TODO


protected:
private:

    std::map<def::eProduct, double> m_mapeProd_dSatisf;
    CStock m_Stock;

    std::map<def::eProduct, double> m_mapeProd_dPrCapacity; //Production capacity

    std::map<def::eProduct, double> m_mapeProd_dConsumption;

    long m_nId;
    static long nObjectCounter;

    long m_nStep;

    //Logs
    std::map<long,CStock> m_mapLogStockIni; //Inicial y final?
    std::map<long,CStock> m_mapLogStockFin; //Inicial y final?
    std::map<long,CStock> m_mapLogConsume;
    std::map<long,CStock> m_mapLogProduction;
    std::map<long,CStock> m_mapLogSatisfaction; //TODO


};


#endif // CPARTICIPANT_H
