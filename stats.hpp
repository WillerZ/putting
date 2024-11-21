#pragma once

#include <functional>
#include <gmpxx.h>

namespace phil::stats {

struct Sample {
  unsigned long made;
  unsigned long missed;
  constexpr unsigned long total() const noexcept { return made + missed; }
  constexpr Sample operator-(Sample other) const noexcept { return Sample{made - other.made, missed - other.missed}; }
};

// Willoughby's Exact Test, Cumulative. Result is {exact part, cumulative part}
std::pair<mpf_class, mpf_class> WilloughbysExactTestCumulative(Sample const base, Sample const comparand);
} // namespace phil::stats