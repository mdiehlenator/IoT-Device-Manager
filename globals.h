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
extern int pin_lastvalue[MAXPINS];
extern int pin_intervals[MAXPINS];
extern int pin_lastpoll[MAXPINS];

extern void init_callback();
extern struct command commands[];
void init_polling();
extern void dummy_poll(int pin);

extern void ping(char *topic, char *message);
extern void pinmode(char *topic, char *message);
extern void digitalwrite(char *topic, char *message);
extern void digitalread(char *topic, char *message);
extern void analogread(char *topic, char *message);
extern void analogwrite(char *topic, char *message);
extern void reboot(char *topic, char *message);
extern void version(char *topic, char *message);
extern void uptime(char *topic, char *message);

