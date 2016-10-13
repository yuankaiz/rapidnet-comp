#ifndef PKTFWDNORMONLINEEXACTMATCHAUXTABLESQUERY_HELPER_H
#define PKTFWDNORMONLINEEXACTMATCHAUXTABLESQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-online-exactmatch-auxtables-query.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormonlineexactmatchauxtablesquery {

class PktfwdNormOnlineExactmatchAuxtablesQuery;

class PktfwdNormOnlineExactmatchAuxtablesQueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormOnlineExactmatchAuxtablesQueryHelper ()
  {
    m_factory.SetTypeId (PktfwdNormOnlineExactmatchAuxtablesQuery::GetTypeId ());
  }
  virtual ~PktfwdNormOnlineExactmatchAuxtablesQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormOnlineExactmatchAuxtablesQuery> ();
  }
};

} // namespace pktfwdnormonlineexactmatchauxtablesquery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMONLINEEXACTMATCHAUXTABLESQUERY_HELPER_H

