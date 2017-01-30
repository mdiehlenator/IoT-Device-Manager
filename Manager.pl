#!/usr/bin/perl
use EV;
use AnyEvent;
use Net::MQTT::Simple;
use FileHandle;

require "./modules/mqtt.pm";
require "./modules/device.pm";

STDOUT->autoflush;

get_config();

device::read_devices();

print "Registered $device::device_count devices.\n";

print "==============================================================\n";

mqtt::do_connect();

$wallclock = 0;

while (1) {
	my $t = AnyEvent->timer(interval => 1, cb =>  
		sub {
			return &main::wallclock();
		}
	); 

	AnyEvent->condvar->recv; # wake up current and all future recv's
}

################################################################


sub	wallclock {
	my($d);
	my($device);

	#print "Wallclock: $wallclock\n";
	see("manager", 1);
	$wallclock++;

	foreach $d (keys %seen) {

		if ($d eq "manager") { next; }

		if (($wallclock - $seen{$d}) > 20) {
			$device = device::find_device($d);
			print "X: $d{from} was last seen at " . $seen{$d}+0 . " ($device)\n";
			mqtt::publish("Diehl/all/manager/devicedown/$d/", $wallclock-$seen{$d});
		}
	}
}

sub	process {
	my ($h) = @_;

	if ($h->{from} ne "") { see($h->{from}, 0); }

	$from_device = device::find_device($h->{from});
	$to_device = device::find_device($h->{to});

	if ($from_device->{id} eq $h->{from}) { $from_id = 1;}
	if ($to_device->{id} eq $h->{to}) { $to_id = 1;}

	print "I got (topic = $h->{topic} (from = $h->{from}) (to = $h->{to}) (command = $h->{command}) (params = $h->{params})  (message = $h->{message}) (from_id = $from_id) (to_id = $to_id}\n";

#I got (topic = Diehl-raw/60:01:94:0e:87:14/tst/digitalwrite/16 (from = ) (to = ) (command = ) (params = )  (message = 1) (from_id = 1) (to_id = }

	if (($from_id == 1) and ($to_id == 0) and ($h->{from} ne "manager") and ($h->{to} ne "manager")) {
		protocol_error($h, "Error: Someone sent a message directly to a device."); # No one but the manager should be sending messages to devices directly.
		return;
	}


	$device = device::find_device($h->{from});

	if (($h->{to} eq "manager") and ($from_device ne "")) {

### @@@ Do we really use this?
		if ($functions{$from_device->{type}}{$h->{command}}) {
			&{$functions{$from_device->{type}}{$h->{command}}}($h);
		}

		foreach $i (keys %$from_device) {
			if ($functions{$i}{"device_" . $h->{command}}) {
				&{$functions{$i}{"device_" . $h->{command}}}($h);
				#return;
			}
		}

		return;
	}
}

sub	protocol_error {
	my($h, $msg) = @_;
	my($a);

	$a = stringify_h($h);

	print "Protocol: ($a) \"$msg\"\n";

	mqtt::publish("Diehl/all/manager/protocol/", "($a) \"$msg\"");
}

sub	stringify_h {
	my($h) = @_;
	my($a, $i);

	foreach $i (sort keys %$h) {
		$a .= "$i->$h->{$i} ";
	}

	return $a;
}

sub	get_config {
	my($fh);

	$fh = FileHandle->new("./config.h", "r");
	while (<$fh>) {
		if ($_ =~ m/#define\s+(\S+)\s+(\S+)/) {
			my(@a) = ($1, $2);
			$a[1] =~ s/"//g;

			$main::config{$a[0]} = $a[1];
		}
	}
}

sub	see {
	my($device, $force) = @_;
	my($d);

	if ($force == 1) {
		$seen{$device} = $wallclock;
		#print ("I see $device.\n");
		return;
	}

	if ($device eq "manager") {
		$seen{$device} = $wallclock;
		#print ("I see $device.\n");
		return;
	}

	$d = device::find_device($device);

	if ($d ne "") {
		#print ("I see $device.\n");
		$seen{$device} = $wallclock;
		return;
	}

	print "Not seeing unknown device, ($device)\n";
}

