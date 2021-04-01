#include <algorithm>
#include "long_number.h"

std::istream& operator>>(std::istream& stream, big_integer& a) {
  std::string s;
  std::getline(stream, s);
  for (int i = s.size() - 1; i > 0; --i) {
    a.digits_.push_back(s[i] - '0');
  }
  if (s[0] != '-') {
    a.digits_.push_back(s[0] - '0');
  } else {
    a.sign_ = false;
  }
  return stream;
}

std::ostream& operator<<(std::ostream& stream, big_integer& number) {
  if (!number.sign_) {
    stream << '-';
  }
  for (int i = number.digits_.size() - 1; i >= 0; --i) {
    stream << number.digits_[i];
  }
  stream << std::endl;
  return stream;
}

bool operator==(const big_integer& lhs, const big_integer& rhs) {
  if (lhs.sign_ != rhs.sign_ || lhs.digits_.size() != rhs.digits_.size()) {
    return false;
  }
  for (int i{0}; i < lhs.digits_.size(); ++i) {
    if (lhs.digits_[i] != rhs.digits_[i]) {
      return false;
    }
  }
  return true;
}

bool operator!=(const big_integer& lhs, const big_integer& rhs) {
  return !(lhs == rhs);
}

bool operator<(const big_integer& lhs, const big_integer& rhs) {
  if (lhs == rhs) {
    return false;
  }
  if (lhs.sign_) {
    if (!rhs.sign_) {
      return false;
    } else {
      if (lhs.digits_.size() < rhs.digits_.size()) {
        return true;
      } else if (rhs.digits_.size() < lhs.digits_.size()) {
        return false;
      }
      for (int i = lhs.digits_.size(); i >= 0; --i) {
        if (lhs.digits_[i] < rhs.digits_[i]) {
          return true;
        }
      }
      return false;
    }
  } else {
    if (rhs.sign_) {
      return true;
    } else {
      if (lhs.digits_.size() < rhs.digits_.size()) {
        return false;
      } else if (rhs.digits_.size() < lhs.digits_.size()) {
        return true;
      }
      for (int i = lhs.digits_.size(); i >= 0; --i) {
        if (lhs.digits_[i] > rhs.digits_[i]) {
          return true;
        }
      }
      return false;
    }
  }
}

bool operator>(const big_integer& lhs, const big_integer& rhs) {
  return !(lhs < rhs);
}

bool operator<=(const big_integer& lhs, const big_integer& rhs) {
  return (lhs < rhs) || (lhs == rhs);
}

bool operator>=(const big_integer& lhs, const big_integer& rhs) {
  return !(lhs < rhs) || lhs == rhs;
}

big_integer big_integer::operator-(const big_integer& rhs) const {
  big_integer output = *this;
  if (!rhs.sign_) {
      return output + (-rhs);
  }
  if (!sign_) {
    return -(-output + rhs);
  }
  if ((*this) < rhs) {
    return -(rhs - output);
  }
  int borrowed_pos;
  for (int i{0}; i < rhs.digits_.size(); ++i) {
    if (output.digits_[i] < rhs.digits_[i]) {
      borrowed_pos = i + 1;
      while (output.digits_[borrowed_pos] == 0) {
        output.digits_[borrowed_pos] = 9;
        ++borrowed_pos;
      }
      --output.digits_[borrowed_pos];
      output.digits_[i] = output.digits_[i] + 10 - rhs.digits_[i];
    } else {
      output.digits_[i] = output.digits_[i] - rhs.digits_[i];
    }
  }
  if (output.digits_[output.digits_.size() - 1] != 0) {
    return output;
  }
  size_t i = output.digits_.size() - 1;
  while (output.digits_[i] == 0 && i != 0) {
    --i;
  }
  output.digits_.resize(i + 1);
  return output;
}

big_integer big_integer::operator+(const big_integer& rhs) {
  big_integer output = *this;
  if (!sign_) {
    if (!rhs.sign_) {
      return - (-output + (-rhs));
    } else {
      return rhs - (-output);
    }
  } else if (!rhs.sign_){
    return output - (-rhs);
  }
  int source;
  for (int i{0}; i < rhs.digits_.size(); ++i) {
    source = output.digits_[i] + rhs.digits_[i];
    if (source >= 10) {
      if (i == output.digits_.size() - 1) {
        output.digits_.resize(output.digits_.size() + 1);
      }
      output.digits_[i] = source - 10;
      ++output.digits_[i + 1];
    } else {
      output.digits_[i] = source;
    }
  }
  return output;
}

