#pragma once

namespace dungeon {

// An interval on the real line.
struct Interval {
  Interval() = default;
  Interval(float _low, float _high) : low(_low), high(_high) {}
  float low{};
  float high{};

  static Interval Union(const Interval& a, const Interval& b) {
    return Interval(std::min(a.low, b.low), std::max(a.high, b.high));
  }

  friend bool operator==(const Interval& lhs, const Interval& rhs) {
    return std::make_pair(lhs.low, lhs.high) ==
           std::make_pair(rhs.low, rhs.high);
  }

  friend std::ostream& operator<<(std::ostream& os, const Interval& obj) {
    os << "Interval [" << obj.low << ", " << obj.high << "]";
    return os;
  }
};

}  // namespace dungeon
