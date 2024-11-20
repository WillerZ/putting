#include <chrono>
#include <iomanip>
#include <iostream>

#include "stats.hpp"

using namespace phil::stats;

int main(void) {
  std::chrono::steady_clock::time_point pre, post;
  pre = std::chrono::steady_clock::now();
  while (!std::cin.eof()) {
    std::string name1;
    std::string name2;
    Sample sample1;
    Sample sample2;
    std::cin >> name1 >> sample1.made >> sample1.missed >> name2 >> sample2.made >> sample2.missed;
    if ((1.0 * sample1.made) / sample1.total() > (1.0 * sample2.made) / sample2.total()) {
      std::cout << name1 << " is distinct from " << name2 << " at " << std::fixed << std::setprecision(1)
                << 100.0 - (mpf_class{FishersExactTest2TailIntegration(sample1, sample2)} * 100.0) << "% confidence ";
    } else {
      std::cout << name2 << " is distinct from " << name1 << " at " << std::fixed << std::setprecision(1)
                << 100.0 - (mpf_class{FishersExactTest2TailIntegration(sample1, sample2)} * 100.0) << "% confidence ";
    }
    post = std::chrono::steady_clock::now();
    std::cout << "in " << std::chrono::duration_cast<std::chrono::microseconds>(post - pre) << "\n";
    pre = post;
  }
  return 0;
}
