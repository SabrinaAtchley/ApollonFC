#ifndef APOLLON_FC_SRC_RECEIVERS_H
#define APOLLON_FC_SRC_RECEIVERS_H

/* Includes relevant receiver type based on configuration */
#include "../configuration.h"

#if ENABLED(INPUT_RECEIVER_PPM)
#include "receivers/ppm_receiver.h"

#elif ENABLED(INPUT_RECEIVER_PWM)
#include "receivers/pwm_receiver.h"

#elif ENABLED(INPUT_RECEIVER_SBUS)
#include "receiver/sbus_receiver.h"

#elif ENABLED(INPUT_RECEIVER_IBUS)
#include "receiver/ibus_receiver.h"

#endif

#endif
