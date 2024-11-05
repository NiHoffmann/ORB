//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

#ifndef MONITOR_MOCKUP_H
#define MONITOR_MOCKUP_H
#include "stdint.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif


enum Taste_enum
    {
      Keine_Taste = 0, //!< Es ist keine Taste gedrückt

      Taste_A1  =  1,  //!<.
      Taste_A2  =  2,  //!<.
      Taste_A3  =  3,  //!<.
      Taste_A4  =  4,  //!<.
      Taste_A5  =  5,  //!<.
      Taste_A6  =  6,  //!<.
      Taste_A7  =  7,  //!<.
      Taste_A8  =  8,  //!<.

      Taste_B1  =  9,  //!<.
      Taste_B2  = 10,  //!<.
      Taste_B3  = 11,  //!<.

      Taste_B4  = 12,  //!<.
      Taste_B5  = 13,  //!<.
      Taste_B6  = 14,  //!<.

      Taste_B7  = 15,  //!<.
      Taste_B8  = 16,  //!<.
      Taste_B9  = 17,  //!<.

      Taste_B10 = 18,  //!<.
      Taste_B11 = 19,  //!<.
      Taste_B12 = 20,  //!<.

      Taste_C1  = 21,  //!<.

    };

uint8_t getMonitorKey();

void setMonitorText(uint8_t line, const char *str, size_t len);

#ifdef __cplusplus
}
#endif
#endif
