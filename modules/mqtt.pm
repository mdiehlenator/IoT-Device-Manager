package mqtt;

use AnyEvent::MQTT;

sub     do_connect {
	$mqtt = AnyEvent::MQTT->new("host" => $main::config{MQTT_SERVER}, "port" => $main::config{MQTT_PORT}, 
		"user_name" => $main::config{MQTT_USER}, "password" => $main::config{MQTT_PASSWORD});

	print "Connected\n";

	startup();

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
	my($t, $cv);

	### @@@ Man, this needs to be separated.
	print "Subscribing to owntracks/#\n";
	$cv = $mqtt->subscribe("topic" => "owntracks/#", "callback" => sub {return &callback; } );
	$cv->recv;

	$t = $main::config{MQTT_PREFIX} . "/#";
	print "Subscribing to $t\n";
	$cv = $mqtt->subscribe("topic" => $t, "callback" => sub {return &callback; } );
	$cv->recv;

	if ($main::config{MQTT_SUFFIX} ne "") {
		$t = $main::config{MQTT_PREFIX} . $main::config{MQTT_SUFFIX} . "/#";
		print "Subscribing to $t\n";
		$cv = $mqtt->subscribe("topic" => $t, "callback" => sub {return &callback; } );
		$cv->recv;
	}

	$t = $main::config{MQTT_PREFIX} . "/";
	publish($t . "all/manager/startup", "Broker restart");

	return;
}


sub	callback {
	my ($topic, $message) = @_;
	my(%t, @a);

	my @a = split("/", $topic);

	$t{message} = $message;
	$t{topic} = $topic;

	$t{root} = $a[0];

	if ($a[1] =~ m/[a-f0-9]{2}:[a-f0-9]{2}:/) {
		print "This must have been sent to a device.\n";
		if ($a[2] eq "manager") {
			$t{to} = $a[1];
			$t{from} = "manager";
			$t{command} = $a[3];

			$t{params} = join(",", $a[4],$a[5]);
		}
	} else {
		if ($a[1] eq "manager") { 
			print "This was sent to the manager\n";
			$t{to} = "manager";
			$t{from} = $a[2];
			$t{command} = $a[3];
			$t{params} = join(",", $a[4]);
		} else {
			print "This was something else.\n";
			$t{to} = $a[1];
			$t{from} = $a[2];
			$t{command} = $a[3];
			$t{params} = join(",", $a[4]);
		}
	}

	main::process(\%t);
}

return 1;
