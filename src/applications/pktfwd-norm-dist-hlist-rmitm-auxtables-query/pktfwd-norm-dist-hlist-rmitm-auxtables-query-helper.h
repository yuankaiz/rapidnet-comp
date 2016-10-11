#ifndef PKTFWDNORMDISTHLISTRMITMAUXTABLESQUERY_HELPER_H
#define PKTFWDNORMDISTHLISTRMITMAUXTABLESQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-dist-hlist-rmitm-auxtables-query.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormdisthlistrmitmauxtablesquery {

class PktfwdNormDistHlistRmitmAuxtablesQuery;

class PktfwdNormDistHlistRmitmAuxtablesQueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormDistHlistRmitmAuxtablesQueryHelper ()
  {
    m_factory.SetTypeId (PktfwdNormDistHlistRmitmAuxtablesQuery::GetTypeId ());
  }
  virtual ~PktfwdNormDistHlistRmitmAuxtablesQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormDistHlistRmitmAuxtablesQuery> ();
  }
};

} // namespace pktfwdnormdisthlistrmitmauxtablesquery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDISTHLISTRMITMAUXTABLESQUERY_HELPER_H

