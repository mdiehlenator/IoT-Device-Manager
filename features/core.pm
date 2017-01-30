package core;

BEGIN {
	$main::functions{core}{device_boot} = core::boot;
	$main::functions{core}{device_startup} = core::startup;
	$main::functions{core}{device_pong} = core::pong;
	$main::functions{core}{device_status} = core::status;

	print "Core loaded.\n";
}

sub	parse_line {
	my($device, $line) = @_;
	my($var, $val);

	($var, $val) = split(/\s*=\s*/, $line);

	$device->{core}->{$var} = $val;
	$device->{$var} = $val;

	if ($var eq "id") {
		#main::see($val, 1);
		$device{id} = $val;
	}

	if ($var eq "type") {
		require "./device_definitions/$val\.pm";
	}
}

sub	boot {
	my($h) = @_;
	my($device);

	$device = device::find_device($h->{from});
	$name = $device->{core}->{name};

	print "Core::boot() called for $h->{from}/$name\n";
        mqtt::publish("$main::config{MQTT_PREFIX}/all/$device->{core}->{name}/boot", 1);
        mqtt::publish("$main::config{MQTT_PREFIX}$main::config{MQTT_SUFFIX}/$device->{core}->{id}/manager/version", 1);
        mqtt::publish("$main::config{MQTT_PREFIX}$main::config{MQTT_SUFFIX}/$device->{core}->{id}/manager/uptime", 1);

	return;
}

sub	startup{
	my($h) = @_;

	print "Core::startup() called because master just restarted.\n";
        mqtt::publish("$main::config{MQTT_PREFIX}/all/master/startup", 1);

	return;
}

sub	pong{
	my($h) = @_;

	print "Core::pong() called\n";

	#$device = device::find_device($h->{from});
	#mqtt::publish("$main::config{MQTT_PREFIX}/all/$device->{core}->{name}/pong/", 1);

	return;
}

sub	status {
	my($h) = @_;
	my($device, $i);

	#print "Core::status() called\n";

	#print "=============================\n";
	foreach $i (keys %$h) {
	#	print "\t$i\t$h->{$i}\n";
	}


	$device = device::find_device($h->{from});

	#print "============================= $device\n";
	foreach $i (keys %$device) {
	#	print "\t$i\t$device->{$i}\n";
	}
	#print "=============================\n";

        mqtt::publish("$main::config{MQTT_PREFIX}/all/$device->{core}->{name}/status/$h->{params}", $h->{message});

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
