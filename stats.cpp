#include "stats.hpp"
#include "cache.hpp"

#include <gmpxx.h>

#include <cassert>

namespace phil::stats {
// Willoughby's Exact Test
static mpf_class WilloughbysExactTest(IntermediateCache &cache, Sample const base, Sample const comparand) {
  auto &&x = cache.getBinomialCoefficient(base.made, comparand.made);
  auto &&y = cache.getBinomialCoefficient(base.missed, comparand.missed);
  auto &&z = cache.getReciprocal(cache.getBinomialCoefficient(base.total(), comparand.total()));
  return x * y * z;
}

// Willoughby's Exact Test, Cumulative part only
static mpf_class WilloughbysExactTestCumulativeOnly(IntermediateCache &cache, Sample const left, Sample const r) {
  Sample right = r;
  if (right.made > right.missed) {
    mpf_class result;
    while (right.made) {
      --right.made;
      ++right.missed;
      assert(r.total() == right.total());
      result += WilloughbysExactTest(cache, left, right);
    }
    return result;
  } else {
    mpf_class result;
    while (right.missed) {
      ++right.made;
      --right.missed;
      assert(r.total() == right.total());
      result += WilloughbysExactTest(cache, left, right);
    }
    return 1 - result;
  }
}

// Willoughby's Exact Test, Cumulative
std::pair<mpf_class, mpf_class> WilloughbysExactTestCumulative(Sample const base, Sample const comparand) {
  IntermediateCache cache;
  auto const result = WilloughbysExactTest(cache, base, comparand);
  return std::make_pair(std::move(result), WilloughbysExactTestCumulativeOnly(cache, base, comparand));
}

} // namespace phil::stats
