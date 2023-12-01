#ifndef SR_FILENAME_H
#define SR_FILENAME_H

#include <string>


class SrFileName
{
public:

    bool assign(const std::wstring& s);
    void getPath(std::wstring& dst);
private:
    bool isValidFileName(std::wstring& s);

    std::wstring m_dirPath;
    std::wstring m_root;
    std::wstring m_filename;
    std::wstring m_ext;

};

#endif // SR_FILENAME_H

