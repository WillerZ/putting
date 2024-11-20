#include "stats.hpp"

#include <gmpxx.h>

#include <cassert>

namespace phil::stats {
mpq_class FishersExactTest(unsigned long a, unsigned long b, unsigned long c,
                           unsigned long d) {
  mpz_class x, y, z;
  mpz_bin_uiui(x.get_mpz_t(), a + c, a);
  mpz_bin_uiui(y.get_mpz_t(), b + d, b);
  mpz_bin_uiui(z.get_mpz_t(), a + b + c + d, a + b);
  return mpq_class{x * y} / z;
}

static mpq_class FishersExactTest1TailIntegrationX(Sample const l,
                                                   Sample const r) {
  Sample left = l;
  Sample right = r;
  mpq_class result = FishersExactTest(left.made, left.missed, right.made, right.missed);
  while (left.made && right.missed) {
    --left.made;
    ++left.missed;
    ++right.made;
    --right.missed;
    assert(l.total() == left.total());
    assert(r.total() == right.total());
    assert(l.made + r.made == left.made + right.made);
    assert(l.missed + r.missed == left.missed + right.missed);
    result +=
        FishersExactTest(left.made, left.missed, right.made, right.missed);
  }
  return result;
}

// Fisher's exact test, 1-tail integration
mpq_class FishersExactTest1TailIntegration(Sample const base,
                                           Sample const comparand) {
  if ((1.0 * comparand.made) / comparand.total() >=
      (1.0 * base.made) / base.total()) {
    return FishersExactTest1TailIntegrationX(base, comparand);
  } else {
    return FishersExactTest1TailIntegrationX(comparand, base);
  }
}

static mpq_class FishersExactTest2TailIntegrationX(Sample const l,
                                                   Sample const r,
                                                   mpq_class const& level) {
  Sample left = l;
  Sample right = r;
  mpq_class result;
  while (left.made && right.missed) {
    --left.made;
    ++left.missed;
    ++right.made;
    --right.missed;
    assert(l.total() == left.total());
    assert(r.total() == right.total());
    assert(l.made + r.made == left.made + right.made);
    assert(l.missed + r.missed == left.missed + right.missed);
    auto x = FishersExactTest(left.made, left.missed, right.made, right.missed);
    if (x < level) {
      result += x;
    }
  }
  return result;
}

// Fisher's exact test, 2-tail integration
mpq_class FishersExactTest2TailIntegration(Sample const base,
                                           Sample const comparand) {
  mpq_class result = FishersExactTest(base.made, base.missed, comparand.made,
                                      comparand.missed);
  auto const tail1 = FishersExactTest2TailIntegrationX(base, comparand, result);
  auto const tail2 = FishersExactTest2TailIntegrationX(comparand, base, result);
  return result + tail1 + tail2;
}

}  // namespace phil::stats
