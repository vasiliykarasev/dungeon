#include <algorithm>
#include <cmath>
#include <iostream>

#include <glog/logging.h>
#include "absl/strings/str_format.h"
#include "data/bloom_filter.h"
#include "data/count_min_sketch.h"

namespace dungeon {

namespace detail {

double BloomFilterErrorRate(int num_hash_functions, int bit_length,
                            int num_items_inserted) {
  return std::pow((1.0 - std::exp(-num_hash_functions * num_items_inserted /
                                  static_cast<double>(bit_length))),
                  num_hash_functions);
}

}  // namespace detail

class BloomFilterEvaluator {
 public:
  BloomFilterEvaluator(int num_hash_functions, int bit_length)
      : bf_(num_hash_functions, bit_length),
        num_hash_functions_(num_hash_functions),
        bit_length_(bit_length) {}

  std::string Evaluate() {
    std::vector<int> items_sizes{4, 32, 64};

    std::stringstream ss;
    ss << absl::StrFormat("BloomFilter num_hash_functions=%d bit_length=%d\n",
                          num_hash_functions_, bit_length_);
    for (int i = 0; i < items_sizes.back(); ++i) {
      bf_.Add(std::to_string(i));
      num_items_inserted_++;
      if (std::count(items_sizes.begin(), items_sizes.end(),
                     num_items_inserted_)) {
        double expected_error_rate = TheoreticalErrorRate();
        double actual_error_rate = EmpiricalErrorRate();
        ss << absl::StrFormat(
            "num_inserted=%d\t expected_error=%.3f\tactual_error=%.3f\n",
            num_items_inserted_, expected_error_rate, actual_error_rate);
      }
    }
    return ss.str();
  }

 private:
  double TheoreticalErrorRate() const {
    return detail::BloomFilterErrorRate(num_hash_functions_, bit_length_,
                                        num_items_inserted_);
  }

  double EmpiricalErrorRate() const {
    constexpr int kNumSamples = 1e5;
    int num_errors = 0;
    int min_item_not_seen = num_items_inserted_ + 1;
    for (int i = 0; i < kNumSamples; ++i) {
      int item =
          rand() % (std::numeric_limits<int>::max() - min_item_not_seen) +
          min_item_not_seen;
      CHECK_GT(item, num_items_inserted_);
      num_errors += bf_.Contains(std::to_string(item));
    }
    return num_errors / static_cast<double>(kNumSamples);
  }

  BloomFilter bf_;
  int num_hash_functions_;
  int bit_length_;
  int num_items_inserted_{0};
};

void BloomFilterEvaluation() {
  const std::vector<int> kNumHashFunctions{1, 4};
  const std::vector<int> kBitLength{8, 16, 128};

  for (int num_hash_functions : kNumHashFunctions) {
    for (int bit_length : kBitLength) {
      BloomFilterEvaluator evaluator(num_hash_functions, bit_length);
      std::cout << "\n" << evaluator.Evaluate();
    }
  }
}

void CountMinSketchEvaluation() {
  // TODO.
}

void main() {
  BloomFilterEvaluation();
  CountMinSketchEvaluation();
}

}  // namespace dungeon

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = 1;
  dungeon::main();
  return 0;
}
