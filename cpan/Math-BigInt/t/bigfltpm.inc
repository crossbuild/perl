#include this file into another test for subclass testing...

is ($class->config()->{lib},$CL);

use strict;

my $z;

while (<DATA>)
  {
  $_ =~ s/[\n\r]//g;	# remove newlines
  $_ =~ s/#.*$//;	# remove comments
  $_ =~ s/\s+$//;	# trailing spaces
  next if /^$/;		# skip empty lines & comments
  if (s/^&//)
    {
    $f = $_;
    }
  elsif (/^\$/)
    {
    $setup = $_; $setup =~ s/\$/\$${class}::/g;	# round_mode, div_scale
    #print "\$setup== $setup\n";
    }
  else
    {
    if (m|^(.*?):(/.+)$|)
      {
      $ans = $2;
      @args = split(/:/,$1,99);
      }
    else
      {
      @args = split(/:/,$_,99); $ans = pop(@args);
      }
    $try = "\$x = $class->new(\"$args[0]\");";
    if ($f eq "fnorm")
      {
        $try .= "\$x;";
      } elsif ($f eq "finf") {
        $try .= "\$x->finf('$args[1]');";
      } elsif ($f eq "is_inf") {
        $try .= "\$x->is_inf('$args[1]');"; 
      } elsif ($f eq "fone") {
        $try .= "\$x->bone('$args[1]');";
      } elsif ($f eq "fstr") {
        $try .= "\$x->accuracy($args[1]); \$x->precision($args[2]);";
        $try .= '$x->fstr();';
      } elsif ($f eq "parts") {
        # ->bstr() to see if an object is returned
        $try .= '($a,$b) = $x->parts(); $a = $a->bstr(); $b = $b->bstr();';
        $try .= '"$a $b";';
      } elsif ($f eq "exponent") {
        # ->bstr() to see if an object is returned
        $try .= '$x->exponent()->bstr();';
      } elsif ($f eq "mantissa") {
        # ->bstr() to see if an object is returned
        $try .= '$x->mantissa()->bstr();';
      } elsif ($f =~ /^(numify|length|as_number|as_hex|as_bin)$/) {
        $try .= "\$x->$f();";
      # some unary ops (test the fxxx form, since that is done by AUTOLOAD)
      } elsif ($f =~ /^f(nan|sstr|neg|floor|ceil|int|abs)$/) {
        $try .= "\$x->f$1();";
      # overloaded functions
      } elsif ($f =~ /^(log|exp|sin|cos|atan2|int|neg|abs|sqrt)$/) {
        $try .= "\$x = $f(\$x);";
      # some is_xxx test function	
      } elsif ($f =~ /^is_(zero|one|negative|positive|odd|even|nan|int)$/) {
        $try .= "\$x->$f();";
      } elsif ($f eq "bpi") {
        $try .= '$class->bpi($x);';
      } elsif ($f eq "finc") {
        $try .= '++$x;';
      } elsif ($f eq "fdec") {
        $try .= '--$x;';
      }elsif ($f eq "fround") {
        $try .= "$setup; \$x->fround($args[1]);";
      } elsif ($f eq "ffround") {
        $try .= "$setup; \$x->ffround($args[1]);";
      } elsif ($f eq "fsqrt") {
        $try .= "$setup; \$x->fsqrt();";
      } elsif ($f eq "ffac") {
        $try .= "$setup; \$x->ffac();";
      } elsif ($f eq "flog") {
        if (defined $args[1] && $args[1] ne '')
          {
          $try .= "\$y = $class->new($args[1]);";
          $try .= "$setup; \$x->flog(\$y);";
          }
        else
          {
          $try .= "$setup; \$x->flog();";
          }
      }
    else
      {
      $try .= "\$y = $class->new(\"$args[1]\");";
      
      if ($f eq "bgcd")
        {
        if (defined $args[2])
          {
          $try .= " \$z = $class->new(\"$args[2]\"); ";
          }
        $try .= "$class\::bgcd(\$x, \$y";
        $try .= ", \$z" if (defined $args[2]);
        $try .= " );";
        }
      elsif ($f eq "blcm")
        {
        if (defined $args[2])
          {
          $try .= " \$z = $class->new(\"$args[2]\"); ";
          }
        $try .= "$class\::blcm(\$x, \$y";
        $try .= ", \$z" if (defined $args[2]);
        $try .= " );";
      } elsif ($f eq "fcmp") {
        $try .= '$x->fcmp($y);';
      } elsif ($f eq "facmp") {
        $try .= '$x->facmp($y);';
      } elsif ($f eq "fpow") {
        $try .= '$x ** $y;';
      } elsif ($f eq "bnok") {
        $try .= '$x->bnok($y);';
      } elsif ($f eq "bcos") {
        $try .= '$x->bcos($y);';
      } elsif ($f eq "bsin") {
        $try .= '$x->bsin($y);';
      } elsif ($f eq "batan") {
        $try .= '$x->batan($y);';
      } elsif ($f eq "froot") {
        $try .= "$setup; \$x->froot(\$y);";
      } elsif ($f eq "fadd") {
        $try .= '$x + $y;';
      } elsif ($f eq "fsub") {
        $try .= '$x - $y;';
      } elsif ($f eq "fmul") {
        $try .= '$x * $y;';
      } elsif ($f eq "fdiv") {
        $try .= "$setup; \$x / \$y;";
      } elsif ($f eq "fdiv-list") {
        $try .= "$setup; join(',',\$x->fdiv(\$y));";
      } elsif ($f eq "frsft") {
        $try .= '$x >> $y;';
      } elsif ($f eq "flsft") {
        $try .= '$x << $y;';
      } elsif ($f eq "fmod") {
        $try .= '$x % $y;';
       } else {
       # Functions with three arguments
       $try .= "\$z = $class->new(\"$args[2]\");";

       if( $f eq "bmodpow") {
          $try .= '$x->bmodpow($y,$z);';
         } elsif ($f eq "bmuladd"){
          $try .= '$x->bmuladd($y,$z);';
         } elsif ($f eq "batan2"){
          $try .= '$x->batan2($y,$z);';
         } else { warn "Unknown op '$f'"; }
       }
    }
    # print "# Trying: '$try'\n";
    $ans1 = eval $try;
    print "# Error: $@\n" if $@;
    if ($ans =~ m|^/(.*)$|)
      {
      my $pat = $1;
      if ($ans1 =~ /$pat/)
        {
        is (1,1);
        }
      else
        {
        print "# '$try' expected: /$pat/ got: '$ans1'\n" if !is (1,0);
        }
      }
    else
      {
      if ($ans eq "")
        {
	  is ($ans1, undef);
        }
      else
        {
        print "# Tried: '$try'\n" if !is ($ans1, $ans);
        if (ref($ans1) eq "$class")
	  {
	  # float numbers are normalized (for now), so mantissa shouldn't have
	  # trailing zeros
	  #print $ans1->_trailing_zeros(),"\n";
          print "# Has trailing zeros after '$try'\n"
	   if !is ($CL->_zeros( $ans1->{_m}), 0);
	  }
        }
      } # end pattern or string
    }
  } # end while

# check whether $class->new( Math::BigInt->new()) destroys it 
# ($y == 12 in this case)
$x = Math::BigInt->new(1200); $y = $class->new($x);
is ($y,1200); is ($x,1200);

###############################################################################
# Really huge, big, ultra-mega-biggy-monster exponents
# Technically, the exponents should not be limited (they are BigInts), but
# practically there are a few places were they are limited to a Perl scalar.
# This is sometimes for speed, sometimes because otherwise the number wouldn't
# fit into your memory (just think of 1e123456789012345678901234567890 + 1!)
# anyway. We don't test everything here, but let's make sure it just basically
# works.

my $monster = '1e1234567890123456789012345678901234567890';

# new and exponent
is ($class->new($monster)->bsstr(), 
		'1e+1234567890123456789012345678901234567890');
is ($class->new($monster)->exponent(), 
		'1234567890123456789012345678901234567890');
# cmp
is ($class->new($monster) > 0,1);

# sub/mul 
is ($class->new($monster)->bsub( $monster),0);
is ($class->new($monster)->bmul(2)->bsstr(),
                '2e+1234567890123456789012345678901234567890');

# mantissa
$monster = '1234567890123456789012345678901234567890e2';
is ($class->new($monster)->mantissa(), 
		'123456789012345678901234567890123456789');

###############################################################################
# zero,inf,one,nan

$x = $class->new(2); $x->fzero(); is ($x->{_a}, undef); is ($x->{_p}, undef);
$x = $class->new(2); $x->finf();  is ($x->{_a}, undef); is ($x->{_p}, undef);
$x = $class->new(2); $x->fone();  is ($x->{_a}, undef); is ($x->{_p}, undef);
$x = $class->new(2); $x->fnan();  is ($x->{_a}, undef); is ($x->{_p}, undef);

###############################################################################
# bone/binf etc as plain calls (Lite failed them)

is ($class->fzero(),0);
is ($class->fone(),1);
is ($class->fone('+'),1);
is ($class->fone('-'),-1);
is ($class->fnan(),'NaN');
is ($class->finf(),'inf');
is ($class->finf('+'),'inf');
is ($class->finf('-'),'-inf');
is ($class->finf('-inf'),'-inf');
        
$class->accuracy(undef); $class->precision(undef);	# reset

###############################################################################
# bug in bsstr()/numify() showed up in after-rounding in bdiv()

$x = $class->new('0.008'); $y = $class->new(2);
$x->bdiv(3,$y);
is ($x,'0.0027');

###############################################################################
# Verify that numify() returns a normalized value, and underflows and
# overflows when given "extreme" values.

like($class->new("12345e67")->numify(), qr/^1\.2345e\+?0*71$/);
like($class->new("1e-9999")->numify(), qr/^\+?0$/);              # underflow
unlike($class->new("1e9999")->numify(), qr/^1(\.0*)?e\+?9+$/);   # overflow

###############################################################################
# Check numify on non-finite objects.

{
    my $inf = 1e99 ** 1e99;
    my $nan = $inf - $inf;
    is($class -> binf("+") -> numify(),  $inf, "numify of +Inf");
    is($class -> binf("-") -> numify(), -$inf, "numify of -Inf");
    is($class -> bnan()    -> numify(),  $nan, "numify of NaN");
}

###############################################################################
# fsqrt() with set global A/P or A/P enabled on $x, also a test whether fsqrt()
# correctly modifies $x


$x = $class->new(12); $class->precision(-2); $x->fsqrt(); is ($x,'3.46');

$class->precision(undef);
$x = $class->new(12); $class->precision(0); $x->fsqrt(); is ($x,'3');

$class->precision(-3); $x = $class->new(12); $x->fsqrt(); is ($x,'3.464');

{
  no strict 'refs';
  # A and P set => NaN
  ${${class}.'::accuracy'} = 4; $x = $class->new(12);
  $x->fsqrt(3); is ($x,'NaN');
  # supplied arg overrides set global
  $class->precision(undef); $x = $class->new(12); $x->fsqrt(3); is ($x,'3.46');
  $class->accuracy(undef); $class->precision(undef); # reset for further tests
}

#############################################################################
# can we call objectify (broken until v1.52)

{
  no strict;
  $try = 
    '@args' . " = $class" . "::objectify(2,$class,4,5);".'join(" ",@args);';
  $ans = eval $try;
  is ($ans,"$class 4 5");
}

#############################################################################
# is_one('-') (broken until v1.64)

is ($class->new(-1)->is_one(),0);
is ($class->new(-1)->is_one('-'),1);

#############################################################################
# bug 1/0.5 leaving 2e-0 instead of 2e0

is ($class->new(1)->fdiv('0.5')->bsstr(),'2e+0');

###############################################################################
# [perl #30609] bug with $x -= $x not being 0, but 2*$x

$x = $class->new(3);  $x -= $x; is ($x, 0);
$x = $class->new(-3); $x -= $x; is ($x, 0);
$x = $class->new(3);  $x += $x; is ($x, 6);
$x = $class->new(-3); $x += $x; is ($x, -6);

$x = $class->new('NaN'); $x -= $x; is ($x->is_nan(), 1);
$x = $class->new('inf'); $x -= $x; is ($x->is_nan(), 1);
$x = $class->new('-inf'); $x -= $x; is ($x->is_nan(), 1);

$x = $class->new('NaN'); $x += $x; is ($x->is_nan(), 1);
$x = $class->new('inf'); $x += $x; is ($x->is_inf(), 1);
$x = $class->new('-inf'); $x += $x; is ($x->is_inf('-'), 1);

$x = $class->new('3.14');  $x -= $x; is ($x, 0);
$x = $class->new('-3.14'); $x -= $x; is ($x, 0);
$x = $class->new('3.14');  $x += $x; is ($x, '6.28');
$x = $class->new('-3.14'); $x += $x; is ($x, '-6.28');

$x = $class->new('3.14');  $x *= $x; is ($x, '9.8596');
$x = $class->new('-3.14'); $x *= $x; is ($x, '9.8596');
$x = $class->new('3.14');  $x /= $x; is ($x, '1');
$x = $class->new('-3.14'); $x /= $x; is ($x, '1');
$x = $class->new('3.14');  $x %= $x; is ($x, '0');
$x = $class->new('-3.14'); $x %= $x; is ($x, '0');

###############################################################################
# the following two were reported by "kenny" via hotmail.com:

#perl -MMath::BigFloat -wle 'print Math::BigFloat->new(0)->bpow(".1")'
#Use of uninitialized value in numeric le (<=) at BigFloat.pm line 1851.

$x = $class->new(0); $y = $class->new('0.1');
is ($x ** $y, 0, 'no warnings and zero result');

#perl -MMath::BigFloat -lwe 'print Math::BigFloat->new(".222222222222222222222222222222222222222222")->bceil()'
#Use of uninitialized value in numeric le (<=) at BigFloat.pm line 1851.

$x = $class->new(".222222222222222222222222222222222222222222"); 
is ($x->bceil(), 1, 'no warnings and one as result');

###############################################################################
# test **=, <<=, >>=

# ((2^148)-1)/17
$x = $class->new(2); $x **= 148; $x++; $x->bdiv(17, 60)->bfloor(); $x->accuracy(undef);
is ($x,"20988936657440586486151264256610222593863921");
is ($x->length(),length "20988936657440586486151264256610222593863921");

$x = $class->new('2');
my $y = $class->new('18');
is ($x <<= $y, 2 << 18);
is ($x, 2 << 18);
is ($x >>= $y, 2);
is ($x, 2);

$x = $class->new('2');
$y = $class->new('18.2');
$x <<= $y;		# 2 * (2 ** 18.2);

is ($x->copy()->bfround(-9), '602248.763144685');
is ($x >>= $y, 2);	# 2 * (2 ** 18.2) / (2 ** 18.2) => 2
is ($x, 2);

1; # all done

__DATA__
&bgcd
inf:12:NaN
-inf:12:NaN
12:inf:NaN
12:-inf:NaN
inf:inf:NaN
inf:-inf:NaN
-inf:-inf:NaN
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+0:+0:0
+0:+1:1
+1:+0:1
+1:+1:1
+2:+3:1
+3:+2:1
-3:+2:1
-3:-2:1
-144:-60:12
144:-60:12
144:60:12
100:625:25
4096:81:1
1034:804:2
27:90:56:1
27:90:54:9
&blcm
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+0:+0:NaN
+1:+0:0
+0:+1:0
+27:+90:270
+1034:+804:415668
$div_scale = 40;
&bcos
1.2:10:0.3623577545
2.4:12:-0.737393715541
0:10:1
0:20:1
1:10:0.5403023059
1:12:0.540302305868
&bsin
1:10:0.8414709848
0:10:0
0:20:0
2.1:12:0.863209366649
1.2:13:0.9320390859672
0.2:13:0.1986693307951
3.2:12:-0.0583741434276
&batan
NaN:10:NaN
inf:14:1.5707963267949
-inf:14:-1.5707963267949
0.2:13:0.1973955598499
0.2:14:0.19739555984988
0:10:0
1:14:0.78539816339744
-1:14:-0.78539816339744
# test an argument X > 1
2:14:1.1071487177941
&batan2
NaN:1:10:NaN
NaN:NaN:10:NaN
1:NaN:10:NaN
inf:1:14:1.5707963267949
-inf:1:14:-1.5707963267949
0:-inf:14:3.1415926535898
-1:-inf:14:-3.1415926535898
1:-inf:14:3.1415926535898
0:inf:14:0
inf:-inf:14:2.3561944901923
-inf:-inf:14:-2.3561944901923
inf:+inf:14:0.7853981633974
-inf:+inf:14:-0.7853981633974
1:5:13:0.1973955598499
1:5:14:0.19739555984988
0:0:10:0
0:1:14:0
0:2:14:0
1:0:14:1.5707963267949
5:0:14:1.5707963267949
-1:0:11:-1.5707963268
-2:0:77:-1.5707963267948966192313216916397514420985846996875529104874722961539082031431
2:0:77:1.5707963267948966192313216916397514420985846996875529104874722961539082031431
-1:5:14:-0.19739555984988
1:5:14:0.19739555984988
-1:8:14:-0.12435499454676
1:8:14:0.12435499454676
-1:1:14:-0.78539816339744
# test an argument X > 1 and one X < 1
1:2:24:0.463647609000806116214256
2:1:14:1.1071487177941
-2:1:14:-1.1071487177941
&bpi
150:3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940813
77:3.1415926535897932384626433832795028841971693993751058209749445923078164062862
+0:3.141592653589793238462643383279502884197
11:3.1415926536
&bnok
+inf:10:inf
NaN:NaN:NaN
NaN:1:NaN
1:NaN:NaN
1:1:1
# k > n
1:2:0
2:3:0
# k < 0
1:-2:0
# 7 over 3 = 35
7:3:35
7:6:7
100:90:17310309456440
100:95:75287520
2:0:1
7:0:1
2:1:2
&flog
0::-inf
-1::NaN
-2::NaN
# base > 0, base != 1
2:-1:NaN
2:0:0
2:1:NaN
# log(1)
1::0
1:1:NaN
1:2:0
2::0.6931471805599453094172321214581765680755
2.718281828::0.9999999998311266953289851340574956564911
$div_scale = 20;
2.718281828::0.99999999983112669533
$div_scale = 15;
123::4.81218435537242
10::2.30258509299405
1000::6.90775527898214
100::4.60517018598809
2::0.693147180559945
3.1415::1.14470039286086
12345::9.42100640177928
0.001::-6.90775527898214
# bug until v1.71:
10:10:1
100:100:1
# reset for further tests
$div_scale = 40;
1::0
&frsft
NaNfrsft:2:NaN
0:2:0
1:1:0.5
2:1:1
4:1:2
123:1:61.5
32:3:4
&flsft
NaNflsft:0:NaN
2:1:4
4:3:32
5:3:40
1:2:4
0:5:0
&fnorm
1:1
-0:0
fnormNaN:NaN
+inf:inf
-inf:-inf
123:123
-123.4567:-123.4567
# invalid inputs
1__2:NaN
1E1__2:NaN
11__2E2:NaN
.2E-3.:NaN
1e3e4:NaN
# strange, but valid
.2E2:20
1.E3:1000
# some inputs that result in zero
0e0:0
+0e0:0
+0e+0:0
-0e+0:0
0e-0:0
-0e-0:0
+0e-0:0
000:0
00e2:0
00e02:0
000e002:0
000e1230:0
00e-3:0
00e+3:0
00e-03:0
00e+03:0
-000:0
-00e2:0
-00e02:0
-000e002:0
-000e1230:0
-00e-3:0
-00e+3:0
-00e-03:0
-00e+03:0
&as_number
0:0
1:1
1.2:1
2.345:2
-2:-2
-123.456:-123
-200:-200
-inf:-inf
inf:inf
NaN:NaN
71243225429896467497217836789578596379:71243225429896467497217836789578596379
# test for bug in brsft() not handling cases that return 0
0.000641:0
0.0006412:0
0.00064123:0
0.000641234:0
0.0006412345:0
0.00064123456:0
0.000641234567:0
0.0006412345678:0
0.00064123456789:0
0.1:0
0.01:0
0.001:0
0.0001:0
0.00001:0
0.000001:0
0.0000001:0
0.00000001:0
0.000000001:0
0.0000000001:0
0.00000000001:0
0.12345:0
0.123456:0
0.1234567:0
0.12345678:0
0.123456789:0
&finf
1:+:inf
2:-:-inf
3:abc:inf
&as_hex
+inf:inf
-inf:-inf
hexNaN:NaN
0:0x0
5:0x5
-5:-0x5
&as_bin
+inf:inf
-inf:-inf
hexNaN:NaN
0:0b0
5:0b101
-5:-0b101
&numify
# uses bsstr() so 5 => 5e+0 to be compatible w/ Perls output
0:0
+1:1
1234:1234
-5:-5
100:100
-100:-100
&fnan
abc:NaN
2:NaN
-2:NaN
0:NaN
&fone
2:+:1
-2:-:-1
-2:+:1
2:-:-1
0::1
-2::1
abc::1
2:abc:1
&fsstr
+inf:inf
-inf:-inf
abcfsstr:NaN
-abcfsstr:NaN
1234.567:1234567e-3
123:123e+0
-5:-5e+0
-100:-1e+2
&fstr
+inf:::inf
-inf:::-inf
abcfstr:::NaN
1234.567:9::1234.56700
1234.567::-6:1234.567000
12345:5::12345
0.001234:6::0.00123400
0.001234::-8:0.00123400
0:4::0
0::-4:0.0000
&fnorm
inf:inf
+inf:inf
-inf:-inf
+infinity:NaN
+-inf:NaN
abc:NaN
   1 a:NaN
1bcd2:NaN
11111b:NaN
+1z:NaN
-1z:NaN
0e999:0
0e-999:0
-0e999:0
-0e-999:0
0:0
+0:0
+00:0
+0_0_0:0
000000_0000000_00000:0
-0:0
-0000:0
+1:1
+01:1
+001:1
+00000100000:100000
123456789:123456789
-1:-1
-01:-1
-001:-1
-123456789:-123456789
-00000100000:-100000
123.456a:NaN
123.456:123.456
0.01:0.01
.002:0.002
+.2:0.2
-0.0003:-0.0003
-.0000000004:-0.0000000004
123456E2:12345600
123456E-2:1234.56
-123456E2:-12345600
-123456E-2:-1234.56
1e1:10
2e-11:0.00000000002
# exercise _split
  .02e-1:0.002
   000001:1
   -00001:-1
   -1:-1
  000.01:0.01
   -000.0023:-0.0023
  1.1e1:11
-3e111:-3000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
-4e-1111:-0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004
&fpow
NaN:1:NaN
1:NaN:NaN
NaN:-1:NaN
-1:NaN:NaN
NaN:-21:NaN
-21:NaN:NaN
NaN:21:NaN
21:NaN:NaN
0:0:1
0:1:0
0:9:0
0:-2:inf
2:2:4
1:2:1
1:3:1
-1:2:1
-1:3:-1
123.456:2:15241.383936
2:-2:0.25
2:-3:0.125
128:-2:0.00006103515625
abc:123.456:NaN
123.456:abc:NaN
+inf:123.45:inf
-inf:123.45:-inf
+inf:-123.45:inf
-inf:-123.45:-inf
-2:2:4
-2:3:-8
-2:4:16
-2:5:-32
-3:2:9
-3:3:-27
-3:4:81
-3:5:-243
# 2 ** 0.5 == sqrt(2)
#     1.41..7 and not 1.4170 since fallback (bsqrt(9) is '3', not 3.0...0)
2:0.5:1.41421356237309504880168872420969807857
#2:0.2:1.148698354997035006798626946777927589444
#6:1.5:14.6969384566990685891837044482353483518
$div_scale = 20;
#62.5:12.5:26447206647554886213592.3959144
$div_scale = 40;
&fneg
fnegNaN:NaN
+inf:-inf
-inf:inf
+0:0
+1:-1
-1:1
+123456789:-123456789
-123456789:123456789
+123.456789:-123.456789
-123456.789:123456.789
&fabs
fabsNaN:NaN
+inf:inf
-inf:inf
+0:0
+1:1
-1:1
+123456789:123456789
-123456789:123456789
+123.456789:123.456789
-123456.789:123456.789
&fround
$round_mode = "trunc"
+inf:5:inf
-inf:5:-inf
0:5:0
NaNfround:5:NaN
+10123456789:5:10123000000
-10123456789:5:-10123000000
+10123456789.123:5:10123000000
-10123456789.123:5:-10123000000
+10123456789:9:10123456700
-10123456789:9:-10123456700
+101234500:6:101234000
-101234500:6:-101234000
$round_mode = "zero"
+20123456789:5:20123000000
-20123456789:5:-20123000000
+20123456789.123:5:20123000000
-20123456789.123:5:-20123000000
+20123456789:9:20123456800
-20123456789:9:-20123456800
+201234500:6:201234000
-201234500:6:-201234000
$round_mode = "+inf"
+30123456789:5:30123000000
-30123456789:5:-30123000000
+30123456789.123:5:30123000000
-30123456789.123:5:-30123000000
+30123456789:9:30123456800
-30123456789:9:-30123456800
+301234500:6:301235000
-301234500:6:-301234000
$round_mode = "-inf"
+40123456789:5:40123000000
-40123456789:5:-40123000000
+40123456789.123:5:40123000000
-40123456789.123:5:-40123000000
+40123456789:9:40123456800
-40123456789:9:-40123456800
+401234500:6:401234000
-401234500:6:-401235000
$round_mode = "odd"
+50123456789:5:50123000000
-50123456789:5:-50123000000
+50123456789.123:5:50123000000
-50123456789.123:5:-50123000000
+50123456789:9:50123456800
-50123456789:9:-50123456800
+501234500:6:501235000
-501234500:6:-501235000
$round_mode = "even"
+60123456789:5:60123000000
-60123456789:5:-60123000000
+60123456789:9:60123456800
-60123456789:9:-60123456800
+601234500:6:601234000
-601234500:6:-601234000
+60123456789.0123:5:60123000000
-60123456789.0123:5:-60123000000
$round_mode = "common"
+60123456789:5:60123000000
-60123456789:5:-60123000000
+60123456789:6:60123500000
-60123456789:6:-60123500000
+60123456789:9:60123456800
-60123456789:9:-60123456800
+601234500:6:601235000
-601234500:6:-601235000
+601234400:6:601234000
-601234400:6:-601234000
+601234600:6:601235000
-601234600:6:-601235000
+601234300:6:601234000
+60123456789.0123:5:60123000000
-60123456789.0123:5:-60123000000
&ffround
$round_mode = "trunc"
+inf:5:inf
-inf:5:-inf
0:5:0
NaNffround:5:NaN
+1.23:-1:1.2
+1.234:-1:1.2
+1.2345:-1:1.2
+1.23:-2:1.23
+1.234:-2:1.23
+1.2345:-2:1.23
+1.23:-3:1.230
+1.234:-3:1.234
+1.2345:-3:1.234
-1.23:-1:-1.2
+1.27:-1:1.2
-1.27:-1:-1.2
+1.25:-1:1.2
-1.25:-1:-1.2
+1.35:-1:1.3
-1.35:-1:-1.3
-0.0061234567890:-1:0.0
-0.0061:-1:0.0
-0.00612:-1:0.0
-0.00612:-2:0.00
-0.006:-1:0.0
-0.006:-2:0.00
-0.0006:-2:0.00
-0.0006:-3:0.000
-0.0065:-3:/-0\.006|-6e-03
-0.0065:-4:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
-0.0065:-5:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
0.05:0:0
0.5:0:0
0.51:0:0
0.41:0:0
$round_mode = "zero"
+2.23:-1:/2.2(?:0{5}\d+)?
-2.23:-1:/-2.2(?:0{5}\d+)?
+2.27:-1:/2.(?:3|29{5}\d+)
-2.27:-1:/-2.(?:3|29{5}\d+)
+2.25:-1:/2.2(?:0{5}\d+)?
-2.25:-1:/-2.2(?:0{5}\d+)?
+2.35:-1:/2.(?:3|29{5}\d+)
-2.35:-1:/-2.(?:3|29{5}\d+)
-0.0065:-1:0.0
-0.0065:-2:/-0\.01|-1e-02
-0.0065:-3:/-0\.006|-6e-03
-0.0065:-4:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
-0.0065:-5:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
0.05:0:0
0.5:0:0
0.51:0:1
0.41:0:0
$round_mode = "+inf"
+3.23:-1:/3.2(?:0{5}\d+)?
-3.23:-1:/-3.2(?:0{5}\d+)?
+3.27:-1:/3.(?:3|29{5}\d+)
-3.27:-1:/-3.(?:3|29{5}\d+)
+3.25:-1:/3.(?:3|29{5}\d+)
-3.25:-1:/-3.2(?:0{5}\d+)?
+3.35:-1:/3.(?:4|39{5}\d+)
-3.35:-1:/-3.(?:3|29{5}\d+)
-0.0065:-1:0.0
-0.0065:-2:/-0\.01|-1e-02
-0.0065:-3:/-0\.006|-6e-03
-0.0065:-4:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
-0.0065:-5:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
0.05:0:0
0.5:0:1
0.51:0:1
0.41:0:0
$round_mode = "-inf"
+4.23:-1:/4.2(?:0{5}\d+)?
-4.23:-1:/-4.2(?:0{5}\d+)?
+4.27:-1:/4.(?:3|29{5}\d+)
-4.27:-1:/-4.(?:3|29{5}\d+)
+4.25:-1:/4.2(?:0{5}\d+)?
-4.25:-1:/-4.(?:3|29{5}\d+)
+4.35:-1:/4.(?:3|29{5}\d+)
-4.35:-1:/-4.(?:4|39{5}\d+)
-0.0065:-1:0.0
-0.0065:-2:/-0\.01|-1e-02
-0.0065:-3:/-0\.007|-7e-03
-0.0065:-4:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
-0.0065:-5:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
0.05:0:0
0.5:0:0
0.51:0:1
0.41:0:0
$round_mode = "odd"
+5.23:-1:/5.2(?:0{5}\d+)?
-5.23:-1:/-5.2(?:0{5}\d+)?
+5.27:-1:/5.(?:3|29{5}\d+)
-5.27:-1:/-5.(?:3|29{5}\d+)
+5.25:-1:/5.(?:3|29{5}\d+)
-5.25:-1:/-5.(?:3|29{5}\d+)
+5.35:-1:/5.(?:3|29{5}\d+)
-5.35:-1:/-5.(?:3|29{5}\d+)
-0.0065:-1:0.0
-0.0065:-2:/-0\.01|-1e-02
-0.0065:-3:/-0\.007|-7e-03
-0.0065:-4:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
-0.0065:-5:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
0.05:0:0
0.5:0:1
0.51:0:1
0.41:0:0
$round_mode = "even"
+6.23:-1:/6.2(?:0{5}\d+)?
-6.23:-1:/-6.2(?:0{5}\d+)?
+6.27:-1:/6.(?:3|29{5}\d+)
-6.27:-1:/-6.(?:3|29{5}\d+)
+6.25:-1:/6.(?:2(?:0{5}\d+)?|29{5}\d+)
-6.25:-1:/-6.(?:2(?:0{5}\d+)?|29{5}\d+)
+6.35:-1:/6.(?:4|39{5}\d+|29{8}\d+)
-6.35:-1:/-6.(?:4|39{5}\d+|29{8}\d+)
-0.0065:-1:0.0
-0.0065:-2:/-0\.01|-1e-02
-0.0065:-3:/-0\.006|-7e-03
-0.0065:-4:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
-0.0065:-5:/-0\.006(?:5|49{5}\d+)|-6\.5e-03
0.05:0:0
0.5:0:0
0.51:0:1
0.41:0:0
0.01234567:-3:0.012
0.01234567:-4:0.0123
0.01234567:-5:0.01235
0.01234567:-6:0.012346
0.01234567:-7:0.0123457
0.01234567:-8:0.01234567
0.01234567:-9:0.012345670
0.01234567:-12:0.012345670000
&fcmp
fcmpNaN:fcmpNaN:
fcmpNaN:+0:
+0:fcmpNaN:
+0:+0:0
-1:+0:-1
+0:-1:1
+1:+0:1
+0:+1:-1
-1:+1:-1
+1:-1:1
-1:-1:0
+1:+1:0
-1.1:0:-1
+0:-1.1:1
+1.1:+0:1
+0:+1.1:-1
+123:+123:0
+123:+12:1
+12:+123:-1
-123:-123:0
-123:-12:-1
-12:-123:1
+123:+124:-1
+124:+123:1
-123:-124:1
-124:-123:-1
0:0.01:-1
0:0.0001:-1
0:-0.0001:1
0:-0.1:1
0.1:0:1
0.00001:0:1
-0.0001:0:-1
-0.1:0:-1
0:0.0001234:-1
0:-0.0001234:1
0.0001234:0:1
-0.0001234:0:-1
0.0001:0.0005:-1
0.0005:0.0001:1
0.005:0.0001:1
0.001:0.0005:1
0.000001:0.0005:-1
0.00000123:0.0005:-1
0.00512:0.0001:1
0.005:0.000112:1
0.00123:0.0005:1
1.5:2:-1
2:1.5:1
1.54321:234:-1
234:1.54321:1
1e1234567890987654321:1e1234567890987654320:1
1e-1234567890987654321:1e-1234567890987654320:-1
# infinity
-inf:5432112345:-1
+inf:5432112345:1
-inf:-5432112345:-1
+inf:-5432112345:1
-inf:54321.12345:-1
+inf:54321.12345:1
-inf:-54321.12345:-1
+inf:-54321.12345:1
+inf:+inf:0
-inf:-inf:0
+inf:-inf:1
-inf:+inf:-1
# return undef
+inf:NaN:
NaN:inf:
-inf:NaN:
NaN:-inf:
&facmp
fcmpNaN:fcmpNaN:
fcmpNaN:+0:
+0:fcmpNaN:
+0:+0:0
-1:+0:1
+0:-1:-1
+1:+0:1
+0:+1:-1
-1:+1:0
+1:-1:0
-1:-1:0
+1:+1:0
-1.1:0:1
+0:-1.1:-1
+1.1:+0:1
+0:+1.1:-1
+123:+123:0
+123:+12:1
+12:+123:-1
-123:-123:0
-123:-12:1
-12:-123:-1
+123:+124:-1
+124:+123:1
-123:-124:-1
-124:-123:1
0:0.01:-1
0:0.0001:-1
0:-0.0001:-1
0:-0.1:-1
0.1:0:1
0.00001:0:1
-0.0001:0:1
-0.1:0:1
0:0.0001234:-1
0:-0.0001234:-1
0.0001234:0:1
-0.0001234:0:1
0.0001:0.0005:-1
0.0005:0.0001:1
0.005:0.0001:1
0.001:0.0005:1
0.000001:0.0005:-1
0.00000123:0.0005:-1
0.00512:0.0001:1
0.005:0.000112:1
0.00123:0.0005:1
1.5:2:-1
2:1.5:1
1.54321:234:-1
234:1.54321:1
# infinity
-inf:5432112345:1
+inf:5432112345:1
-inf:-5432112345:1
+inf:-5432112345:1
-inf:54321.12345:1
+inf:54321.12345:1
-inf:-54321.12345:1
+inf:-54321.12345:1
+inf:+inf:0
-inf:-inf:0
+inf:-inf:0
-inf:+inf:0
5:inf:-1
-1:inf:-1
5:-inf:-1
-1:-inf:-1
# return undef
+inf:facmpNaN:
facmpNaN:inf:
-inf:facmpNaN:
facmpNaN:-inf:
&fdec
fdecNaN:NaN
+inf:inf
-inf:-inf
+0:-1
+1:0
-1:-2
1.23:0.23
-1.23:-2.23
100:99
101:100
-100:-101
-99:-100
-98:-99
99:98
&finc
fincNaN:NaN
+inf:inf
-inf:-inf
+0:1
+1:2
-1:0
1.23:2.23
-1.23:-0.23
100:101
-100:-99
-99:-98
-101:-100
99:100
&fadd
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+inf:-inf:NaN
-inf:+inf:NaN
+inf:+inf:inf
-inf:-inf:-inf
baddNaN:+inf:NaN
baddNaN:+inf:NaN
+inf:baddNaN:NaN
-inf:baddNaN:NaN
+0:+0:0
+1:+0:1
+0:+1:1
+1:+1:2
-1:+0:-1
+0:-1:-1
-1:-1:-2
-1:+1:0
+1:-1:0
+9:+1:10
+99:+1:100
+999:+1:1000
+9999:+1:10000
+99999:+1:100000
+999999:+1:1000000
+9999999:+1:10000000
+99999999:+1:100000000
+999999999:+1:1000000000
+9999999999:+1:10000000000
+99999999999:+1:100000000000
+10:-1:9
+100:-1:99
+1000:-1:999
+10000:-1:9999
+100000:-1:99999
+1000000:-1:999999
+10000000:-1:9999999
+100000000:-1:99999999
+1000000000:-1:999999999
+10000000000:-1:9999999999
+123456789:+987654321:1111111110
-123456789:+987654321:864197532
-123456789:-987654321:-1111111110
+123456789:-987654321:-864197532
0.001234:0.0001234:0.0013574
&fsub
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+inf:-inf:inf
-inf:+inf:-inf
+inf:+inf:NaN
-inf:-inf:NaN
baddNaN:+inf:NaN
baddNaN:+inf:NaN
+inf:baddNaN:NaN
-inf:baddNaN:NaN
+0:+0:0
+1:+0:1
+0:+1:-1
+1:+1:0
-1:+0:-1
+0:-1:1
-1:-1:0
-1:+1:-2
+1:-1:2
+9:+1:8
+99:+1:98
+999:+1:998
+9999:+1:9998
+99999:+1:99998
+999999:+1:999998
+9999999:+1:9999998
+99999999:+1:99999998
+999999999:+1:999999998
+9999999999:+1:9999999998
+99999999999:+1:99999999998
+10:-1:11
+100:-1:101
+1000:-1:1001
+10000:-1:10001
+100000:-1:100001
+1000000:-1:1000001
+10000000:-1:10000001
+100000000:-1:100000001
+1000000000:-1:1000000001
+10000000000:-1:10000000001
+123456789:+987654321:-864197532
-123456789:+987654321:-1111111110
-123456789:-987654321:864197532
+123456789:-987654321:1111111110
&bmuladd
abc:abc:0:NaN
abc:+0:0:NaN
+0:abc:0:NaN
+0:0:abc:NaN
NaNmul:+inf:0:NaN
NaNmul:-inf:0:NaN
-inf:NaNmul:0:NaN
+inf:NaNmul:0:NaN
+inf:+inf:0:inf
+inf:-inf:0:-inf
-inf:+inf:0:-inf
-inf:-inf:0:inf
+0:+0:0:0
+0:+1:0:0
+1:+0:0:0
+0:-1:0:0
-1:+0:0:0
123456789123456789:0:0:0
0:123456789123456789:0:0
-1:-1:0:1
-1:-1:0:1
-1:+1:0:-1
+1:-1:0:-1
+1:+1:0:1
+2:+3:0:6
-2:+3:0:-6
+2:-3:0:-6
-2:-3:0:6
111:111:0:12321
10101:10101:0:102030201
1001001:1001001:0:1002003002001
100010001:100010001:0:10002000300020001
10000100001:10000100001:0:100002000030000200001
11111111111:9:0:99999999999
22222222222:9:0:199999999998
33333333333:9:0:299999999997
44444444444:9:0:399999999996
55555555555:9:0:499999999995
66666666666:9:0:599999999994
77777777777:9:0:699999999993
88888888888:9:0:799999999992
99999999999:9:0:899999999991
11111111111:9:1:100000000000
22222222222:9:1:199999999999
33333333333:9:1:299999999998
44444444444:9:1:399999999997
55555555555:9:1:499999999996
66666666666:9:1:599999999995
77777777777:9:1:699999999994
88888888888:9:1:799999999993
99999999999:9:1:899999999992
-3:-4:-5:7
3:-4:-5:-17
-3:4:-5:-17
3:4:-5:7
-3:4:5:-7
3:-4:5:-7
9999999999999999999:10000000000000000000:1234567890:99999999999999999990000000001234567890
3.2:5.7:8.9:27.14
-3.2:5.197:6.05:-10.5804
&bmodpow
3:4:8:1
3:4:7:4
3:4:7:4
77777:777:123456789:99995084
3.2:6.2:5.2:2.970579856718063040273642739529400818
&fmul
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+inf:NaNmul:NaN
+inf:NaNmul:NaN
NaNmul:+inf:NaN
NaNmul:-inf:NaN
+inf:+inf:inf
+inf:-inf:-inf
+inf:-inf:-inf
+inf:+inf:inf
+inf:123.34:inf
+inf:-123.34:-inf
-inf:123.34:-inf
-inf:-123.34:inf
123.34:+inf:inf
-123.34:+inf:-inf
123.34:-inf:-inf
-123.34:-inf:inf
+0:+0:0
+0:+1:0
+1:+0:0
+0:-1:0
-1:+0:0
+123456789123456789:+0:0
+0:+123456789123456789:0
-1:-1:1
-1:+1:-1
+1:-1:-1
+1:+1:1
+2:+3:6
-2:+3:-6
+2:-3:-6
-2:-3:6
+111:+111:12321
+10101:+10101:102030201
+1001001:+1001001:1002003002001
+100010001:+100010001:10002000300020001
+10000100001:+10000100001:100002000030000200001
+11111111111:+9:99999999999
+22222222222:+9:199999999998
+33333333333:+9:299999999997
+44444444444:+9:399999999996
+55555555555:+9:499999999995
+66666666666:+9:599999999994
+77777777777:+9:699999999993
+88888888888:+9:799999999992
+99999999999:+9:899999999991
6:120:720
10:10000:100000
&fdiv-list
0:0:NaN,0
0:1:0,0
9:4:2,1
9:5:1,4
# bug in v1.74 with bdiv in list context, when $y is 1 or -1
2.1:-1:-2.1,0
2.1:1:2.1,0
-2.1:-1:2.1,0
-2.1:1:-2.1,0
&fdiv
$div_scale = 40; $round_mode = 'even'
abc:abc:NaN
abc:+1:abc:NaN
+1:abc:NaN
-1:abc:NaN
0:abc:NaN
+0:+0:NaN
+0:+1:0
+1:+0:inf
+3214:+0:inf
+0:-1:0
-1:+0:-inf
-3214:+0:-inf
+1:+1:1
-1:-1:1
+1:-1:-1
-1:+1:-1
+1:+2:0.5
+2:+1:2
123:+inf:0
123:-inf:0
+10:+5:2
+100:+4:25
+1000:+8:125
+10000:+16:625
+10000:-16:-625
+999999999999:+9:111111111111
+999999999999:+99:10101010101
+999999999999:+999:1001001001
+999999999999:+9999:100010001
+999999999999999:+99999:10000100001
+1000000000:+9:111111111.1111111111111111111111111111111
+2000000000:+9:222222222.2222222222222222222222222222222
+3000000000:+9:333333333.3333333333333333333333333333333
+4000000000:+9:444444444.4444444444444444444444444444444
+5000000000:+9:555555555.5555555555555555555555555555556
+6000000000:+9:666666666.6666666666666666666666666666667
+7000000000:+9:777777777.7777777777777777777777777777778
+8000000000:+9:888888888.8888888888888888888888888888889
+9000000000:+9:1000000000
+35500000:+113:314159.2920353982300884955752212389380531
+71000000:+226:314159.2920353982300884955752212389380531
+106500000:+339:314159.2920353982300884955752212389380531
+1000000000:+3:333333333.3333333333333333333333333333333
2:25.024996000799840031993601279744051189762:0.07992009269196593320152084692285869265447
123456:1:123456
$div_scale = 20
+1000000000:+9:111111111.11111111111
+2000000000:+9:222222222.22222222222
+3000000000:+9:333333333.33333333333
+4000000000:+9:444444444.44444444444
+5000000000:+9:555555555.55555555556
+6000000000:+9:666666666.66666666667
+7000000000:+9:777777777.77777777778
+8000000000:+9:888888888.88888888889
+9000000000:+9:1000000000
1:10:0.1
1:100:0.01
1:1000:0.001
1:10000:0.0001
1:504:0.001984126984126984127
2:1.987654321:1.0062111801179738436
123456789.123456789123456789123456789:1:123456789.12345678912
# the next two cases are the "old" behaviour, but are now (>v0.01) different
#+35500000:+113:314159.292035398230088
#+71000000:+226:314159.292035398230088
+35500000:+113:314159.29203539823009
+71000000:+226:314159.29203539823009
+106500000:+339:314159.29203539823009
+1000000000:+3:333333333.33333333333
$div_scale = 1
# round to accuracy 1 after bdiv
+124:+3:40
123456789.1234:1:100000000
# reset scale for further tests
$div_scale = 40
&fmod
+9:4:1
+9:5:4
+9000:56:40
+56:9000:56
# inf handling, see table in doc
0:inf:0
0:-inf:0
5:inf:5
5:-inf:-inf
-5:inf:inf
-5:-inf:-5
inf:5:NaN
-inf:5:NaN
inf:-5:NaN
-inf:-5:NaN
5:5:0
-5:-5:0
inf:inf:NaN
-inf:-inf:NaN
-inf:inf:NaN
inf:-inf:NaN
8:0:8
inf:0:inf
-inf:0:-inf
-8:0:-8
0:0:0
abc:abc:NaN
abc:1:abc:NaN
1:abc:NaN
0:1:0
1:0:1
0:-1:0
-1:0:-1
1:1:0
-1:-1:0
1:-1:0
-1:1:0
1:2:1
2:1:0
1000000000:9:1
2000000000:9:2
3000000000:9:3
4000000000:9:4
5000000000:9:5
6000000000:9:6
7000000000:9:7
8000000000:9:8
9000000000:9:0
35500000:113:33
71000000:226:66
106500000:339:99
1000000000:3:1
10:5:0
100:4:0
1000:8:0
10000:16:0
999999999999:9:0
999999999999:99:0
999999999999:999:0
999999999999:9999:0
999999999999999:99999:0
-9:+5:1
+9:-5:-1
-9:-5:-4
-5:3:1
-2:3:1
4:3:1
1:3:1
-5:-3:-2
-2:-3:-2
4:-3:-2
1:-3:-2
4095:4095:0
100041000510123:3:0
152403346:12345:4321
87654321:87654321:0
# now some floating point tests
123:2.5:0.5
1230:2.5:0
123.4:2.5:0.9
123e1:25:5
-2.1:1:0.9
2.1:1:0.1
-2.1:-1:-0.1
2.1:-1:-0.9
-3:1:0
3:1:0
-3:-1:0
3:-1:0
&ffac
Nanfac:NaN
-1:NaN
+inf:inf
-inf:NaN
0:1
1:1
2:2
3:6
4:24
5:120
6:720
10:3628800
11:39916800
12:479001600
&froot
# sqrt()
+0:2:0
+1:2:1
-1:2:NaN
# -$x ** (1/2) => -$y, but not in froot()
-123.456:2:NaN
+inf:2:inf
-inf:2:NaN
2:2:1.41421356237309504880168872420969807857
-2:2:NaN
4:2:2
9:2:3
16:2:4
100:2:10
123.456:2:11.11107555549866648462149404118219234119
15241.38393:2:123.4559999756998444766131352122991626468
1.44:2:1.2
12:2:3.464101615137754587054892683011744733886
0.49:2:0.7
0.0049:2:0.07
# invalid ones
1:NaN:NaN
-1:NaN:NaN
0:NaN:NaN
-inf:NaN:NaN
+inf:NaN:NaN
NaN:0:NaN
NaN:2:NaN
NaN:inf:NaN
NaN:inf:NaN
12:-inf:NaN
12:inf:NaN
+0:0:NaN
+1:0:NaN
-1:0:NaN
-2:0:NaN
-123.45:0:NaN
+inf:0:NaN
12:1:12
-12:1:NaN
8:-1:NaN
-8:-1:NaN
# cubic root
8:3:2
-8:3:NaN
# fourths root
16:4:2
81:4:3
# see t/bigroot() for more tests
&fsqrt
+0:0
-1:NaN
-2:NaN
-16:NaN
-123.45:NaN
nanfsqrt:NaN
+inf:inf
-inf:NaN
1:1
2:1.41421356237309504880168872420969807857
4:2
9:3
16:4
100:10
123.456:11.11107555549866648462149404118219234119
15241.38393:123.4559999756998444766131352122991626468
1.44:1.2
# sqrt(1.44) = 1.2, sqrt(e10) = e5 => 12e4
1.44E10:120000
2e10:141421.356237309504880168872420969807857
144e20:120000000000
# proved to be an endless loop under 7-9
12:3.464101615137754587054892683011744733886
0.49:0.7
0.0049:0.07
&is_nan
123:0
abc:1
NaN:1
-123:0
&is_inf
+inf::1
-inf::1
abc::0
1::0
NaN::0
-1::0
+inf:-:0
+inf:+:1
-inf:-:1
-inf:+:0
# it must be exactly /^[+-]inf$/
+infinity::0
-infinity::0
&is_odd
abc:0
0:0
-1:1
-3:1
1:1
3:1
1000001:1
1000002:0
+inf:0
-inf:0
123.45:0
-123.45:0
2:0
&is_int
NaNis_int:0
0:1
1:1
2:1
-2:1
-1:1
-inf:0
+inf:0
123.4567:0
-0.1:0
-0.002:0
&is_even
abc:0
0:1
-1:0
-3:0
1:0
3:0
1000001:0
1000002:1
2:1
+inf:0
-inf:0
123.456:0
-123.456:0
0.01:0
-0.01:0
120:1
1200:1
-1200:1
&is_positive
0:0
1:1
-1:0
-123:0
NaN:0
-inf:0
+inf:1
&is_negative
0:0
1:0
-1:1
-123:1
NaN:0
-inf:1
+inf:0
&parts
0:0 1
1:1 0
123:123 0
-123:-123 0
-1200:-12 2
NaNparts:NaN NaN
+inf:inf inf
-inf:-inf inf
&exponent
0:1
1:0
123:0
-123:0
-1200:2
+inf:inf
-inf:inf
NaNexponent:NaN
&mantissa
0:0
1:1
123:123
-123:-123
-1200:-12
+inf:inf
-inf:-inf
NaNmantissa:NaN
&length
123:3
-123:3
0:1
1:1
12345678901234567890:20
&is_zero
NaNzero:0
+inf:0
-inf:0
0:1
-1:0
1:0
&is_one
NaNone:0
+inf:0
-inf:0
0:0
2:0
1:1
-1:0
-2:0
&ffloor
0:0
abc:NaN
+inf:inf
-inf:-inf
1:1
-51:-51
-51.2:-52
12.2:12
0.12345:0
0.123456:0
0.1234567:0
0.12345678:0
0.123456789:0
&fceil
0:0
abc:NaN
+inf:inf
-inf:-inf
1:1
-51:-51
-51.2:-51
12.2:13
-0.4:0
&fint
0:0
NaN:NaN
+inf:inf
-inf:-inf
1:1
-51:-51
-51.2:-51
12.2:12
-0.4:0
# overloaded functions
&log
-1:NaN
0:-inf
1:0
2:0.6931471805599453094172321214581765680755
3:1.098612288668109691395245236922525704647
123456789:18.63140176616801803319393334796320420971
1234567890987654321:41.657252696908474880343847955484513481
-inf:inf
inf:inf
NaN:NaN
&exp
&sin
&cos
&atan2
&int
&neg
&abs
&sqrt
