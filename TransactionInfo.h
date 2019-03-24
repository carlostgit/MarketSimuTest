#ifndef CTRANSACTIONINFO_H
#define CTRANSACTIONINFO_H

#include "Stock.h"

class CMarket;
class CParticipant;


class CTransactionInfo
{
public:
    CTransactionInfo(CMarket* pMarket,
                     CParticipant* pParticipant, //No sé si es bueno que sea un puntero
                     CStock ostockSentToMarket, //esto es una copia, no un puntero
                     CStock ostockReceivedFromMarket  //esto es una copia, no un puntero
                    );


    virtual ~CTransactionInfo();

    CStock GetReceivedFromMarket();
    CStock GetSentToMarket();
    CMarket* GetMarketRef(){return m_pMarket;}
    CParticipant* GetParticipantRef(){return m_pParticipant;}

protected:
private:

    CMarket* m_pMarket;
    CParticipant* m_pParticipant;
    CStock m_ostockSentToMarket;
    CStock m_ostockReceivedFromMarket;
};

#endif // CTRANSACTIONINFO_H
