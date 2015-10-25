#
# $Id: cow.t,v 1.1 2013/08/29 16:47:39 dankogai Exp $
#
use strict;
use Encode ();
use Test::More;

if (ord "A" == 193 && ord '^' != 95) {
    plan skip_all => "Haven't bothered to port this to EBCDIC non-1047";
}
else {
    plan tests => 2;
}

my %a = (ord "A" == 65)
        ? ( "L\x{c3}\x{a9}on" => "acme" )
        : ( "L\x{8B}\x{4A}on" => "acme" );
my ($k) = ( keys %a );
Encode::_utf8_on($k);
my %h = ( $k => "acme" );
if (ord "A" == 65) {
    is $h{"L\x{e9}on"} => 'acme';
}
else {
    is $h{"L\x{51}on"} => 'acme';
}
($k) = ( keys %h );
Encode::_utf8_off($k);
%a = ( $k => "acme" );
if (ord "A" == 65) {
    is $h{"L\x{e9}on"} => 'acme';
}
else {
    is $h{"L\x{51}on"} => 'acme';
}
# use Devel::Peek;
# Dump(\%h);

