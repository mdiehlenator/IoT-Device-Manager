package core;

BEGIN {
	$main::functions{core}{boot} = core::boot;
	$main::functions{core}{startup} = core::startup;
	$main::functions{core}{pong} = core::pong;
	$main::functions{core}{status} = core::status;

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

sub	pong{
	print "Core::pong() called\n";
	return;
}

sub	status {
	print "Core::status() called\n";
	return;
}

return 1;
