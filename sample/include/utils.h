#ifndef UTILS_H
#define UTILS_H

extern int run;
void button_cb();
void print_mac(void *mac);
void init();
void flip_screen();
#define error(...) \
	printf(__VA_ARGS__); \
	while (run) { \
		WPAD_ScanPads(); \
		if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) run = 0; \
		flip_screen(); \
	} \
	goto end;

#endif
