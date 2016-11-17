package raw;

BEGIN {
	$main::functions{"raw::boot"} = "1";
}

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
