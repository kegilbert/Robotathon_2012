#ifndef TIME_FUNCTIONS_H
#define TIME_FUNCTIONS_H

extern void resetWatchdogTimer(void);
extern void WatchdogIntHandler(void);
extern void initWatchdog(void);
extern void initTimers(void);
extern void startTimers(void);
extern void initFlasherTimer(void);

#endif
