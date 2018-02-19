/*
 * timeUtils.h
 *
 * Created: 13.01.2018 15:31:56
 *  Author: Tojwek
 */ 


#ifndef TIMEUTILS_H_
#define TIMEUTILS_H_

#include <stdint.h>

void timeUtils_init();
uint32_t timeUtils_getTickms();
void timeUtils_tic();
uint32_t timeUtils_tocMs();

#endif /* TIMEUTILS_H_ */