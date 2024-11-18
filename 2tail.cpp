#include "stats.hpp"

#include <iostream>
#include <iomanip>

int main(void)
{
  while (!std::cin.eof()) {
    std::string name1;
    std::string name2;
    phil::stats::Sample sample1;
    phil::stats::Sample sample2;
    std::cin >> name1 >> sample1.made >> sample1.missed >> name2 >> sample2.made >> sample2.missed;
    if ((1.0 * sample1.made) / sample1.total() > (1.0 * sample2.made) / sample2.total()) {
      std::cout << name1 << " is better than " << name2 << " at " << std::fixed << std::setprecision(1) << 100.0 - (mpf_class{phil::stats::FishersExactTest2TailIntegration(sample1, sample2)} * 100.0) << "% confidence\n";
    } else {
      std::cout << name2 << " is better than " << name1 << " at " << std::fixed << std::setprecision(1) << 100.0 - (mpf_class{phil::stats::FishersExactTest2TailIntegration(sample1, sample2)} * 100.0) << "% confidence\n";
    }
  }
  return 0;
}
