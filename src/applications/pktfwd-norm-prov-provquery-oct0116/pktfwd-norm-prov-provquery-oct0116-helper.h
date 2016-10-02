#ifndef PKTFWDNORMPROVPROVQUERYOCT0116_HELPER_H
#define PKTFWDNORMPROVPROVQUERYOCT0116_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-provquery-oct0116.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovprovqueryoct0116 {

class PktfwdNormProvProvqueryOct0116;

class PktfwdNormProvProvqueryOct0116Helper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvProvqueryOct0116Helper ()
  {
    m_factory.SetTypeId (PktfwdNormProvProvqueryOct0116::GetTypeId ());
  }
  virtual ~PktfwdNormProvProvqueryOct0116Helper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvProvqueryOct0116> ();
  }
};

} // namespace pktfwdnormprovprovqueryoct0116
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVPROVQUERYOCT0116_HELPER_H

