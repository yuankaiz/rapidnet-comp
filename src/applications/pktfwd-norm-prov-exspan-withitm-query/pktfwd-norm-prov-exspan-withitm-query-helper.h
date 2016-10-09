#ifndef PKTFWDNORMPROVEXSPANWITHITMQUERY_HELPER_H
#define PKTFWDNORMPROVEXSPANWITHITMQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-exspan-withitm-query.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovexspanwithitmquery {

class PktfwdNormProvExspanWithitmQuery;

class PktfwdNormProvExspanWithitmQueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvExspanWithitmQueryHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProvExspanWithitmQuery::GetTypeId ());
  }
  virtual ~PktfwdNormProvExspanWithitmQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvExspanWithitmQuery> ();
  }
};

} // namespace pktfwdnormprovexspanwithitmquery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVEXSPANWITHITMQUERY_HELPER_H

