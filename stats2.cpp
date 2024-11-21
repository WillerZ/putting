#include "stats2.hpp"
#include "cache.hpp"

#include <gmpxx.h>

#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>

namespace phil::stats {

static mpf_class FishersExact2Test(IntermediateCache &cache, Sample2 const one, Sample2 const other) {
  auto &&w = cache.getBinomialCoefficient(one.exact + other.exact, one.exact);
  auto &&x = cache.getBinomialCoefficient(one.made + other.made, one.made);
  auto &&y = cache.getBinomialCoefficient(one.missed + other.missed, one.missed);
  auto &&z = cache.getReciprocal(cache.getBinomialCoefficient(one.total() + other.total(), one.total()));
  return w * x * y * z;
}

// tail case missed -> made
static mpf_class FishersExact2Test1TailIntegrationX(IntermediateCache &cache, Sample2 const l, Sample2 const r) {
  Sample2 left = l;
  Sample2 right = r;
  mpf_class result;
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
    result += FishersExact2Test(cache, left, right);
  }
  return result;
}

// tail case missed -> made -> exact
static mpf_class FishersExact2Test1TailIntegrationY(IntermediateCache &cache, Sample2 const l, Sample2 const r) {
  Sample2 left = l;
  Sample2 right = r;
  // Every better made count with this exact count
  auto result = FishersExact2Test1TailIntegrationX(cache, left, right);
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
    result += FishersExact2Test(cache, left, right);
    // Add in every better made count with the improved exact count
    result += FishersExact2Test1TailIntegrationX(cache, left, right);
  }
  return result;
}

// Fisher's exact test for 3 categories, 1-tail integration
mpf_class FishersExact2Test1TailIntegration(Sample2 const base, Sample2 const comparand) {
  IntermediateCache cache;
  auto result = FishersExact2Test(cache, base, comparand);
  if (((1.0 * comparand.exact) / comparand.total() > (1.0 * base.exact) / base.total()) ||
      (((1.0 * comparand.exact) / comparand.total() == (1.0 * base.exact) / base.total()) &&
       ((1.0 * comparand.made) / comparand.total() >= (1.0 * base.made) / base.total()))) {
    return result + FishersExact2Test1TailIntegrationY(cache, base, comparand);
  } else {
    return result + FishersExact2Test1TailIntegrationY(cache, comparand, base);
  }
}
} // namespace phil::stats
