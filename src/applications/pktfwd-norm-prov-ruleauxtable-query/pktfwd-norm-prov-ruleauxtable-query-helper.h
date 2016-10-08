#ifndef PKTFWDNORMPROVRULEAUXTABLEQUERY_HELPER_H
#define PKTFWDNORMPROVRULEAUXTABLEQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-ruleauxtable-query.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovruleauxtablequery {

class PktfwdNormProvRuleauxtableQuery;

class PktfwdNormProvRuleauxtableQueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvRuleauxtableQueryHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProvRuleauxtableQuery::GetTypeId ());
  }
  virtual ~PktfwdNormProvRuleauxtableQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvRuleauxtableQuery> ();
  }
};

} // namespace pktfwdnormprovruleauxtablequery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVRULEAUXTABLEQUERY_HELPER_H

