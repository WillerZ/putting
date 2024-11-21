#pragma once

#include <gmpxx.h>

namespace phil::stats {

struct Sample2 {
  unsigned long exact;
  unsigned long made;
  unsigned long missed;
  constexpr unsigned long total() const noexcept { return exact + made + missed; }
  constexpr Sample2 operator-(Sample2 other) const noexcept {
    return Sample2{exact - other.exact, made - other.made, missed - other.missed};
  }
};

mpf_class FishersExact2Test1TailIntegration(Sample2 const base, Sample2 const comparand);
} // namespace phil::stats