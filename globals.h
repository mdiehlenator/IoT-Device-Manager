#define MAXPINS (24)

#define VERSIONMAJOR (1)
#define VERSIONMINOR (0)

#define DEBUGGING (1)

#define DEBUG(M,A,B,C,D)
#ifdef DEBUGGING
#define DEBUG(M,A,B,C,D)   Serial.printf(M,A,B,C,D)
#endif


extern char IP[16];
extern char MAC[18];
extern uint8 MAC_NVRAM[7];
extern int mqtt_port;
extern const char* prefix;

extern int mode[4];

extern int pin_mode[MAXPINS];
extern int pin_values[MAXPINS];
extern int pin_intervals[MAXPINS];
extern int pin_lastpoll[MAXPINS];

extern void init_callback();

void init_polling();
extern void do_poll();

extern void ping(char *topic, char *message);
extern void pinmode(char *topic, char *message);
