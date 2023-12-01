#ifndef SARE_ERROR_H_
#define SARE_ERROR_H_

namespace sare {

//-----------------------------------------------------------------------------

enum ErrorCode {
    kErrorCode_Ok,
    kErrorCode_NotEnoughMemory,
    kErrorCode_NotAvailable,
    kErrorCode_ApiInternal,
    kErrorCode_MalformedData,
    kErrorCode_NoData,
    kErrorCode_FileNotFound
};


//-----------------------------------------------------------------------------
enum ErrorLevel {
    kErrorLevel_Warning,
    kErrorLevel_Error,
    kErrorLevel_Fatal
};

} // end of sare

#endif // SARE_ERROR_H_

