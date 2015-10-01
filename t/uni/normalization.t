use strict;
use warnings;
#no warnings 'once';

BEGIN {
    chdir 't' if -d 't';
    @INC = '../lib';
    require './test.pl';
}

skip_all_if_miniperl('miniperl');

skip_all("$^O cannot handle this test") if $^O eq 'dec_osf';

my $file = '../lib/unicore/TestNorm.pl';

skip_all("$file not built") unless -e $file;

do $file;
