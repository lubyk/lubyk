#ifndef RUBYK_SRC_CORE_MAPPER_H_
#define RUBYK_SRC_CORE_MAPPER_H_

#include "oscit/scale_real.h"
#include "oscit/thash.h"

namespace oscit {

/** The Mapper class helps transform some input data
 * into something else based on strings for mapping and
 * on Real numbers for scaling.
 */
class Mapper {
public:
  Mapper();
  
  Mapper(size_t hash_table_size);
  
  virtual ~Mapper();
  
  bool parse(const std::string &definitions);
  
  const std::string &error() {
    return error_;
  }
  
  /** Find a mapping for the given source.
   * If the mapping is found, scale value.
   * @return false if no mapping is found.
   */
  bool map(const std::string &source, Real value, std::string *target, Real *target_value);
  
  /** Find a reverse mapping for the given target (find source from target).
   * If the mapping is found, reverse scale value.
   * @return false if no mapping is found.
   */
  bool reverse_map(const std::string &source, Real value, std::string *target, Real *target_value);
  
private:
  struct MapElement : public ScaleReal {

    MapElement(const std::string &target_url, Real source_min, Real source_max, Real target_min, Real target_max) :
                ScaleReal(source_min, source_max, target_min, target_max), target_url_(target_url) {}

    const std::string &target_url() const {
      return target_url_;
    }

  private:
    std::string target_url_;
  };
  
  void clear();
  
  bool set_map(const std::string &source_url, Real source_min, Real source_max,
               const std::string &target_url, Real target_min, Real target_max);
  
  THash<std::string, MapElement> map_;
  THash<std::string, MapElement> reverse_map_;
  std::string error_;
};

}  // oscit

#endif // RUBYK_SRC_CORE_MAPPER_H_