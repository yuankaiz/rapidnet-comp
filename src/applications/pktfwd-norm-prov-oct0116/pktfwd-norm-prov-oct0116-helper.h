#ifndef PKTFWDNORMPROVOCT0116_HELPER_H
#define PKTFWDNORMPROVOCT0116_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-oct0116.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovoct0116 {

class PktfwdNormProvOct0116;

class PktfwdNormProvOct0116Helper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvOct0116Helper ()
  {
    m_factory.SetTypeId (PktfwdNormProvOct0116::GetTypeId ());
  }
  virtual ~PktfwdNormProvOct0116Helper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvOct0116> ();
  }
};

} // namespace pktfwdnormprovoct0116
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVOCT0116_HELPER_H

