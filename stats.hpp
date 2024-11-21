#pragma once

#include <gmpxx.h>

namespace phil::stats {

struct Sample {
  unsigned long made;
  unsigned long missed;
  constexpr unsigned long total() const noexcept { return made + missed; }
  constexpr Sample operator-(Sample other) const noexcept { return Sample{made - other.made, missed - other.missed}; }
};

mpf_class FishersExactTest1TailIntegration(Sample const base, Sample const comparand);
mpf_class FishersExactTest2TailIntegration(Sample const base, Sample const comparand);
} // namespace phil::stats