#include <memory>
#include <string.h>

#include "assert.h"
#include "log.h"


#ifndef SARE_LOGGER_INDENT_SPACE_COUNT
#define SARE_LOGGER_INDENT_SPACE_COUNT 2
#endif // SARE_LOGGER_INDENT_SPACE_COUNT

namespace sare {

//-----------------------------------------------------------------------------
struct LoggerDeleter { // deleter
    LoggerDeleter() {};
    LoggerDeleter(const LoggerDeleter&) { }
    LoggerDeleter(LoggerDeleter&) { }
    LoggerDeleter(LoggerDeleter&&) { }
    void operator()(Logger* p) const { }
};
static uint8_t s_logger_memory[sizeof(Logger) + 4];
static std::unique_ptr<Logger, LoggerDeleter> s_logger(new (&s_logger_memory[0]) Logger(), LoggerDeleter());

//-----------------------------------------------------------------------------
int LoggerAutoFlush::ref_count_ = 0;

//-----------------------------------------------------------------------------
LoggerAutoFlush::~LoggerAutoFlush() {
    if (!--ref_count_) {
        Logger::instance().Flush();
    }
}

//-----------------------------------------------------------------------------
Logger::Logger() : Singleton<Logger>(*this),
    indent_level_(0),
    indent_level_last_(0),
    header_char_count_(0),
    header_char_count_last_(0),
    new_msg_i_(-1) {}

//-----------------------------------------------------------------------------
Logger::~Logger() {
#ifdef SARE_DEBUG_LEVEL
    printf("Logger::~Logger()\n");
#endif // SARE_DEBUG
}

//-----------------------------------------------------------------------------
LoggerAutoFlush Logger::StartMessage(E_MessageTypes msg_type) {
    std::string s;
    switch (msg_type) {
        case kMessageType_Info:
            s = "[INFO ]: ";
            break;
        case kMessageType_Warn:
            s = "[WARN ]: ";
            break;
        case kMessageType_Error:
            s = "[ERROR]: ";
            break;
        case kMessageType_Fatal:
            s = "[FATAL]: ";
            break;
        case kMessageType_Trace:
            s = "[TRACE]: ";
            break;
    }
    header_char_count_ = s.length();
    messages_.push_back(s + UpdateIndent(true));
    new_msg_i_ = messages_.size() - 1;
    return LoggerAutoFlush();
}

//-----------------------------------------------------------------------------
Logger& Logger::operator <<(char ch) {
    SARE_ASSERT(new_msg_i_ != uint32_t(-1));
    std::string& s = messages_[new_msg_i_];
    s += ch;
    if ('\n' == s.back()) {
        s += UpdateIndent();
    }
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::operator <<(const char* text) {
    SARE_ASSERT(new_msg_i_ != uint32_t(-1));
    std::string& s = messages_[new_msg_i_];
    s += text;
    if ('\n' == s.back()) {
        s += UpdateIndent();
    }
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::operator <<(const std::string& text) {
    SARE_ASSERT(new_msg_i_ != uint32_t(-1));
    std::string& s = messages_[new_msg_i_];
    s += text;
    if ('\n' == s.back()) {
        s += UpdateIndent();
    }
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::operator <<(uint32_t ui) {
    char buf[32] = {};
    sprintf(buf, "%d", ui);
    (*this) << buf;
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::operator <<(int32_t i) {
    char buf[32] = {};
    sprintf(buf, "%d", i);
    (*this) << buf;
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::operator <<(uint16_t ui) {
    char buf[32] = {};
    sprintf(buf, "%d", ui);
    (*this) << buf;
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::operator <<(int16_t i) {
    char buf[32] = {};
    sprintf(buf, "%d", i);
    (*this) << buf;
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::operator <<(uint8_t ui) {
    char buf[32] = {};
    sprintf(buf, "%d", ui);
    (*this) << buf;
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::operator <<(int8_t i) {
    char buf[32] = {};
    sprintf(buf, "%d", i);
    (*this) << buf;
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::operator <<(float f) {
    char buf[32] = {};
    sprintf(buf, "%f", f);
    (*this) << buf;
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::operator <<(double d) {
    char buf[32] = {};
    sprintf(buf, "%lg", d);
    (*this) << buf;
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::AppendQuoted(const char* text) {
    SARE_ASSERT(new_msg_i_ != uint32_t(-1));
    std::string& s = messages_[new_msg_i_];
    s += "\"";
    s += text;
    s += "\"";
    return *this;
}

//-----------------------------------------------------------------------------
Logger& Logger::AppendQuoted(const std::string& text) {
    SARE_ASSERT(new_msg_i_ != uint32_t(-1));
    std::string& s = messages_[new_msg_i_];
    s += "\"";
    s += text;
    s += "\"";
    return *this;
}

//-----------------------------------------------------------------------------
void Logger::Flush() {
    void Log(std::vector<std::string>& messages);
    Log(messages_);
    new_msg_i_ = -1;
    messages_.clear();
}

//-----------------------------------------------------------------------------
void Logger::IncIndent() {
    indent_level_++;
    RewriteLastIndent();
}

//-----------------------------------------------------------------------------
void Logger::DecIndent() {
    if (indent_level_) {
        indent_level_--;
        RewriteLastIndent();
    }
}

//-----------------------------------------------------------------------------
void Logger::ResetIndent() {
    indent_level_ = 0;
}

//-----------------------------------------------------------------------------
void Logger::FixIndent() {
    indent_level_stack_.push_back(indent_level_);
}

//-----------------------------------------------------------------------------
void Logger::RestoreIndent() {
    if (!indent_level_stack_.empty()) {
        indent_level_ = indent_level_stack_.back();
        indent_level_stack_.pop_back();
        RewriteLastIndent();
    }
}

//-----------------------------------------------------------------------------
std::string& Logger::UpdateIndent(bool without_header) {

    if (indent_level_last_ == indent_level_ &&
        header_char_count_last_ == header_char_count_) {
        return without_header ? indent_without_header_ : indent_;
    }

    indent_ = "";
    uint32_t space_count = SARE_LOGGER_INDENT_SPACE_COUNT * indent_level_;
    for (uint32_t i = 0; i < space_count; ++i) {
        indent_ += ' ';
    }
    indent_without_header_ = indent_;
    for (uint32_t i = 0; i < header_char_count_; ++i) {
        indent_ += ' ';
    }
    indent_level_last_ = indent_level_;
    header_char_count_last_ = header_char_count_;
    if (without_header)
        return indent_without_header_;
    return indent_;
}

//-----------------------------------------------------------------------------
void Logger::RewriteLastIndent() {
    if (new_msg_i_ == uint32_t(-1))
        return;
    std::string& s = messages_[new_msg_i_];
    std::string::size_type pos = s.rfind('\n');
    if (s.npos != pos) {
        s.erase(pos);
        (*this) << '\n';
    }
}

} // end of sare
