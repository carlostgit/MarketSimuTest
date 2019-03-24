#ifndef CLEDGER_H
#define CLEDGER_H

#include "Stock.h"
#include "PricesInfo.h"
#include <vector>
#include <map>
#include <memory>
class CTransactionInfo;

struct SLedgerInfo
{
    int nMarketId;
    int nMarketStep;
    int nParticipantId;
    CStock ostockSentToMarket;
    CStock ostockReceivedFromMarket;
    CPricesInfo pricesInfo;
};

class CLedger
{
public:
    CLedger();
    virtual ~CLedger();

    void AddTransaction(std::unique_ptr<CTransactionInfo> upTransactionInfo);

    std::vector<SLedgerInfo> GetInfo(){return m_vInfo;}
    std::vector<CPricesInfo*> GetPricesOfEachStep();
    std::map<int,std::vector<CStock>> GetSentToMarketForEachStep();
    std::map<int,std::vector<CStock>> GetReceivedFromMarketEachStep();

    void DrawInfo();

protected:
private:

    std::vector<std::unique_ptr<CTransactionInfo> > m_vupTransactionInfo;
    std::vector<SLedgerInfo> m_vInfo;
};

#endif // CLEDGER_H
