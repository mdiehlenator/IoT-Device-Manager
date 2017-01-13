package core;

BEGIN {
	$main::functions{core}{boot} = core::boot;
	$main::functions{core}{startup} = core::startup;
	$main::functions{core}{pong} = core::pong;
	$main::functions{core}{status} = core::status;

	print "Core loaded.\n";
}

sub	boot {
	my($h) = @_;

	print "Core::boot() called for $h->{from}\n";
	##mqtt::publish
	return;
}

sub	startup{
	my($h) = @_;

	print "Core::startup() called because master just restarted.\n";
	return;
}

sub	pong{
	my($h) = @_;

	print "Core::pong() called\n";
	return;
}

sub	status {
	my($h) = @_;

	print "Core::status() called\n";
	return;
}

return 1;

__DATA__

sub	boot {
	my($topic, $device) = @_;
	my($name);

	print "Looking for name for $device and found $main::device{$device}{name}\n";
	$name = $main::device{$device}{name};

	foreach $pin (keys %{$main::pin{$device}}) {

		if ($pin,$main::pin{$device}{$pin}{mode} eq "digitalinput") {
			protocol::pinmode($device, $pin, "digitalinput");
		}

		if ($pin,$main::pin{$device}{$pin}{mode} eq "digitaloutput") {
			protocol::pinmode($device, $pin, "digitaloutput");
			protocol::digitalwrite($device, $pin, $main::pin{$device}{$pin}{initial});
		}

		if ($pin,$main::pin{$device}{$pin}{mode} eq "analoginput") {
			protocol::pinmode($device, $pin, "analoginput");
		}

		if ($pin,$main::pin{$device}{$pin}{mode} eq "analogoutput") {
			protocol::pinmode($device, $pin, "analogoutput");
			protocol::analogwrite($device, $pin, $main::pin{$device}{$pin}{initial});
		}
	}

	print "Boot: $topic, $device($name)\n";
}

return 1;
