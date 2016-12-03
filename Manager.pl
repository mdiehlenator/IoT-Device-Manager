#!/usr/bin/perl
use EV;
use AnyEvent;
use Net::MQTT::Simple;
use FileHandle;

require "./modules/utils.pm";
require "./modules/mqtt.pm";
require "./modules/raw.pm";
require "./modules/device.pm";

$device_count = 0;

STDOUT->autoflush;

utils::get_config();

require "./features/core.pm";

device::read_devices();

print "Registered $device_count devices.\n";

print "==============================================================\n";

mqtt::do_connect();

while (1) {
	AnyEvent->condvar->recv; # wake up current and all future recv's
}

################################################################

sub	process {
	my ($h) = @_;

	print "I got (topic = $h->{topic} (from = $h->{from}) (to = $h->{to}) (command = $h->{command}) (params = $h->{params})  (message = $h->{message})\n";
	
	if ($h->{from} eq "manager") {
		return;
	}

	if ($functions{core}{$h->{command}}) {
		&{$functions{core}{$h->{command}}}($h);
		return;
	}
}

