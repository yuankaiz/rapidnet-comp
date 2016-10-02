#ifndef PKTFWDNORMPROVAUXTABLEOCT0116_HELPER_H
#define PKTFWDNORMPROVAUXTABLEOCT0116_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-auxtable-oct0116.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovauxtableoct0116 {

class PktfwdNormProvAuxtableOct0116;

class PktfwdNormProvAuxtableOct0116Helper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvAuxtableOct0116Helper ()
  {
    m_factory.SetTypeId (PktfwdNormProvAuxtableOct0116::GetTypeId ());
  }
  virtual ~PktfwdNormProvAuxtableOct0116Helper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvAuxtableOct0116> ();
  }
};

} // namespace pktfwdnormprovauxtableoct0116
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVAUXTABLEOCT0116_HELPER_H

