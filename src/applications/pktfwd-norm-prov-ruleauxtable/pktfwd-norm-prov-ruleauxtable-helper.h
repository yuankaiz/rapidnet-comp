#ifndef PKTFWDNORMPROVRULEAUXTABLE_HELPER_H
#define PKTFWDNORMPROVRULEAUXTABLE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-ruleauxtable.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovruleauxtable {

class PktfwdNormProvRuleauxtable;

class PktfwdNormProvRuleauxtableHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvRuleauxtableHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProvRuleauxtable::GetTypeId ());
  }
  virtual ~PktfwdNormProvRuleauxtableHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvRuleauxtable> ();
  }
};

} // namespace pktfwdnormprovruleauxtable
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVRULEAUXTABLE_HELPER_H

