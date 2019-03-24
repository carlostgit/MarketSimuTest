#include "Ledger.h"

#include "TransactionInfo.h"

#include "Market.h"
#include "Participant.h"
#include "Tester.h"

CLedger::CLedger()
{
    //ctor
}

CLedger::~CLedger()
{
    //dtor
}

void CLedger::AddTransaction(std::unique_ptr<CTransactionInfo> upTransactionInfo)
{
    m_vupTransactionInfo.push_back(std::move(upTransactionInfo));

    SLedgerInfo ledgerInfo;

    CMarket* pMarket=nullptr;
    pMarket = m_vupTransactionInfo.back().get()->GetMarketRef();

    ledgerInfo.nMarketId = pMarket->GetId();
    ledgerInfo.nParticipantId = m_vupTransactionInfo.back().get()->GetParticipantRef()->GetId();
    ledgerInfo.nMarketStep = pMarket->GetStep();
    ledgerInfo.ostockSentToMarket = m_vupTransactionInfo.back().get()->GetSentToMarket();
    ledgerInfo.ostockReceivedFromMarket = m_vupTransactionInfo.back().get()->GetReceivedFromMarket();
    ledgerInfo.pricesInfo = pMarket->GetPricesInfo();

    this->m_vInfo.push_back(ledgerInfo);

}


void CLedger::DrawInfo()
{
    std::map<std::string, std::vector<double> > mapStrVDouble;
    for(auto & ledgInfo:m_vInfo)
    {
        for(auto & prod:def::vProducts)
        {
            std::string str = def::mapeProductNames.at(prod);

            //Amount Sent
            std::string strSent= str+" Sent";
            double dAmountSent = ledgInfo.ostockSentToMarket.GetAmount(prod);
            if(mapStrVDouble.end()==mapStrVDouble.find(strSent))
            {
                std::vector<double> vValues;
                vValues.push_back(dAmountSent);
                mapStrVDouble.insert(std::make_pair(strSent,vValues));
            }
            else
            {
                mapStrVDouble.at(strSent).push_back(dAmountSent);
            }

            //Amount Received
            std::string strReceived= str+" Received";
            double dAmountReceived = ledgInfo.ostockReceivedFromMarket.GetAmount(prod);
            if(mapStrVDouble.end()==mapStrVDouble.find(strReceived))
            {
                std::vector<double> vValues;
                vValues.push_back(dAmountReceived);
                mapStrVDouble.insert(std::make_pair(strReceived,vValues));
            }
            else
            {
                mapStrVDouble.at(strReceived).push_back(dAmountSent);
            }

            //Price
            std::string strPrice= str+" Price";
            double dAmountPrice = ledgInfo.pricesInfo.GetPrice(prod); // .GetAmount(prod);
            if(mapStrVDouble.end()==mapStrVDouble.find(strPrice))
            {
                std::vector<double> vValues;
                vValues.push_back(dAmountPrice);
                mapStrVDouble.insert(std::make_pair(strPrice,vValues));
            }
            else
            {
                mapStrVDouble.at(strPrice).push_back(dAmountPrice);
            }
        }

        //Amount


    }


//    mapStrVDouble.insert(std::make_pair("Cloth",vClothValues));
//    mapStrVDouble.insert(std::make_pair("Gain",vGain));

//    for (auto & val:vFoodValues)
//        std::cout<<val<<"\n";
//    std::cout<< std::endl;

    Test::DrawGraphics(&mapStrVDouble,1.0);
}

std::vector<CPricesInfo*> CLedger::GetPricesOfEachStep()
{
    std::vector<CPricesInfo*> vPricesInfo;


//
//    int nMarketId;
//    int nMarketStep;
//    int nParticipantId;
//    CStock ostockSentToMarket;
//    CStock ostockReceivedFromMarket;
//    CPricesInfo pricesInfo;
    //std::vector<SLedgerInfo> vSLedgerInfo = GetInfo();

    int nStep=0;
    for(auto & ledgInfo:m_vInfo)
    {
        if(ledgInfo.nMarketStep != nStep)
        {
            nStep=ledgInfo.nMarketStep;
            vPricesInfo.push_back(&ledgInfo.pricesInfo);
        }
    }
    return vPricesInfo;
}

std::map<int,std::vector<CStock>> CLedger::GetSentToMarketForEachStep()
{
    std::map<int,std::vector<CStock>> mapIdPart_vStockSent;

    for(auto & ledgInfo:m_vInfo)
    {
        mapIdPart_vStockSent[ledgInfo.nParticipantId].push_back(ledgInfo.ostockSentToMarket);
    }

    return mapIdPart_vStockSent;
}


std::map<int,std::vector<CStock>> CLedger::GetReceivedFromMarketEachStep()
{
    std::map<int,std::vector<CStock>> mapIdPart_vStockRec;

    for(auto & ledgInfo:m_vInfo)
    {
        mapIdPart_vStockRec[ledgInfo.nParticipantId].push_back(ledgInfo.ostockReceivedFromMarket);
    }

    return mapIdPart_vStockRec;
}
    //std::map<int,std::vector<CStock>> GetRecivedFromMarketEachStep();


