#include <chrono>
#include <iomanip>
#include <iostream>

#include "stats.hpp"

using namespace phil::stats;

int main(void) {
  mpf_set_default_prec(128);
  std::chrono::steady_clock::time_point pre, post;
  pre = std::chrono::steady_clock::now();
  while (!std::cin.eof()) {
    std::string name1;
    std::string name2;
    Sample sample1;
    Sample sample2;
    std::cin >> name1 >> sample1.made >> sample1.missed >> name2 >> sample2.made >> sample2.missed;
    auto [exact, better] = WilloughbysExactTestCumulative(sample1, sample2);
    std::cout << std::fixed << std::setprecision(3);
    std::cout << name2 << " is equivalent or better than " << name1 << " " << (exact * 0.5 + better) * 100.0 << "% ";
    post = std::chrono::steady_clock::now();
    std::cout << "in " << std::chrono::duration_cast<std::chrono::microseconds>(post - pre) << "\n";
    pre = post;
  }
  return 0;
}
