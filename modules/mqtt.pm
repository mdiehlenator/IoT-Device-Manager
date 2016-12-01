package mqtt;

use AnyEvent::MQTT;

# @@@ We may be able to reduce this.
$modenumber{"digitalinput"} = 0;
$modenumber{"digitaloutput"} = 1;
$modenumber{"analoginput"} = 2;
$modenumber{"analogoutput"} = 3;

sub     do_connect {
	$mqtt = AnyEvent::MQTT->new("host" => $main::config{MQTT_SERVER}, "port" => $main::config{MQTT_PORT}, "user_name" => $main::config{MQTT_USER}, "password" => $main::config{MQTT_PASSWORD});

	print "Connected\n";

	mqtt::startup("", "");

	print "Announced\n";
}

sub     publish {
	my($topic, $message) = @_;
	my($cv);

	$cv = $mqtt->publish("topic" => $topic, "message" => $message);
	$cv->recv;

	print "Publish: ($topic) -> ($message)\n";
}

sub	startup {
	my($topic, $message) = @_;

	publish(${main::config{MQTT_PREFIX}} . "/startup", "Broker restart");

	return;
}

return 1;
