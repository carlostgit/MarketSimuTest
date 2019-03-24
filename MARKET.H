#ifndef CMARKET_H
#define CMARKET_H

#include <vector>
#include <memory>

//class CLedger;
#include "Ledger.h"
#include "PricesInfo.h"

class CParticipant;
class CPricesInfo;
class CTransactionInfo;

class CMarket
{
public:

    CMarket();
    virtual ~CMarket();
    CMarket(CMarket& otroMarket) = delete;
    CMarket& operator=(CMarket& otroMarket) = delete;

    void Step();
    static void CalculatePrice(std::vector<CParticipant*>& vpParticipants, CMarket* pMarket, CPricesInfo* pPricesInfo);
    //void AdjustPrices(double dGain,CPricesInfo* pOriginalPricesInfo, CPricesInfo* pAdjustedPricesInfo);
    static std::unique_ptr<CTransactionInfo> CalculateTransactionInfoWithParticipant(CMarket* pMarket, CParticipant* pPartic, CPricesInfo* pPricesInfo, double* dPerformanceSeconds=NULL);
    void MakeTransactions();

//    void SetLedger(CLedger* pLedger)
//    {
//        m_pLedger = pLedger;
//    }

    CPricesInfo GetPricesInfo() const {return m_PricesInfo;}
    void SetPricesInfo(CPricesInfo pricesInfo){m_PricesInfo=pricesInfo;}
    void AddParticipant(CParticipant* participant){m_vpParticipants.push_back(participant);}

    long GetId(){return m_nId;}
    long GetStep(){return m_nStep;}

    CLedger* GetLedgerRef(){return &m_Ledger;}

    std::vector<CParticipant*> GetParticipants(){return m_vpParticipants;}


protected:

private:

    CLedger m_Ledger;
    CPricesInfo m_PricesInfo;
    std::vector<CParticipant*> m_vpParticipants;

    long m_nId;
    long m_nStep;

    static long nObectCounter;


};

#endif // CMARKET_H
