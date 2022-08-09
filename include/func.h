#ifndef _FUNC_H
#define _FUNC_H
int version();
static void Beep(int nFrequence);
void StopBeep();
void signal_callback_handler(int signum);
int checkArgv(char* arg, const char* what);
int playFile(char* finm);
#endif