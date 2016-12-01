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

	opendir DIR, "./devices";
	while (readdir(DIR)) {
		if ($_ eq ".") { next; }
		if ($_ eq "..") { next; }
		if ($_ =~ m/\.swp/) { next; }

		read_device($_);
	}
}

sub	read_device {
	my($file) = @_;
	my($line, $device, $var, $val);
	local(*FILE);

	$device = $file;
	$device =~ s/\.conf//;

	print "Reading: ./devices/${file}\n";

	open FILE, "./devices/${file}";
	while (<FILE>) {
		chomp($_);

		if ($_ eq "") { next; }
		
		if (m/^pin(\d+):/) {
			$_ =~ s/^pin(\d+)://;
			parse_line($device, $1, $_);
		} else {
			($var, $val) = split(/\s*=\s*/);
			$main::device{$device}{$var} = $val;
			print "\tSetting: ($var) = ($val)\n";

			if ($var eq "name") {
				$main::mac{$val} = $device;
			}
		}
	}
}

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
