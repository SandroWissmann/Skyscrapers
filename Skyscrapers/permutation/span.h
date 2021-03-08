#ifndef PERMUTATION_SPAN_H
#define PERMUTATION_SPAN_H

#include <cassert>
#include <iterator>

namespace permutation {

template <typename T> class Span {
public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using const_iterator = const_pointer;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Span(const_pointer ptr, size_type size);

    constexpr const_pointer data() const noexcept;

    constexpr size_type size() const noexcept;

    constexpr const_reference operator[](size_type idx) const;

    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;

    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

private:
    const_pointer mPtr;
    size_type mSize;
};

template <typename T>
Span<T>::Span(const_pointer ptr, size_type size) : mPtr{ptr}, mSize{size}
{
}

template <typename T>
constexpr typename Span<T>::const_pointer Span<T>::data() const noexcept
{
    return mPtr;
}

template <typename T>
constexpr typename Span<T>::size_type Span<T>::size() const noexcept
{
    return mSize;
}

template <typename T>
constexpr typename Span<T>::const_reference
Span<T>::operator[](Span::size_type idx) const
{
    assert(idx < mSize);
    return *(data() + idx);
}

template <typename T>
constexpr typename Span<T>::const_iterator Span<T>::cbegin() const noexcept
{
    return data();
}

template <typename T>
constexpr typename Span<T>::const_iterator Span<T>::cend() const noexcept
{
    return data() + size();
}

template <typename T>
constexpr typename Span<T>::const_reverse_iterator
Span<T>::crbegin() const noexcept
{
    return reverse_iterator(cend());
}

template <typename T>
constexpr typename Span<T>::const_reverse_iterator
Span<T>::crend() const noexcept
{
    return reverse_iterator(cbegin());
}

} // namespace permutation

#endif
