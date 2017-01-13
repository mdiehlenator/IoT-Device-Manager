package core;

BEGIN {
	$main::functions{core}{boot} = core::boot;
	$main::functions{core}{startup} = core::startup;

	print "Core loaded.\n";
}

sub	boot {
	print "Core::boot() called\n";
	return;
}

sub	startup{
	print "Core::startup() called\n";
	return;
}

return 1;
