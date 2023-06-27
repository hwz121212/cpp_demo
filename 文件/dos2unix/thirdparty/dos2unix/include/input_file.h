#ifndef INPUT_FILE_H
#define INPUT_FILE_H

#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

class input_file {
  public:
    // Constructor.
    input_file();

    // Destructor.
    ~input_file();

    // Open.
    bool open(const wchar_t* filename);
    bool open(const wchar_t* filename, _off_t filesize);

    // Close.
    void close();

    // Get data.
    const uint8_t* data() const;

    // Get size.
    size_t size() const;

    // Get end of data.
    const uint8_t* const end() const;

  private:
    HANDLE _M_file;

    HANDLE _M_mapping;

    void* _M_data;
    size_t _M_size;
    const uint8_t* _M_end;
};

inline input_file::input_file()
  : _M_file(INVALID_HANDLE_VALUE),
    _M_mapping(NULL),
    _M_data(NULL),
    _M_size(0)
{
}

inline input_file::~input_file()
{
  close();
}

inline const uint8_t* input_file::data() const
{
  return reinterpret_cast<const uint8_t*>(_M_data);
}

inline size_t input_file::size() const
{
  return _M_size;
}

inline const uint8_t* const input_file::end() const
{
  return _M_end;
}

#endif // INPUT_FILE_H
