package pins;

BEGIN {
	$main::functions{esp8266}{pinmode} = pins::pinmode;
}

sub	parse_config {
	my($fh, $device) = @_;
	my($pin, $pin_number);

        while (<$fh>) {
		chomp($_);

		if ($_ eq "") { next; }

		if (m/^pin(\d+):/) {
			$_ =~ s/^pin(\d+)://;
			$pin_number = $1;
			$pin = parse_line($device, $pin_number, $_);

			$pins{$pin->{name}} = $pin;
		}

	}

	return;
}

sub	parse_line {
	my($device, $pin, $line) = @_;
	my(@terms, $term, $var, $val);
	my(%pin, $name);

	@terms = split(/\s*;\s*/);

	foreach $term (@terms) {
		($var,$val) = split("=", $term);

		#print "Pin: $device / $pin $var = $val\n";

		$pin{$var} = $val;
	}

	return \%pin;
}

return 1;
