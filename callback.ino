#include "globals.h"

struct command {
  char *name;
  void (*run)(char *topic, char *message);
};

struct command commands[20];

int command_count = 0;

void init_callback() {
  commands[command_count].run = ping;  commands[command_count++].name = "ping";
  commands[command_count].run = pinmode;  commands[command_count++].name = "pinmode";
  commands[command_count].run = digitalwrite;  commands[command_count++].name = "digitalwrite";
  commands[command_count].run = digitalread;  commands[command_count++].name = "digitalread";
  commands[command_count].run = analogread;  commands[command_count++].name = "analogread";
  commands[command_count].run = analogwrite;  commands[command_count++].name = "analogwrite";
  commands[command_count].run = reboot;  commands[command_count++].name = "reboot";
  commands[command_count].run = version;  commands[command_count++].name = "version";
  commands[command_count].run = uptime;  commands[command_count++].name = "uptime";
}

void callback(char* topic, byte* payload, unsigned int length) {
  int i, found;
  
  strcpy(buffer1, topic+strlen(prefix)+strlen(suffix)+19);

  memcpy(buffer2, payload, length);
  buffer2[length] = '\0';

  DEBUG("I received a message for topic:\n(%s)\n[%s]\n(%s)\n", topic, buffer1, buffer2, "");
  
  for (i=0; i<command_count; i++) {  
    if (strncmp(commands[i].name, buffer1, strlen(commands[i].name)) == 0) {
      found = i;
      i=100000;
DEBUG("Running it from for for (%i)\n", found, "", "","");
      return (commands[i].run)(buffer1,buffer2);
    }
  }
  
  DEBUG("Running it from switch for (%i)\n", found, "", "","");

  switch (found) {
    case 0: return ping(buffer1, buffer2);
    case 1: return pinmode(buffer1, buffer2);
    case 2: return digitalwrite(buffer1, buffer2);
    case 3: return digitalread(buffer1, buffer2);
    case 4: return analogread(buffer1, buffer2);
    case 5: return analogwrite(buffer1, buffer2);
    case 6: return reboot(buffer1, buffer2);
    case 7: return version(buffer1, buffer2);
    case 8: return uptime(buffer1, buffer2);    
  }

  DEBUG("But I can not match it.  (%s) (%s)\n", buffer1, buffer2, "", "");
}
