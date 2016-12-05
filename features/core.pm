package core;

BEGIN {
	$main::functions{core}{boot} = core::bootup;
	$main::functions{core}{startup} = core::startup;

	print "Core loaded.\n";
}

sub	bootup {
	print "Core::bootup() called\n";
	return;
}

sub	startup{
	print "Core::startup() called\n";
	return;
}

return 1;
