package protocol;

# @@@ We may be able to reduce this.
$modenumber{"digitalinput"} = 0;
$modenumber{"digitaloutput"} = 1;
$modenumber{"analoginput"} = 2;
$modenumber{"analogoutput"} = 3;

sub     do_connect {
	$mqtt = Net::MQTT::Simple->new("test.mosquitto.org");

	print "Connected\n";

	protocol::startup("", "");

	print "Announced\n";
}

sub	run {
	$mqtt->run(
		"$main::prefix/#" => sub {
		return main::dispatch(@_);
		},
	);
}


sub     publish {
	my($topic, $message) = @_;

	$mqtt->publish($topic, $message);
	print "Publish: ($topic) -> ($message)\n";
}

sub	startup {
	my($topic, $message) = @_;

	publish(${main::prefix} . "/startup", "Broker restart");

	return;
}

sub	pinmode {
	my($device, $pin, $mode) = @_;

	$pin = sprintf("%02s", $pin);
	publish(${main::prefix} . "/$device/pinmode/$pin", $mode);
}

sub	digitalwrite {
	my($device, $pin, $value) = @_;

	$pin = sprintf("%02s", $pin);
	publish(${main::prefix} . "/$device/digitalwrite/$pin", $value);
}

sub	analogwrite {
	my($device, $pin, $value) = @_;

	$pin = sprintf("%02s", $pin);
	publish(${main::prefix} . "/$device/analogwrite/$pin", $value);
}

return 1;
