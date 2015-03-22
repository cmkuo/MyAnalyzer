#!/usr/bin/perl
 
my @list = `ls /data3/ncuhep/splash/digi/BeamSplash*.root`;

for (@list) {

    chomp($_);
    my $fname = $_;

    system("cmsRun test.py files=file:$fname");

    $fname =~ s/\/data3\/ncuhep\/splash\/digi\/// ;

    system("mv test.root tree_$fname");
}
