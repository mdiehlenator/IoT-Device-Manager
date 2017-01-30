package device;

$device_count = 0;


sub	read_devices {
	local(*DIR);
	my($device);

	opendir DIR, "./devices";
	while (readdir(DIR)) {
		if ($_ eq ".") { next; }
		if ($_ eq "..") { next; }
		if ($_ =~ m/.+\.swp/) { next; }

		$device = read_device($_);
		add_device($device);
	}
}

sub	read_device {
	my($file) = @_;
	my($fh, $line, $device, $var, $val);
	my(%device, $feature);

	$device = $file;
	$device =~ s/\.conf//;

	$device{id} = $device;

	print "Reading: ./devices/${file}\n";

	main::see($device);

	$fh = FileHandle->new("./devices/${file}", "r");
	while (<$fh>) {
		chomp($_);

		if ($_ eq "") { next; }
		if ($_ =~ m/^#/) { next; }

		if ($_ =~ m/feature\s+(\S+)/) {
			$feature = $1;
			require "./features/${feature}.pm";
			next;
		}

		&{$feature . "::parse_line"}(\%device, $_);
	}

	return \%device;
}

sub	add_device {
	my($device) = @_;

	$devices[$device_count] = $device;
	$device_by_id{$device->{id}} = $device_count;
	$device_by_name{$device->{name}} = $device_count;

	$device_count++;
}

sub	find_device {
	my($which) = @_;

	if (defined $device_by_id{$which}) {
		return $devices[$device_by_id{$which}];
	}

	if (defined $device_by_name{$which}) {
		return $devices[$device_by_name{$which}];
	}

	return "";
}

return 1;
