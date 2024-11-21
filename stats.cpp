#include "stats.hpp"
#include "cache.hpp"

#include <gmpxx.h>

#include <cassert>

namespace phil::stats {
static mpf_class FishersExactTest(IntermediateCache &cache, unsigned long a, unsigned long b, unsigned long c,
                                  unsigned long d) {
  auto &&x = cache.getBinomialCoefficient(a + c, a);
  auto &&y = cache.getBinomialCoefficient(b + d, b);
  auto &&z = cache.getReciprocal(cache.getBinomialCoefficient(a + b + c + d, a + b));
  return x * y * z;
}

static mpf_class FishersExactTest1TailIntegrationX(IntermediateCache &cache, Sample const l, Sample const r) {
  Sample left = l;
  Sample right = r;
  mpf_class result;
  while (left.made && right.missed) {
    --left.made;
    ++left.missed;
    ++right.made;
    --right.missed;
    assert(l.total() == left.total());
    assert(r.total() == right.total());
    assert(l.made + r.made == left.made + right.made);
    assert(l.missed + r.missed == left.missed + right.missed);
    result += FishersExactTest(cache, left.made, left.missed, right.made, right.missed);
  }
  return result;
}

// Fisher's exact test, 1-tail integration
mpf_class FishersExactTest1TailIntegration(Sample const base, Sample const comparand) {
  IntermediateCache cache;
  auto const result = FishersExactTest(cache, base.made, base.missed, comparand.made, comparand.missed);
  if ((1.0 * comparand.made) / comparand.total() >= (1.0 * base.made) / base.total()) {
    return result + FishersExactTest1TailIntegrationX(cache, base, comparand);
  } else {
    return result + FishersExactTest1TailIntegrationX(cache, comparand, base);
  }
}

// Fisher's exact test, 2-tail integration
mpf_class FishersExactTest2TailIntegration(Sample const base, Sample const comparand) {
  IntermediateCache cache;
  auto const result = FishersExactTest(cache, base.made, base.missed, comparand.made, comparand.missed);
  if ((1.0 * comparand.made) / comparand.total() >= (1.0 * base.made) / base.total()) {
    return result + 2 * FishersExactTest1TailIntegrationX(cache, base, comparand);
  } else {
    return result + 2 * FishersExactTest1TailIntegrationX(cache, comparand, base);
  }
}

} // namespace phil::stats
