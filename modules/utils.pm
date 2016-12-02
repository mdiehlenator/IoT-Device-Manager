package utils;

sub	get_config {
	local(*FILE);

	open FILE, "./config.h";
	while (<FILE>) {
		if ($_ =~ m/#define\s+(\S+)\s+(\S+)/) {
			my(@a) = ($1, $2);
			$a[1] =~ s/"//g;

			$main::config{$a[0]} = $a[1];
		}
	}
}

sub	read_devices {
	local(*DIR);
	my($device);

	opendir DIR, "./devices";
	while (readdir(DIR)) {
		if ($_ eq ".") { next; }
		if ($_ eq "..") { next; }
		if ($_ =~ m/\.swp/) { next; }

		$device = read_device($_);
		add_device($device);
	}
}

sub	read_device {
	my($file) = @_;
	my($line, $device, $var, $val);
	local(*FILE);
	my(%device);

	$device = $file;
	$device =~ s/\.conf//;

	$device{id} = $device;

	print "Reading: ./devices/${file}\n";

	open FILE, "./devices/${file}";
	while (<FILE>) {
		chomp($_);

		if ($_ eq "") { next; }

		if ($_ =~ m/feature/) {
			return \%device;
		}
		
		($var, $val) = split(/\s*=\s*/);

		$device{$var} = $val;
	}

	return \%device;
}

sub	add_device {
	my($device) = @_;

	$main::devices[$main::device_count] = $device;
	$main::device_by_id{$device->{id}} = $main::device_count;
	$main::device_by_name{$device->{name}} = $main::device_count;

	$main::device_count++;
}

return 1;

__DATA__

sub	parse_line {
	my($device, $pin, $line) = @_;
	my(@terms, $term, $var, $val);

	@terms = split(/;/);

	foreach $term (@terms) {
		($var,$val) = split("=", $term);

		print "Pin: $device / $pin $var = $val\n";
		$main::pin{$device}{$pin}{$var} = $val;

		if ($var eq "name") {
			$main::pinnumber{$val} = $pin;
		}
	}
}

return 1;
