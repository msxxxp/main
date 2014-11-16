#ifndef BASIS_CONFIGURE_OBLIGATORY_THIS_FILE_HPP_
#define BASIS_CONFIGURE_OBLIGATORY_THIS_FILE_HPP_

const char* filename_only(const char* path);

const wchar_t* filename_only(const wchar_t* path);

const char* funcname_only(const char* path, char ch = '(');

const wchar_t* funcname_only(const wchar_t* path, wchar_t ch = L'(');

#endif
