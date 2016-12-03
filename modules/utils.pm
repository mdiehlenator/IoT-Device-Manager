package utils;

sub	get_config {
	my($fh);

	$fh = FileHandle->new("./config.h", "r");
	while (<$fh>) {
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
	my($fh, $line, $device, $var, $val);
	my(%device, $feature);

	$device = $file;
	$device =~ s/\.conf//;

	$device{id} = $device;

	print "Reading: ./devices/${file}\n";

	$fh = FileHandle->new("./devices/${file}", "r");
	while (<$fh>) {
		chomp($_);

		if ($_ eq "") { next; }

		if ($_ =~ m/feature\s+(\S+)/) {
			$feature = $1;
			require "./features/${feature}.pm";
			&{$feature . "::parse_config"}($fh, $device);

			$device{$key}->{$o};
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

	read_device_template($device->{type});
}

sub	read_device_template {
	my($type) = @_;

	print "Reading template for $type\n";

}

return 1;
