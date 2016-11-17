# IoT-Device-Manager

Please note that this is a work-in-progress.  It's not ready for use at this point.  I'm hoping to have a working version soon.  In the mean time, if you'd like to provide assistance or feedback, please feel free.

In the mean time:

This package provides a standard Arduino sketch and a Perl-based server to configure and manage any number of ESP8266's.  Each ESP8266 will be flashed with the same image.  This means that you don't have to keep track of which sketch belongs to a given device.

For example, let's say you had an ESP8266 (device) in the garage, living room, utility room, and green house.  By using the IoT Device Manager, you would be able to burn the same image onto each device.  You'd even be able to keep a stand-by device pre-burned and ready to go in case any of the other devices fail.

All of the pin mode and initial value configuration is managed at the central server.  This means that adding or replacing devices is easy.

Here's how it works:

The Manager server logs into an MQTT message broker and listens for messages.

When a device boots up, it uses the MQTT message broker to announce itself to network.  The Manager, in turn, listens for these announcements.  When a new announcement is received, the Manager responds by sending pin configuration and initial value commands to the device that just booted up.

The device receives these commands and configures it's pins accordingly.

The Manager, in turn, begins to listen for messages that request changes to various pin values.  Those requests would come from other software components, Android clients, or IOS clients.  These clients wouldn't need to know the actual name of the device or pin number.  All they would need is the "user-friendly" name associated with the device and pin.  So, if the underlying topology or devices change, the clients don't need to be updated.

So, from a user's point of view:

A user client would send a "turn the garage lights on" message to the network.  The Manager would calculate which device manages the garage, and which pin manages the lights.  Then, the Manager will send the appropriate command to the device.

In addition to being able to respond to requests from the Manager for pin value status, the devices will also be sending out regular pin value reports.  This will allow for data-logging and trend analysis.

Any software agent that happened to be listening for these pin values status reports would be in an ideal position to perform data logging and such.  Eventually, the Manaager may provide an abstraction for this function.

So, where do I want to go from here:

It would be nice if we had a logging infrastructure that would log the on-going status of various pins.  This would allow a user to do trend analysis or even power-cost estimations.

It would be nice if the Manager (or a distant cousin) could validate pin mode requests against device-specific capabilities.

It would be nice to have a mechanism where various events could be correlated and cause other events to be triggered.  These could be used, for example, to send out an SMS notification when a window-broken event occurs when the "alarm active" event is in effect.  I'll be putting a LOT of thought into that.

