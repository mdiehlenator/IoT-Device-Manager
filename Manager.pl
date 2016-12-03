#!/usr/bin/perl
use EV;
use AnyEvent;
use Net::MQTT::Simple;
use FileHandle;

require "./modules/utils.pm";
require "./modules/mqtt.pm";
require "./modules/raw.pm";
require "./modules/device.pm";

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
	
	# I _THINK_ we can ignore messages that we send...
	if ($h->{from} eq "manager") {
		return;
	}

	# Let's check the core functions first.
	if ($functions{core}{$h->{command}}) {
		&{$functions{core}{$h->{command}}}($h);
		return;
	}

	# Then, let's check the platform-specific functions.
	if (($h->{to} eq "manager") and (device::find_device($h->{from}) ne "")) {
		my $device = device::find_device($h->{from});
		print "The device, $h->{from} is a $device->{type} named $device->{name}\n";
		return;
	}

	# Finally, let's try the device-specific (feature?) functions.
}

