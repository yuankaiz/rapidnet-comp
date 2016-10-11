#ifndef PKTFWDNORMDISTHLISTONLINEAUXTABLESQUERY_HELPER_H
#define PKTFWDNORMDISTHLISTONLINEAUXTABLESQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-dist-hlist-online-auxtables-query.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormdisthlistonlineauxtablesquery {

class PktfwdNormDistHlistOnlineAuxtablesQuery;

class PktfwdNormDistHlistOnlineAuxtablesQueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormDistHlistOnlineAuxtablesQueryHelper ()
  {
    m_factory.SetTypeId (PktfwdNormDistHlistOnlineAuxtablesQuery::GetTypeId ());
  }
  virtual ~PktfwdNormDistHlistOnlineAuxtablesQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormDistHlistOnlineAuxtablesQuery> ();
  }
};

} // namespace pktfwdnormdisthlistonlineauxtablesquery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDISTHLISTONLINEAUXTABLESQUERY_HELPER_H

