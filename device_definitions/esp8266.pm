package esp8266;

BEGIN {
	print "Device definition for esp8266 has been loaded.\n";

	$main::functions{esp8266}{version} = esp8266::version;

}

sub	version {
	my($h) = @_;

	print "esp8266::version() called.\n";

	if ($h->{from} eq "manager") {
		print "This came from the manager, so we are skipping it.\n";
		return;
	}

	my $device = device::find_device($h->{to});

	$to = $device->{mac};

	mqtt::publish("$main::config{mqtt_prefix}/$to/manager/version", "a");
}

return 1;
