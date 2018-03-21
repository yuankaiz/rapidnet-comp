#ifndef ML_HELPER_H
#define ML_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "ml.h"

namespace ns3 {
namespace rapidnet {
namespace ml {

class Ml;

class MlHelper: public RapidNetApplicationHelper
{
public:
  MlHelper ()
  {
    m_factory.SetTypeId (Ml::GetTypeId ());
  }
  virtual ~MlHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Ml> ();
  }
};

} // namespace ml
} // namespace rapidnet
} // namespace ns3

#endif // ML_HELPER_H

