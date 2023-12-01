#include "Containers/srFileName.h"
#include <boost/filesystem.hpp>
#include <algorithm>
#include "Containers/srDynamicArray.h"

bool SrFileName::assign(const std::wstring& s)
{
    m_root.clear();
    m_dirPath.clear();
    m_filename.clear();
    m_ext.clear();

    if (s.find(L"\\\\") != s.npos)
        return false;
    if (s.find(L"//") != s.npos)
        return false;

    static SrDynamicArray<std::wstring> pathParts;
    pathParts.clear();

    const wchar_t* slashes = L"\\/";

    std::wstring::size_type pos = 0;
    std::wstring::size_type lastPos = pos;

    bool bIsFile{true};
    for (; pos < s.length(); )
    {
        lastPos = pos;
        pos = s.find_first_of(slashes, pos);
        if (pos != lastPos)
        {
            std::wstring::size_type pos2 = pos;
            if (pos2 == s.npos)
                pos2 = s.length();

            pathParts.push_back(s.substr(lastPos, pos2 - lastPos));
            if (pos2 + 1 == s.length())
            {
                bIsFile = false;
                break;
            }
        }
        if (pos == s.npos)
        {
            break;
        }
        pos++;
    }

    if (pathParts.size() > 0)
    {
        std::wstring& part = pathParts.back();
        if (part.length() >= 2)
        {
            if (part.at(1) == L':')
            {
                m_root = part;
                pathParts.pop_back();
            }
        }
        if (m_root.empty() && bIsFile)
        {
            if (!isValidFileName(part))
            {
                return false;
            }

            std::wstring::size_type extPos = part.rfind(L'.');
            if (extPos != part.npos && extPos + 1 < part.length())
            {
                m_ext = part.substr(extPos + 1);
                part.erase(extPos);
                m_filename = part;
                pathParts.pop_back();
            }
        }

    }
    int partCount{};

    for (auto it = pathParts.begin(); it != pathParts.end(); ++it, ++partCount)
    {
        std::wstring& part = *it;
        if (0 == partCount && m_root.empty())
        {
            if (part.length() >= 2)
            {
                if (part.at(1) == L':')
                {
                    m_root = part;
                }
            }
        }
        else
        {
            if (part.find(L":") != part.npos)
                return false;

            if (part.find(L"..") != part.npos)
            {
                if (part.length() != 2)
                    return false;
                m_dirPath += part;
                m_dirPath += L"/";
            }
            else if (part.find(L".") != part.npos)
            {
                if (part.length() != 1)
                    return false;
                m_dirPath += part;
                m_dirPath += L"/";
            }
            else
            {
                if (!isValidFileName(part))
                    return false;
                m_dirPath += part;
                m_dirPath += L"/";
            }
        }
    }
    return true;
}

bool SrFileName::isValidFileName(std::wstring& s)
{
    if (s.empty())
        return false;
#ifdef _WIN32
    for (auto ch : s)
    {
        if ((uint16_t)ch < uint16_t(32))
            return false;
    }
    static std::wstring prohibiteds = L"\"*<>?\\/|:";
    return (s.find_first_of(prohibiteds) == s.npos);
#endif // _WIN32
    return false;
}

void SrFileName::getPath(std::wstring& dst)
{
    dst += m_root;
    if (!m_root.empty())
        dst += L"/";
    dst += m_dirPath;

    dst += m_filename;
    if (!m_filename.empty() && !m_ext.empty())
        dst += L".";
    dst += m_ext;
}
