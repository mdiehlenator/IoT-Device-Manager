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

print "Registered $device::device_count devices.\n";

print "==============================================================\n";

mqtt::do_connect();

while (1) {
	AnyEvent->condvar->recv; # wake up current and all future recv's
}

################################################################

sub	process {
	my ($h) = @_;

	print "I got (topic = $h->{topic} (from = $h->{from}) (to = $h->{to}) (command = $h->{command}) (params = $h->{params})  (message = $h->{message})\n";
	
	# Let's check the core functions first.
	if ($functions{core}{$h->{command}}) {
		&{$functions{core}{$h->{command}}}($h);
		return;
	}

	$from_device = device::find_device($h->{from});
	$to_device = device::find_device($h->{to});

	if ($from_device->{id} eq $h->{from}) { $from_id = 1;}
	if ($to_device->{id} eq $h->{to}) { $to_id = 1;}

	if (($from_id == 1) && !($h->{to} ne "manager")) { protocol_error($h); }	# No one but the manager should be receiving messages from devices directly.
	if (($to_id ==1) && $h->{from} ne "managaer") { protocol_error($h); }		# No one but the manager should be sending messages to devices directly.

	# Then, let's check the platform-specific functions.
	if (($h->{to} eq "manager") and (device::find_device($h->{from}) ne "")) {
		my $device = device::find_device($h->{from});
		print "The device, $h->{from} is a $device->{type} named $device->{name}\n";

		if ($functions{$device->{type}}{$h->{command}}) {
		&{$functions{$device->{type}}{$h->{command}}}($h);
	}

	}

	# Finally, let's try the device-specific (feature?) functions.
}

sub	protocol_error {
	my($h) = @_;

	print "Protocol Error\n";
}

