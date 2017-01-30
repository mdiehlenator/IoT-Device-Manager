package pins;

BEGIN {
	$main::functions{pins}{device_boot} = pins::boot;
	$main::functions{pins}{device_set} = pins::set;

}

sub	set {
	my($h) = @_;

	print "XXX: pins::set() called for $h->{from} to $h->{to}\n";

}

sub	boot {
	my($h) = @_;
        my($device);

	print "X: pins::boot() called for $h->{from}\n";

	$device = device::find_device($h->{from});

	$pins = $device->{pins};

	foreach $pin (keys %$pins) {
		print "X: $pin -> $pins->{$pin}->{name}\n";
		mqtt::publish("$main::config{MQTT_PREFIX}$main::config{MQTT_SUFFIX}/$device->{core}->{id}/manager/pinmode/$pin", $pins->{$pin}->{mode});
		mqtt::publish("$main::config{MQTT_PREFIX}$main::config{MQTT_SUFFIX}/$device->{core}->{id}/manager/digitalwrite/$pin", $pins->{$pin}->{initial});
	}

}

sub	parse_line {
        my($device, $line) = @_;
	my(@terms, $term, $var, $val);
	my(%pin, $name);
	my($number, $config);

	($number, $config) = split(":");

	@terms = split(/\s*;\s*/, $config);

	foreach $term (@terms) {
		($var,$val) = split("=", $term);

		print "Pin: $device->{id} / $number $var = $val\n";

		$pin->{$var} = $val;
	}

	$device->{pins}->{$number} = $pin;
}

return 1;
