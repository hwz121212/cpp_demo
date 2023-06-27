#ifndef DOS2UNIX_H
#define DOS2UNIX_H

#include <stdlib.h>
#include "buffer.h"

class dos2unix {
  public:
    enum class type {
      kEmpty,
      kBinary,
      kMixed,
      kUnix,
      kDos
    };

    static const wchar_t* type_to_string(type t);

    enum class operation {
      kCheck,
      kConvertToUnix,
      kConvertToDos
    };

    // Constructor.
    dos2unix();

    // Destructor.
    ~dos2unix();

    // Add file extension.
    bool add_file_extension(const wchar_t* ext);

    // Install.
    bool install(const wchar_t* filename);

    // Uninstall.
    bool uninstall();

    // Load file extensions from registry.
    bool load_file_extensions_from_registry();

    // Check line endings.
    bool check(const wchar_t* name);

    // Check line endings recursively.
    bool check_directory(const wchar_t* dirname);

    // Check file's line endings.
    bool check_file(const wchar_t* filename, _off_t filesize);

    // Convert to UNIX.
    bool to_unix(const wchar_t* name, bool backup);

    // Convert to UNIX recursively.
    bool directory_to_unix(const wchar_t* dirname, bool backup);

    // Convert file to UNIX.
    bool file_to_unix(const wchar_t* filename, _off_t filesize, bool backup);

    // Convert to DOS.
    bool to_dos(const wchar_t* name, bool backup);

    // Convert to DOS recursively.
    bool directory_to_dos(const wchar_t* dirname, bool backup);

    // Convert file to DOS.
    bool file_to_dos(const wchar_t* filename, _off_t filesize, bool backup);

    // Get log.
    const buffer& log() const;

  private:
    static const size_t kFileExtensionMaxLength = 16;
    static const unsigned kMaxFilenameSearches = 32;
    static constexpr const wchar_t* const kProgram = L"dos2unix";
    static constexpr const wchar_t* const kLineEndingsFileType = L"lineendings";
    static constexpr const wchar_t* const kLineEndingsShell = L"LineEndings";

    wchar_t** _M_file_extensions;
    size_t _M_size;
    size_t _M_used;

    buffer _M_log;

    // Process.
    bool process(const wchar_t* name, operation op, bool backup = false);

    // Walk directory.
    bool walk_directory(const wchar_t* dirname,
                        operation op,
                        bool backup = false);

    // Convert file.
    bool convert_file(const wchar_t* filename,
                      _off_t filesize,
                      type final_type,
                      bool backup);

    // Is target file?
    bool target(const wchar_t* filename) const;

    // Register file extensions.
    bool register_file_extensions(const wchar_t* filename);

    // Unregister file extensions.
    bool unregister_file_extensions();

    // Register folder.
    bool register_folder();

    // Unregister folder.
    bool unregister_folder();

    // Register program.
    bool register_program();

    // Unregister program.
    bool unregister_program();

    // Register line endings.
    bool register_line_endings(const wchar_t* subkey);

    // Load file extensions from file.
    bool load_file_extensions_from_file(const wchar_t* filename);

    // Set values.
    bool set_values(HKEY hKey,
                    const wchar_t* const key,
                    const wchar_t** subkeys,
                    const wchar_t** values,
                    size_t count);

    // Generate log file.
    bool generate_log_file(const wchar_t* filename) const;
};

inline const wchar_t* dos2unix::type_to_string(type t)
{
  switch (t) {
    case type::kEmpty:
      return L"empty";
    case type::kBinary:
      return L"binary";
    case type::kMixed:
      return L"mixed (CRLF & LF)";
    case type::kUnix:
      return L"UNIX";
    case type::kDos:
      return L"DOS";
    default:
      return L"(unknown)";
  }
}

inline dos2unix::dos2unix()
  : _M_file_extensions(NULL),
    _M_size(0),
    _M_used(0)
{
}

inline bool dos2unix::check(const wchar_t* name)
{
  return process(name, operation::kCheck);
}

inline bool dos2unix::check_directory(const wchar_t* dirname)
{
  return walk_directory(dirname, operation::kCheck);
}

inline bool dos2unix::to_unix(const wchar_t* name, bool backup)
{
  return process(name, operation::kConvertToUnix, backup);
}

inline bool dos2unix::directory_to_unix(const wchar_t* dirname, bool backup)
{
  return walk_directory(dirname, operation::kConvertToUnix, backup);
}

inline bool dos2unix::file_to_unix(const wchar_t* filename,
                                   _off_t filesize,
                                   bool backup)
{
  return convert_file(filename, filesize, type::kUnix, backup);
}

inline bool dos2unix::to_dos(const wchar_t* name, bool backup)
{
  return process(name, operation::kConvertToDos, backup);
}

inline bool dos2unix::directory_to_dos(const wchar_t* dirname, bool backup)
{
  return walk_directory(dirname, operation::kConvertToDos, backup);
}

inline bool dos2unix::file_to_dos(const wchar_t* filename,
                                  _off_t filesize,
                                  bool backup)
{
  return convert_file(filename, filesize, type::kDos, backup);
}

inline const buffer& dos2unix::log() const
{
  return _M_log;
}

#endif // DOS2UNIX_H
