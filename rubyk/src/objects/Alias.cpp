#include "rubyk.h"
#include "oscit/alias.h"

/** This is an object that can be created through /class/Alias/new but it is not a Node. */
class AliasNode : public oscit::Alias
{
public:
  
  const Value original_(const Value& val)
  {
    String url(val);
    if (!url.is_nil()) {
      set_original(url.string());
    }
    return mOriginal ? String(mOriginal->url()) : gNilValue;
  }
};

extern "C" void init(Planet& planet)
{
  Class * c = planet.classes()->declare<AliasNode>("Alias", "Create an alias to a node's method.");
  ACCESSOR(AliasNode, original, "Set original method.")
}