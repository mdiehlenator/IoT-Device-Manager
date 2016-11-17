#include "globals.h"

struct command {
  char *name;
  void (*run)(char *topic, char *message);
};

struct command commands[] = {
  {"ping", ping},
  {"pinmode", pinmode},
  {"digitalwrite", digitalwrite},
  {"digitalread", digitalread},
  {"analogread", analogread},
  {"analogwrite", analogwrite},
  {"reboot", reboot},
  {"version", version},
  {"uptime", uptime},

  
};

int command_count = sizeof(commands);

void callback(char* topic, byte* payload, unsigned int length) {
  int i;
  
  strcpy(buffer1, topic+strlen(prefix)+strlen(suffix)+19);

  memcpy(buffer2, payload, length);
  buffer2[length] = '\0';

  DEBUG("I received a message for topic:\n(%s)\n[%s]\n(%s)\n", topic, buffer1, buffer2, "");
  
  for (i=0; i<command_count; i++) {  
    if (strncmp(commands[i].name, buffer1, strlen(commands[i].name)) == 0) {
      DEBUG("Running it from for for (%i)\n", i, "", "","");
      return (commands[i].run)(buffer1,buffer2);
    }
  }

  DEBUG("But I can not match it.  (%s) (%s)\n", buffer1, buffer2, "", "");
}
