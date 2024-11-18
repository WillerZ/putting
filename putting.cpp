#include <gmpxx.h>

#include <iostream>
#include <iomanip>

struct Sample
{
  unsigned long made;
  unsigned long missed;
  unsigned long total() const { return made + missed; }
  Sample operator-(Sample other) const noexcept
  {
    return Sample{made - other.made, missed - other.missed};
  }
};

mpq_class FishersExactTest(unsigned long a, unsigned long b, unsigned long c,
                           unsigned long d)
{
  mpz_class x, y, z;
  mpz_bin_uiui(x.get_mpz_t(), a + c, a);
  mpz_bin_uiui(y.get_mpz_t(), b + d, b);
  mpz_bin_uiui(z.get_mpz_t(), a + b + c + d, a + b);
  return mpq_class{x*y} / z;
}

mpq_class PhilsTestMoreExtreme(Sample left, Sample right)
{
  mpq_class result;
  while (left.made && right.missed)
  {
    --left.made;
    ++left.missed;
    ++right.made;
    --right.missed;
    result +=
        FishersExactTest(left.made, left.missed, right.made, right.missed);
  }
  return result;
}

// Fisher's exact test, 1-tail integration
mpq_class PhilsTest(Sample const base, Sample const comparand)
{
  mpq_class result = FishersExactTest(base.made, base.missed, comparand.made,
                                      comparand.missed);
  if ((1.0 * comparand.made) / comparand.total() >=
      (1.0 * base.made) / base.total())
  {
    result += PhilsTestMoreExtreme(base, comparand);
  }
  if ((1.0 * comparand.made) / comparand.total() <=
      (1.0 * base.made) / base.total())
  {
    result += PhilsTestMoreExtreme(comparand, base);
  }
  return result;
}

int main(void)
{
  while (!std::cin.eof()) {
    std::string name1;
    std::string name2;
    Sample sample1;
    Sample sample2;
    std::cin >> name1 >> sample1.made >> sample1.missed >> name2 >> sample2.made >> sample2.missed;
    if ((1.0 * sample1.made) / sample1.total() > (1.0 * sample2.made) / sample2.total()) {
      std::cout << name1 << " is better than " << name2 << " at " << std::fixed << std::setprecision(1) << 100.0 - (mpf_class{PhilsTest(sample1, sample2)} * 100.0) << "% confidence\n";
    } else if ((1.0 * sample1.made) / sample1.total() < (1.0 * sample2.made) / sample2.total()) {
      std::cout << name2 << " is better than " << name1 << " at " << std::fixed << std::setprecision(1) << 100.0 - (mpf_class{PhilsTest(sample1, sample2)} * 100.0) << "% confidence\n";
    } else {
      std::cout << name2 << " is equivalent to " << name1 << "\n";
    }
  }
  return 0;
}
