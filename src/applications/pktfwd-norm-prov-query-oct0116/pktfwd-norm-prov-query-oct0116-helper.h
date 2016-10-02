#ifndef PKTFWDNORMPROVQUERYOCT0116_HELPER_H
#define PKTFWDNORMPROVQUERYOCT0116_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-query-oct0116.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovqueryoct0116 {

class PktfwdNormProvQueryOct0116;

class PktfwdNormProvQueryOct0116Helper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvQueryOct0116Helper ()
  {
    m_factory.SetTypeId (PktfwdNormProvQueryOct0116::GetTypeId ());
  }
  virtual ~PktfwdNormProvQueryOct0116Helper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvQueryOct0116> ();
  }
};

} // namespace pktfwdnormprovqueryoct0116
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVQUERYOCT0116_HELPER_H

