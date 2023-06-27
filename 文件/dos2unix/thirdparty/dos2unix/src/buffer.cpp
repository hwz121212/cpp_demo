#include <stdio.h>
#include "buffer.h"

bool buffer::allocate(size_t size)
{
  if ((size += _M_used) <= _M_size) {
    return true;
  }

  size_t s;
  if (_M_size == 0) {
    s = kInitialSize;
  } else {
    size_t tmp;
    if ((tmp = _M_size * 2) < _M_size) {
      // Overflow.
      return false;
    }

    s = tmp;
  }

  while (s < size) {
    size_t tmp;
    if ((tmp = s * 2) < s) {
      // Overflow.
      return false;
    }

    s = tmp;
  }

  wchar_t* data;
  if ((data = reinterpret_cast<wchar_t*>(
                realloc(_M_data, s * sizeof(wchar_t))
              )) == NULL) {
    return false;
  }

  _M_data = data;
  _M_size = s;

  return true;
}

bool buffer::vformat(const wchar_t* format, va_list ap)
{
  if (!allocate(kInitialSize)) {
    return false;
  }

  int size = static_cast<int>(_M_size - _M_used);

  do {
    va_list aq;
    va_copy(aq, ap);

    int n = _vsnwprintf_s(_M_data + _M_used, size, _TRUNCATE, format, aq);

    va_end(aq);

    if (n > -1) {
      if (n < size) {
        _M_used += n;
        return true;
      }

      size = n + 1;
    } else {
      size *= 2;
    }

    if (!allocate(size)) {
      return false;
    }
  } while (true);
}
