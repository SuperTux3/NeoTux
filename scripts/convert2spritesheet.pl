#!/usr/bin/env perl
use Data::SExpression qw(cons consp scalarp);
use Image::Magick;
use File::Find;
use File::Spec;
use Getopt::Long;
use Data::Dumper;

$image = Image::Magick->new;

my $src = '../data';
my $dest = "data";

sub helpme
{
	"Usage: convert2spritesheet.pl [OPTIONS...]

  --src <dir>                  Source data directory with all sprites
  --dest <dir>                 Destination data directory";
}

GetOptions('src=s' => \$src,
           'dest=s' => \$dest,
		   'help' => sub { printf helpme() . "\n"; exit(); }) or die(helpme());
		   
sub src_dir { $src . '/' . $_[0] }
sub dest_dir { $dest . '/' . $_[0] }

sub append_spritesheet_from_data
{
	my ($image, $dir, $name) = @_;
	$image->Set(size => '100x100');
	$image->ReadImage('canvas:white');
	$image->Set('pixel[49,49]' => 'red');
	$image->Write(dest_dir($dir) . "/test.png");
}

sub parse_sprite
{
	my ($filename, $dir, $name) = @_;
	
	my $sprite = Data::SExpression->new({
		fold_dashes => 0,
		use_symbol_class => 1,
	});
	my $image = Image::Magick->new;
	open my $fh, '<', $filename or die "Couldn't open $filename: $!";
	my $fcontent = do { local $/; <$fh> };
	
	($sexp, $text) = $sprite->read($fcontent);
	die "$filename is not a supertux-sprite" unless $sexp->[0] eq "supertux-sprite";
	
	foreach my $root (@$sexp)
	{
		# supertux-sprite
		if (ref($root) eq 'ARRAY')
		{
			my $name;
			my @images;
			# action
			foreach my $list (@$root)
			{
				my $next = 0;
				if (ref($list) eq 'ARRAY')
				{
					foreach my $action (@$list)
					{
						if (ref($action) eq 'Data::SExpression::Symbol')
						{
							$next = 1 if $action eq "name";
							$next = 2 if $action eq "images";
							next;
						}
						$name = $action if $next == 1;
						push(@images, $action) if $next == 2;
					}
				}
			}
		}
	}
	append_spritesheet_from_data($image, $dir, $name);
	undef $image;
}

find(sub {
	my $filename = $_;
	if ($_ =~ /\.sprite/)
	{
		parse_sprite $File::Find::name, File::Spec->abs2rel($File::Find::dir, $src), $filename;
	}
}, $src);