big_integer big_integer::operator*(const big_integer& rhs) {
  big_integer output;
  int source;
  int next;
  output.digits_.resize(rhs.digits_.size() + digits_.size());
  for (int i{0}; i < rhs.digits_.size(); ++i) {
    for (int j{0}; j < digits_.size(); ++j) {
      source = digits_[j] * rhs.digits_[i];
      next = source + output.digits_[i + j];
      output.digits_[i + j] = next % 10;
      source = next / 10;
      next = source + output.digits_[i + j + 1];
      output.digits_[i + j + 1] = next % 10;
      output.digits_[i + j + 2] += next / 10;
    }
  }
  if (output.digits_[output.digits_.size() - 1] != 0) {
    return output;
  } else {
    output.digits_.resize(output.digits_.size() - 1);
  }
  return output;
}

big_integer big_integer::operator/(const big_integer& rhs) {
  if (rhs.digits_.size() == 1 && rhs.digits_[0] == 0) {
    throw std::invalid_argument("Division by zero");
  }
  big_integer output;
  if (digits_.size() < rhs.digits_.size()) {
    output.digits_.emplace_back(0);
    return output;
  }
  if (sign_ != rhs.sign_) {
    output.sign_ = false;
  }
  big_integer divider = rhs;
  big_integer reminder;
  big_integer zero;
  zero.digits_.emplace_back(0);
  reminder.digits_.emplace_back(0);
  divider.sign_ = true;
  int position = digits_.size() - 1;
  bool begin = true;
  while (position >= 0) {
    if (digits_[position] == 0 && reminder == zero) {
      --position;
      output.digits_.emplace_back(0);
      continue;
    }
    big_integer current;
    if (begin) {
      while (current < divider && position >= 0) {
        std::reverse(current.digits_.begin(), current.digits_.end());
        current.digits_.emplace_back(digits_[position]);
        --position;
        std::reverse(current.digits_.begin(), current.digits_.end());
      }
      begin = false;
    } else {
      if (reminder != zero) {
        for (int i{0}; i < reminder.digits_.size(); ++i) {
          current.digits_.emplace_back(reminder.digits_[i]);
        }
      }
      current.digits_.emplace_back(digits_[position]);
      std::reverse(current.digits_.begin(), current.digits_.end());
      if (current < divider) {
        if (reminder == zero) {
          reminder.digits_[0] = digits_[position];
        } else {
          reminder.digits_.emplace_back(digits_[position]);
        }
        output.digits_.emplace_back(0);
        --position;
        continue;
      }
      --position;
    }
    int counter = 0;
    while (current - divider >= zero) {
      current -= divider;
      ++counter;
    }
    output.digits_.emplace_back(counter);
    reminder = current;
  }
  std::reverse(output.digits_.begin(), output.digits_.end());
  return output;
}

big_integer big_integer::operator%(const big_integer& rhs) {
  if (rhs.digits_.size() == 1 && rhs.digits_[0] == 0) {
    throw std::invalid_argument("Division by zero");
  }
  if (digits_.size() < rhs.digits_.size()) {
    return *this;
  }
  big_integer divider = rhs;
  big_integer reminder;
  big_integer zero;
  zero.digits_.emplace_back(0);
  reminder.digits_.emplace_back(0);
  divider.sign_ = true;
  int position = digits_.size() - 1;
  bool begin = true;
  while (position >= 0) {
    if (digits_[position] == 0 && reminder == zero) {
      --position;
      continue;
    }
    big_integer current;
    if (begin) {
      while (current < divider && position >= 0) {
        std::reverse(current.digits_.begin(), current.digits_.end());
        current.digits_.emplace_back(digits_[position]);
        --position;
        std::reverse(current.digits_.begin(), current.digits_.end());
      }
      begin = false;
    } else {
      if (reminder != zero) {
        for (int i{0}; i < reminder.digits_.size(); ++i) {
          current.digits_.emplace_back(reminder.digits_[i]);
        }
      }
      current.digits_.emplace_back(digits_[position]);
      std::reverse(current.digits_.begin(), current.digits_.end());
      if (current < divider) {
        if (reminder == zero) {
          reminder.digits_[0] = digits_[position];
        } else {
          reminder.digits_.emplace_back(digits_[position]);
        }
        --position;
        continue;
      }
      --position;
    }
    int counter = 0;
    while (current - divider >= zero) {
      current -= divider;
      ++counter;
    }
    reminder = current;
  }
  return reminder;
}
