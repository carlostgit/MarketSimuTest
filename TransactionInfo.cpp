#include "TransactionInfo.h"

CTransactionInfo::CTransactionInfo(CMarket* pMarket,
                                   CParticipant* pParticipant, //No sé si es bueno que sea un puntero
                                   CStock ostockSentToMarket, //esto es una copia, no un puntero
                                   CStock ostockReceivedFromMarket  //esto es una copia, no un puntero
                                  )
{
    m_pMarket = pMarket;
    m_pParticipant = pParticipant;
    m_ostockSentToMarket = ostockSentToMarket;
    m_ostockReceivedFromMarket = ostockReceivedFromMarket;
}

CTransactionInfo::~CTransactionInfo()
{
    //dtor
}

CStock CTransactionInfo::GetReceivedFromMarket()
{
    return m_ostockReceivedFromMarket;
}

CStock CTransactionInfo::GetSentToMarket()
{
    return m_ostockSentToMarket;
}

