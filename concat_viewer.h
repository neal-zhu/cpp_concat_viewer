#ifndef CONCAT_VIEWER_H_
#define CONCAT_VIEWER_H_

#include <assert.h>

#include <iterator>
#include <tuple>
#include <vector>

template <typename Iterator>
class ConcatViewer {
 public:
  using input_difference = typename Iterator::difference_type;

  struct Range {
    std::tuple<Iterator, Iterator> range;
    input_difference distance;
    Range(const std::tuple<Iterator, Iterator> &r, input_difference d)
        : range(r), distance(d) {}
  };

  void Concat(Iterator first, Iterator last) {
    input_difference dist = std::distance(first, last);
    if (dist == 0) return;
    ranges_.emplace_back(std::make_tuple(first, last), dist);
    size_ += dist;
  }

  size_t size() const { return size_; }

  class iterator {
   public:
    using difference_type = typename Iterator::difference_type;
    using value_type = typename Iterator::value_type;
    using pointer = typename Iterator::pointer;
    using reference = typename Iterator::reference;
    using iterator_category = typename std::input_iterator_tag;

    iterator() {}
    bool operator==(const iterator &it) const {
      return cv_ == it.cv_ && cur_index_ == it.cur_index_ &&
             cur_pos_ == it.cur_pos_;
    }

    bool operator!=(const iterator &it) const {
      return !(operator==(it));
    }

    reference operator*() const { return *cur_; }

    pointer operator->() const { return std::addressof(operator*()); }

    iterator &operator++() {
      if (++cur_pos_ >= cv_->ranges_[cur_index_].distance) {
        cur_pos_ = 0;
        if (++cur_index_ < cv_->ranges_.size()) {
          cur_ = std::get<0>(cv_->ranges_[cur_index_].range);
        }
      } else {
        std::advance(cur_, 1);
      }
      return *this;
    }

    iterator operator++(int) {
      iterator old(*this);
      ++(*this);
      return old;
    }

   private:
    friend class ConcatViewer;

    Iterator cur_;
    size_t cur_index_;
    size_t cur_pos_;
    const ConcatViewer *cv_;

    iterator(const ConcatViewer *cv, size_t index, size_t pos)
        : cur_index_(index), cur_pos_(pos), cv_(cv) {
      assert(std::make_tuple(index, pos) == cv_->end_pos() ||
             (index < cv->ranges_.size() && pos < cv->ranges_[index].distance));
      if (cv->end_pos() != std::make_tuple(index, pos)) {
        cur_ = std::get<0>(cv->ranges_[index].range);
        std::advance(cur_, pos);
      }
    }
  };

  iterator begin() { return iterator(this, 0, 0); }

  iterator end() {
    size_t index, pos;
    std::tie(index, pos) = end_pos();
    return iterator(this, index, pos);
  }

 private:
  const Range &at(int index) const { return ranges_[index]; }

  std::tuple<size_t, size_t> end_pos() const { return {ranges_.size(), 0}; }

  std::vector<Range> ranges_;
  size_t size_ = 0;
};


#endif  // CONCAT_VIEWER_H_
