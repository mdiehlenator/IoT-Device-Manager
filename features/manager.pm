package manager;

sub	parse_line {
	my($device, $line) = @_;
	my($var, $val);

	($var, $val) = split(/\s*=\s*/, $line);

	$device->{core}->{$var} = $val;
	$device->{$var} = $val;

	if ($var eq "id") {
		main::see($val, 1);
		$device{id} = $val;
	}
}

return 1;
