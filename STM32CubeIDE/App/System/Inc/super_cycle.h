#ifndef SUPER_CYCLE_H
#define SUPER_CYCLE_H

#include <stdint.h>

void     SuperCycle_Init(void);
void     SuperCycle_Run(void);              /* один проход, никогда не блокирует */

uint32_t SuperCycle_GetCycleCount(void);    /* сколько прошло 6-с циклов        */
uint32_t SuperCycle_GetMaxLoopUs(void);     /* худшая длительность прохода, мкс */

#endif /* SUPER_CYCLE_H */
