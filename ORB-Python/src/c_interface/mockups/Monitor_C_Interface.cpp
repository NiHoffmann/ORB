//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

#include "Monitor_C_Interface.h"
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


uint8_t getMonitorKey(){
	return 97;
}

void setMonitorText(int line, const char *str, size_t len){
	/*for(int i=0;i<len;i++)
        printf("%c", str[i]);*/ //this solution is slower but saves on heapspace
    printf("%.*s",(int)len, str);
    fflush(stdout);
}

#ifdef __cplusplus
}
#endif

