#define VERSIONMAJOR (1)
#define VERSIONMINOR (0)

#define DEBUG(M,A,B,C,D)
#ifdef DEBUGGING
#define DEBUG(M,A,B,C,D)   Serial.printf(M,A,B,C,D)
#endif

extern WiFiClient espclient;
extern char IP[16];
extern char MAC[18];

extern const char prefix[16];
extern const char suffix[16];

extern int wallclock;

