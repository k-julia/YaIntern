#pragma once

#include <iostream>
#include <vector>

class big_integer {
 private:
  std::vector<int> digits_;
  bool sign_ = true;

 public:
  big_integer() = default;
  explicit big_integer(const std::vector<int>& num) {
    for (size_t i = num.size() - 1; i >=0; --i) {
      digits_.emplace_back(num[i]);
    }
  }
  [[maybe_unused]] big_integer(const std::vector<int>& num, bool sign) {
    for (size_t i = num.size() - 1; i >=0; --i) {
      digits_.emplace_back(num[i]);
    }
    if (!sign) {
      sign_ = false;
    }
  }
  big_integer(const big_integer& rhs) {
    for (const auto& element : rhs.digits_) {
      digits_.emplace_back(element);
    }
    sign_ = rhs.sign_;
  }

  friend std::istream& operator>>(std::istream& stream, big_integer& a);
  friend std::ostream& operator<<(std::ostream& stream, big_integer& a);

  big_integer operator-(const big_integer& rhs) const;
  big_integer operator+(const big_integer& rhs);
  big_integer operator*(const big_integer& rhs);
  big_integer operator/(const big_integer& rhs);
  big_integer operator%(const big_integer& rhs);

  friend bool operator==(const big_integer& lhs, const big_integer& rhs);
  friend bool operator!=(const big_integer& lhs, const big_integer& rhs);
  friend bool operator<(const big_integer& lhs, const big_integer& rhs);
  friend bool operator>(const big_integer& lhs, const big_integer& rhs);
  friend bool operator<=(const big_integer& lhs, const big_integer& rhs);
  friend bool operator>=(const big_integer& lhs, const big_integer& rhs);

  big_integer operator+() const { return big_integer(*this);}
  big_integer operator-() const {
    big_integer copy(*this);
    copy.sign_ = !copy.sign_;
    return copy;
  }

  big_integer& operator=(const big_integer& rhs) {
    if (rhs.digits_.size() != digits_.size()) {
      digits_.resize(rhs.digits_.size());
    }
    for (int i{0}; i < digits_.size(); ++i) {
      digits_[i] = rhs.digits_[i];
    }
    sign_ = rhs.sign_;
    return *this;
  }

  big_integer& operator +=(const big_integer& value) {
    return *this = (*this + value);
  }
  big_integer& operator -=(const big_integer& value) {
    return *this = (*this - value);
  }

  big_integer operator++() {
    ++digits_[0];
    return *this;
  }
  const big_integer operator ++(int) {
    big_integer output = *this;
    ++digits_[0];
    return output;
  }

  big_integer operator--() {
    --digits_[0];
    return *this;
  }
  const big_integer operator --(int) {
    big_integer output = *this;
    --digits_[0];
    return output;
  }
};