#include "Monitor.h"

uint8_t getMonitorKey(){
	return 97;
}

void setMonitorText(const char *str, size_t len){
	/*for(int i=0;i<len;i++)
        printf("%c", str[i]);*/ //this solution is slower but saves on heapspace


    printf("%.*s", (int)len, str);
}
