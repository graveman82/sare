#ifndef SARE_LOG_H
#define SARE_LOG_H


#include <vector>
#include <string>
#include <stdint.h>

#include "singleton.h"

namespace sare {

//-----------------------------------------------------------------------------
/** Class for auto flushing logger in function body. See example in Logger class
comment.
*/
class LoggerAutoFlush {
public:
    LoggerAutoFlush() { ref_count_++; }
    LoggerAutoFlush(const LoggerAutoFlush& oth) { ref_count_++; }
    ~LoggerAutoFlush();
private:
    static int ref_count_;
};

//-----------------------------------------------------------------------------
/** @class Класс для журналирования отладочной информации.

    *Пример использования*
    @code
    //-----------------------------------------------------------------------------
    bool TestLogInner() {
        Logger& logger = Singleton<Logger>::Instance();
        logger.StartMessage(Logger::kMessageType_Info);
        logger << "Some system2 initialization...\n";
        logger.FixIndent();
        logger.IncIndent();
        logger << "-bla-bla-bla" << " , value set: " << 0.12345 << "\n";
        logger << "-bla-bla-bla\n";
        if (true) {
            logger.StartMessage(Logger::kMessageType_Error);
            logger << "-some error\n";
            logger.RestoreIndent();
            logger << "Some system2 initialization failed.\n";
            return false;
        }
        logger.RestoreIndent();
        logger << "Some system2 initialization done.\n";
        return true;
    }
    //-----------------------------------------------------------------------------
    void TestLog() {
        Logger& logger = Singleton<Logger>::Instance();
        logger.ResetIndent();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Info);
        logger << "Some system initialization...\n";
        logger.FixIndent();
        logger.IncIndent();
        if(!TestLogInner()) {
            logger.RestoreIndent();
            logger.StartMessage(Logger::kMessageType_Error);
            logger << "Some system initialization failed.\n";
            return;
        }
        logger.RestoreIndent();
        logger << "Some system initialization done.\n";
    }
    //-----------------------------------------------------------------------------
    @endcode

*/
class Logger : public Singleton<Logger> {
    typedef std::vector<std::string> StringVector;
public:
    enum E_MessageTypes {
        kMessageType_Info,
        kMessageType_Warn,
        kMessageType_Error,
        kMessageType_Fatal,
        kMessageType_Trace
    };
    Logger();
    ~Logger();
    /// Start any new messaging by this method.
    LoggerAutoFlush StartMessage(E_MessageTypes msg_type);
    Logger& operator <<(char ch);
    Logger& operator <<(const char* text);
    Logger& operator <<(const std::string& text);
    Logger& operator <<(uint32_t ui);
    Logger& operator <<(int32_t i);
    Logger& operator <<(uint16_t ui);
    Logger& operator <<(int16_t i);
    Logger& operator <<(uint8_t ui);
    Logger& operator <<(int8_t i);
    Logger& operator <<(float f);
    Logger& operator <<(double d);
    Logger& AppendQuoted(const char* text);
    Logger& AppendQuoted(const std::string& text);
    /// Flush all messages in buffer explicitly
    void Flush();

    void IncIndent();
    void DecIndent();
    void ResetIndent();
    void FixIndent();
    void RestoreIndent();

private:
    std::string& UpdateIndent(bool without_header = false);
    void RewriteLastIndent();

    std::string indent_;
    std::string indent_without_header_;
    uint32_t indent_level_;
    uint32_t indent_level_last_;
    uint32_t header_char_count_; // lengths of message header.
    uint32_t header_char_count_last_;
    StringVector messages_; // message buffer
    uint32_t new_msg_i_;
    std::vector<uint32_t> indent_level_stack_;

};

} // end of sare
#endif // SARE_LOG_H
