package core;

BEGIN {
	$main::functions{core}{boot} = core::bootup;

	print "Core loaded.\n";
}

sub	bootup {
	print "Core::bootup() called\n";
	return;
}

return 1;
