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

	#print "Seen has: ";
	foreach $d (keys %seen) {
	#	print "($d) ";

		if ($d eq "manager") { next; }

		if (($wallclock - $seen{$d}) > 20) {
			$device = device::find_device($d);
			print "X: ($d) was last seen at ($seen{$d}) ($device)\n";
			mqtt::publish("Diehl/all/manager/devicedown/$d", $seen{$d}+0);
		}
	}

	#print "\n";
}

sub	process {
	my ($h) = @_;
	my($from_device, $to_device, $from_id, $to_id);
	my($i);

	#if ($h->{from} ne "") { see($h->{from}, 0); }

	$from_device = device::find_device($h->{from});
	$to_device = device::find_device($h->{to});

	$from_id = 0;
	$to_id = 0;

	if ($from_device ne "NA") {
		if ($from_device->{id} eq $h->{from}) { $from_id = 1;}
	}

	if ($to_device ne "NA") {
		if ($to_device->{id} eq $h->{to}) { $to_id = 1;}
	}

#print "I got (topic=$h->{topic} (from=$h->{from}) (to=$h->{to}) (command=$h->{command}) (params=$h->{params})  (message=$h->{message}) (from_id=$from_id) (to_id=$to_id} (from_device=$from_device) (to_device=$to_device)\n";

	#
	# From here down, order is IMPORTANT.
	#

	# Case 14
	if (($from_id == 1) and ($h->{to} eq "manager")) {
		#print "Processing message from raw device to manager\n";
		process_device_to_manager($h, $from_device);
		see($h->{from});
		return;
	}

	# Case 1,2,3
	if ($from_id == 1) {
		protocol_error($h, "Message from device id, but not to the manager");
		return;
	}

	# Case 15,16
	if ($h->{to} eq "manager") {
		#print "Processing message from client/manager to manager\n";
		process_client_to_device($h, $from_device);
		see($h->{from});
		return;

	}

	# Case 9
	if (($from_id == 0) and ($to_device ne "NA")) {
		return;
	}

	# Case 10,11
	if ($to_device eq "NA") {
		return;
	}

	# Case 4,5,6
	if (($from_device ne "NA") and ($to_device ne "NA")) {
		protocol_error($h, "Message from device name, but not to a client");
		return;
	}

	# Case 7
	if (($from_device eq "NA") and ($to_id == 1)) {
		protocol_error($h, "Message from unknown device, but to a device id");
		return;
	}

	# Case 8
	if (($h->{from} eq "manager") and ($to_device ne "NA")) {
		protocol_error($h, "Message from manager, but to a device name");
		return;
	}

	# Case 13
	if (($from_device eq "NA") and ($to_id == 0)) {
		#print "Processing message from client to device\n";
		process_client_to_device($h, $from_device);
		see($h->{from});
		return;
	}

	print "X: (topic=$h->{topic} (from=$h->{from}) (to=$h->{to}) (command=$h->{command}) (params=$h->{params})  (message=$h->{message}) (from_id=$from_id) (to_id=$to_id} (from_device=$from_device) (to_device=$to_device)\n";
}

sub	process_device_to_manager {
	my($h, $device) = @_;

	#print "Processing device to manager request\n";
	process_request($h, $device, "device_");
}

sub	process_client_to_device {
	my($h, $device) = @_;

	#print "Processing client to device request\n";
	process_request($h, $device, "client_");
}

sub	process_request {
	my($h, $device, $who) = @_;

	### @@@ Do we really use this?
	if ($functions{$device->{type}}{$h->{command}}) {
		&{$functions{$device->{type}}{$h->{command}}}($h);
	}

	foreach $i (keys %$device) {
		if ($functions{$i}{$who . $h->{command}}) {
			&{$functions{$i}{$who . $h->{command}}}($h);
			#return;
		}
	}

	return;
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
	my($d, @a);

	@a = caller();


	if ($force == 1) {
		#print "See (forced) $device from " . join(" ", @a) . "\n";
		$seen{$device} = $wallclock;
		return;
	}

	if ($device eq "manager") {
		#print "See $device from " . join(" ", @a) . "\n";
		$seen{$device} = $wallclock;
		return;
	}

	$d = device::find_device($device);

	if ($d ne "") {
		#print "See $device from " . join(" ", @a) . "\n";
		$seen{$device} = $wallclock;
		return;
	}

	print "Not seeing $device from " . join(" ", @a) . "\n";
}

