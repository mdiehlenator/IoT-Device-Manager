#!/usr/bin/perl

use Net::MQTT::Simple;

require "./modules/utils.pm";
require "./modules/protocol.pm";
require "./modules/raw.pm";
require "./modules/device.pm";
require "./modules/pin.pm";

$prefix = utils::get_prefix();
$suffix = utils::get_prefix();
$mqtt_host = utils::get_prefix();
$mqtt_port= utils::get_prefix();
$mqtt_user = utils::get_prefix();
$mqtt_passwd = utils::get_prefix();

print "Using $prefix for message prefixes.\n";
 
utils::read_devices();

protocol::do_connect();

protocol::run();

################################################################

sub	dispatch {
	my($topic, $message) = @_;
	my($f);

	if ($topic eq "") { return 0; }

	$f = $topic;

	$f =~ s/$prefix//e;
	$f =~ s/\//::/g;
	$f =~ s/^:://;

	($item) = split(/\//, $f);

	print "Received message: ($topic = $message): ";

	($p,$board,$pin) = split(/\//, $topic);
	print "X: $p $board $mac{$board} $pin $pinnumber{$pin}";

	$pinnumber = $pinnumber{$pin};

	# /PREFIX/DEVICE/PINNUMBER
	if (defined $pin{$mac{$board}}{$pinnumber}) {
		print "Pin function\n";
		return;
	}

	# /PREFIX/DEVICE/FUNCTION
	if (defined $mac{$board}) {
		print "Device function\n";
		return;
	}

	# PREFIX/DEVICENAME/  @@@ Why do we ignore this?
	if ($device{$f}{name} ne "") {
		print "Ignoring message for ($f) ($device{$f}{$name})\n";
		return;
	}

	# @@@ Why do we ignore this?
	if ($pin{$f}{name} ne "") {
		print "Ignoring message for ($f) ($pin{$f}{$name})\n";
		return;
	}

	# /PREFIX/raw/FUNCTION
	if ($functions{$f} ne "") {
		&{$f}($topic, $message);
		print "Processed\n";
		return;
	}

	# /PREFIX/raw/GARBAGE
	if ($functions{$f} eq "") {
		if ($topic =~ m/${prefix}\/raw/) {
			print "Ignoring raw message\n";
			return;
		}
	}

	print "Extraneous\n";

	return 0;
}

