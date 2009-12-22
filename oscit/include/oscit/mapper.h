/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef OSCIT_INCLUDE_OSCIT_MAPPER_H_
#define OSCIT_INCLUDE_OSCIT_MAPPER_H_

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

#endif // OSCIT_INCLUDE_OSCIT_MAPPER_H_