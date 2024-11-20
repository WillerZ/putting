#include "stats2.hpp"

#include <gmpxx.h>

#include <cassert>

namespace phil::stats {
mpq_class FishersExact2Test(Sample2 const one, Sample2 const other) {
  mpz_class w, x, y, z;
  mpz_bin_uiui(w.get_mpz_t(), one.exact + other.exact, one.exact);
  mpz_bin_uiui(x.get_mpz_t(), one.made + other.made, one.made);
  mpz_bin_uiui(y.get_mpz_t(), one.missed + other.missed, one.missed);
  mpz_bin_uiui(z.get_mpz_t(), one.total() + other.total(), one.total());
  return mpq_class{w * x * y} / z;
}

// tail case missed -> made
static mpq_class FishersExact2Test1TailIntegrationX(Sample2 const l,
                                                    Sample2 const r) {
  Sample2 left = l;
  Sample2 right = r;
  mpq_class result;
  while (left.made && right.missed) {
    --left.made;
    ++left.missed;
    ++right.made;
    --right.missed;
    assert(l.total() == left.total());
    assert(r.total() == right.total());
    assert(l.exact + r.exact == left.exact + right.exact);
    assert(l.made + r.made == left.made + right.made);
    assert(l.missed + r.missed == left.missed + right.missed);
    result += FishersExact2Test(left, right);
  }
  return result;
}

// tail case missed -> made -> exact
static mpq_class FishersExact2Test1TailIntegrationY(Sample2 const l,
                                                    Sample2 const r) {
  Sample2 left = l;
  Sample2 right = r;
  // Every better made count with this exact count
  auto result = FishersExact2Test1TailIntegrationX(left, right);
  // Loop every better exact count
  while (left.exact && right.missed) {
    while (right.made && left.missed) {
      ++left.made;
      --right.made;
      --left.missed;
      ++right.missed;
    }
    if (!right.missed) {
      break;
    }
    --left.exact;
    ++right.exact;
    ++left.missed;
    --right.missed;
    assert(l.total() == left.total());
    assert(r.total() == right.total());
    assert(l.exact + r.exact == left.exact + right.exact);
    assert(l.made + r.made == left.made + right.made);
    assert(l.missed + r.missed == left.missed + right.missed);
    // Add in the probability of this exact count
    result += FishersExact2Test(left, right);
    // Add in every better made count with the improved exact count
    result += FishersExact2Test1TailIntegrationX(left, right);
  }
  return result;
}

// Fisher's exact test for 3 categories, 1-tail integration
mpq_class FishersExact2Test1TailIntegration(Sample2 const base,
                                            Sample2 const comparand) {
  mpq_class result = FishersExact2Test(base, comparand);
  if (((1.0 * comparand.exact) / comparand.total() >
       (1.0 * base.exact) / base.total()) ||
      (((1.0 * comparand.exact) / comparand.total() ==
        (1.0 * base.exact) / base.total()) &&
       ((1.0 * comparand.made) / comparand.total() >=
        (1.0 * base.made) / base.total()))) {
    result += FishersExact2Test1TailIntegrationY(base, comparand);
  } else {
    result += FishersExact2Test1TailIntegrationY(comparand, base);
  }
  return result;
}
}  // namespace phil::stats
