#include <gmpxx.h>
#include <map>

namespace phil::stats {

struct IntermediateCache {
  mpz_class const &getBinomialCoefficient(unsigned long const from, unsigned long const select) {
    auto [fromIter, createdFrom] = binomialCoefficientCache_.try_emplace(from);
    auto [selectIter, createdSelect] = fromIter->second.try_emplace(select);
    if (createdSelect) {
      mpz_bin_uiui(selectIter->second.get_mpz_t(), from, select);
    }
    return selectIter->second;
  }

  mpf_class const &getReciprocal(mpz_class const &key) {
    auto [iter, created] = reciprocalCache_.try_emplace(key);
    if (created) {
      iter->second = 1;
      iter->second /= key;
    }
    return iter->second;
  }

private:
  std::map<unsigned long, std::map<unsigned long, mpz_class>> binomialCoefficientCache_{};
  std::map<mpz_class, mpf_class> reciprocalCache_{};
};

} // namespace phil::stats