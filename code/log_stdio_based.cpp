#include <vector>
#include <string>

#include <cstdio>
#include <io.h>

namespace sare {

#ifndef SARE_LOGFILENAME
#define SARE_LOGFILENAME "SareDebugLog.txt"
#endif // SARE_LOGFILENAME

//-----------------------------------------------------------------------------
class LogFile {
public:
    LogFile();
    ~LogFile();

    operator bool() {return fp_ != 0;}

    FILE* GetFilePtr() const { return fp_; }
private:
    FILE* fp_;
};

//-----------------------------------------------------------------------------
LogFile::LogFile() :
    fp_(0) {
    static bool fLog = false;
    static const char* logFilename = SARE_LOGFILENAME;
    if (!fLog) {
        // Попытка открыть файл лога для чтения
        if (access(logFilename, 0) == 0) {
            // Удаляем файл, если он существует
            remove(logFilename);
        }
        fLog = true;
    }
    // Пишем в лог
    //
    fp_ = fopen(logFilename, "a");
}

//-----------------------------------------------------------------------------
LogFile::~LogFile() {
    if (fp_ != 0) {
        fclose(fp_);
        fp_ = 0;
    }
}

//-----------------------------------------------------------------------------
void Log(std::vector<std::string>& messages) {
    LogFile lf;
    if (lf) {
        for (uint32_t i = 0; i < messages.size(); ++i) {
            fputs(messages[i].c_str(), lf.GetFilePtr());
            fputc('\n', lf.GetFilePtr());
        }

    }
}

} // end of sare


