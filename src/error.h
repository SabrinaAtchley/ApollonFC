#ifndef APOLLON_FC_SRC_ERROR_H
#define APOLLON_FC_SRC_ERROR_H

#include "peripherals.h"

#define ERROR_MESSAGE(c) CAT(ERROR_MESSAGE_, c)

#define ERROR(c) Serial.println(ERROR_MESSAGE(c)); BUZZER_ERROR_CODE(c)

#endif
