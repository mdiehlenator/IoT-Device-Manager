#include "globals.h"

struct command {
  char *name;
  void (*run)(char *topic, char *message);
};

struct command commands[] = {
#ifdef FEATURE_CORE
  {"ping", ping},
  {"reboot", reboot},
  {"version", version},
  {"uptime", uptime},
#endif
#ifdef FEATURE_PIN
  {"pinmode", pinmode},
  {"digitalwrite", digitalwrite},
  {"digitalread", digitalread},
  {"analogread", analogread},
  {"analogwrite", analogwrite},
#endif

#ifdef FEATURE_SSD1306
  {"text", text},
  {"clearscreen", clearscreen},
  {"setfont", setfont},
#endif

};


void callback(char* topic, byte* payload, unsigned int length) {
  int i;
  
  i =   strlen(prefix) + strlen(suffix) + 19;  // Need to skip the prefix, the suffix, and the mac.  Then we need to skip the from field to get to the command.
  char *pos = strchr(topic+i, '/');
  strcpy(buffer1, pos+1);

  DEBUG("I calculated a message for topic:\n(%s)\n[%s]\n(%s)\n", topic, buffer1, buffer2, "");

  memcpy(buffer2, payload, length);
  buffer2[length] = '\0';

  DEBUG("I received a message for topic:\n(%s)\n[%s]\n(%s)\n", topic, buffer1, buffer2, "");

  //for (i=0; i<sizeof(commands); i++) { 
  for (i=0; i<12; i++) {  
    DEBUG("Testing it from for for (%i) (%s) == (%s)\n", i, commands[i].name, buffer1,"");

    if (strncmp(commands[i].name, buffer1, strlen(commands[i].name)) == 0) {
      DEBUG("Running it from for for (%i)\n", i, "", "","");
      return (commands[i].run)(buffer1,buffer2);
    }
  }

  DEBUG("But I can not match it.  (%s) (%s)\n", buffer1, buffer2, "", "");
}
