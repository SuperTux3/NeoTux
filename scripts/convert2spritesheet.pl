#!/usr/bin/env perl
use Data::SExpression qw(cons consp scalarp);
use Image::Magick;
use File::Find;
use File::Basename;
use File::Spec;
use Getopt::Long;
use Data::Dumper;

$image = Image::Magick->new;

my $src = '../data';
my $dest = "data";
my $MAX_W = 2000;

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
	my ($image, $dir, $actions) = @_;
	my $curr_max_w = 0;
	my $curr_max_h = 0;
	my $curr_y = 0;
	my $curr_x = 0;
	my $max_h = 0;
	
	my %used_images;
	
	foreach my $action (@$actions)
	{
		foreach my $file_rel (@{$action->{'images'}})
		{
			my $file = src_dir($dir) . '/' . $file_rel;
			if ($used_images{$file})
			{
				# Don't punch in new images again
				next;
			}
			else {
				$used_images{$file} = 1;
			}
			my $sprite = Image::Magick->new;
			
			# Work with image now
			my $err = $sprite->Read($file);
			#warn "$err" if "$err";
			my ($w, $h) = $sprite->Get('width', 'height');
			$max_h = $h if $h > $max_h;
			# Set canvas size
			$curr_max_h = $h + $curr_y if ($h + $curr_y > $curr_max_h);
			$curr_max_w = $w + $curr_x if ($w + $curr_x > $curr_max_w);
			
			# Resize image width if larger
			#$image->Extent(width => ($curr_x + $w), height => ($curr_y + $max_h));
			$image->Extent(geometry=>$curr_max_w.'x'.$curr_max_h, background=>'transparent');
			
			$image->CopyPixels(
				image => $sprite,
				width => $w, height => $h,
				x => 0, y => 0,
				offset => '0x0+'.$curr_x.'+'.$curr_y
			);
			
			# For the next image...
			undef $sprite;
			$curr_x += $w;
			
			if ($curr_x > $MAX_W)
			{
				$curr_x = 0;
				$curr_y += $max_h;
				$max_h = 0;
			}
		}
	}
}

sub parse_sprite
{
	my ($filename, $dir, $name) = @_;
	
	my $sprite = Data::SExpression->new({
		fold_dashes => 0,
		use_symbol_class => 1,
	});
	my $image = Image::Magick->new(size=>'100x100');
	$image->Read('xc:none');
	open my $fh, '<', $filename or die "Couldn't open $filename: $!";
	my $fcontent = do { local $/; <$fh> };
	
	($sexp, $text) = $sprite->read($fcontent);
	die "$filename is not a supertux-sprite" unless $sexp->[0] eq "supertux-sprite";
	
	my @actions;
	
	foreach my $root (@$sexp)
	{
		# supertux-sprite
		if (ref($root) eq 'ARRAY')
		{
			my $pname;
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
						$pname = $action if $next == 1;
						push(@images, $action) if $next == 2;
					}
				}
			}
			push(@actions, { name => $pname, images => [@images] });
		}
	}
	
	append_spritesheet_from_data($image, $dir, \@actions);
	my $x = $image->Write(dest_dir($dir) . "/sprite_$name.png");
	warn "$x" if "$x";
	
	print "Writing ".dest_dir($dir)."/sprite_$name.png\n";
	undef $image;
}

find(sub {
	my $filename = $_;
	if ($_ =~ /\.sprite/)
	{
		parse_sprite $File::Find::name, File::Spec->abs2rel($File::Find::dir, $src), basename($filename, ".sprite");
	}
}, $src);

