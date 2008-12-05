#include "root.h"
#include "class_listing.h"

/** Return the class listing Object (create one if needed). */
ClassListing * Root::classes()
{
  Object * obj = find(CLASS_ROOT);
  if (!obj) {
    // create ClassListing
    obj = adopt(new ClassListing(std::string(CLASS_ROOT).substr(1, std::string::npos), DEFAULT_OBJECTS_LIB_PATH));
  }
  return (ClassListing*)obj; // FIXME: type checking !!
}