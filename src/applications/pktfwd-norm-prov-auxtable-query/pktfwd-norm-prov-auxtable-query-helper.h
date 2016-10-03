#ifndef PKTFWDNORMPROVAUXTABLEQUERY_HELPER_H
#define PKTFWDNORMPROVAUXTABLEQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-auxtable-query.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovauxtablequery {

class PktfwdNormProvAuxtableQuery;

class PktfwdNormProvAuxtableQueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvAuxtableQueryHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProvAuxtableQuery::GetTypeId ());
  }
  virtual ~PktfwdNormProvAuxtableQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvAuxtableQuery> ();
  }
};

} // namespace pktfwdnormprovauxtablequery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVAUXTABLEQUERY_HELPER_H

