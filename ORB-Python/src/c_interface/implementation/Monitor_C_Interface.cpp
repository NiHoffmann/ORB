#include "Monitor_C_Interface.h"
#include "AppTask.h"
#include "PythonTask.h"

extern "C" {

    BYTE getMonitorKey(){
        return AppTask::getMonitorKey(nullptr);
    }

    void print(BYTE line, const char *format, ...) {
        va_list args;
        va_start(args, format);
        AppTask::setMonitorText(nullptr, line, format, args);

        va_end(args);
    }

    void setMonitorText(BYTE line, const char *str, size_t len){
        line %= 4;
		print(line, "%.*s", len, str);
    }

}
