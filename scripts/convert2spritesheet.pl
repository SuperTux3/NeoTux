#!/usr/bin/env perl
#   SuperTux 
#   Copyright (C) 2025 Hyland B. <me@ow.swag.toys> 
#  
#   This program is free software: you can redistribute it and/or modify 
#   it under the terms of the GNU General Public License as published by 
#   the Free Software Foundation, either version 3 of the License, or 
#   (at your option) any later version. 
#  
#   This program is distributed in the hope that it will be useful, 
#   but WITHOUT ANY WARRANTY; without even the implied warranty of 
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
#   GNU General Public License for more details. 
#  
#   You should have received a copy of the GNU General Public License 
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.

use Data::SExpression;
use Image::Magick;
use File::Find;
use File::Basename;
use File::Spec;
use Getopt::Long;
use Data::Dumper;

my $src = undef;
my $dest = undef;
my $MAX_W = 1000;
my $MAX_H = undef;
my $quality = 9;
my $scale = 1.0;
my $split_sprites = undef;

sub helpme
{
	"Usage: convert2spritesheet.pl [OPTIONS...]

  --src <dir>                  Source data directory with all sprites
  --dest <dir>                 Destination data directory
  --max-width <width>          Optionally provide a texture width (default: 1000)
  --max-height <height>        Optionally provide a texture height (default: unset)
  --quality <quality>          Texture quality (PNG: 0-9)
  --scale <amount>             Scale each texture by this amount (default: 1.0)
  --split-sprites              Split spritesheets into multiple files (requires: max-width, max-height)";
}

GetOptions('src=s' => \$src,
           'dest=s' => \$dest,
		   'max-width=i' => \$MAX_W,
		   'max-height=i' => \$MAX_H,
		   'quality=i' => \$quality,
		   'scale=f' => \$scale,
		   'split-sprites' => sub { $split_sprites = 1; },
		   'help' => sub { printf helpme . "\n"; exit(); }) or die(helpme);

die "Must set both --max-width and --max-height if using --split-sprites.\n".helpme."\n" if $split_sprites && !$MAX_H;
die "Must provide two arguments.\n" . helpme . "\n" unless $src && $dest;
		   
sub src_dir { $src . '/' . $_[0] }
sub dest_dir { $dest . '/' . $_[0] }

sub cons_action
{
	my $action = $_[0];
	my ($name, $fps, $hitbox, $loops) = (
		$action->{'name'}, $action->{'fps'}, $action->{'hitbox'}, $action->{'loops'}
	);
	#print Dumper $images, $hitbox, $fps, $name;
	my $pad = "\t\t\t";
	my $res = "$pad(action \n";
	$res .= "$pad  (scale $scale)\n" if $scale != 1.0;
	$res .= "$pad  (name \"$name\")\n" if $name;
	$res .= "$pad  (fps $fps)\n" unless $fps eq '0';
	$res .= "$pad  (loops $loops)\n" if $loops;
	$res .= sprintf("$pad  (hitbox %s %s %s %s)\n", $hitbox->[0], $hitbox->[1], $hitbox->[2], $hitbox->[3]);
	
	$res;
}

sub parse_sprite_data
{
	my ($image, $dir, $name, $actions) = @_;
	my $curr_max_w = 0;
	my $curr_max_h = 0;
	my $curr_y = 0;
	my $curr_x = 0;
	my $max_h = 0;
	my $spritesheet_idx = 0;
	
	my %used_images;
	

	my $sexp = "";
	my $spritesheet_sexp = "";
	$spritesheet_sexp = "(spritesheet (images ".$name."_spritesheet$spritesheet_idx.png ";
	#$sexp .= "\t\t\t(spritesheet (image \"".$name."_spritesheet.png\"))\n" unless $split_sprites;
	
	foreach my $action (@$actions)
	{
		my $is_spritesheet = 0;
		$sexp .= cons_action($action);
		if (scalar @{$action->{'images'}} != 0)
		{
			$is_spritesheet = 1;
			$sexp .= "\t\t\t  (spritesheets\n";
		}
		foreach my $file_rel (@{$action->{'images'}})
		{
			my $file = src_dir($dir) . '/' . $file_rel;
			# Don't punch in new images again
			#next if ($used_images{$file});
			my $sprite = Image::Magick->new;
			
			# Work with image now
			my $err = $sprite->Read($file);
			my ($w, $h) = $sprite->Get('width', 'height');
			
			unless ($used_images{$file})
			{
				if ($curr_x + $w > $MAX_W)
				{
					$curr_x = 0;
					$curr_y += $max_h;
					$max_h = 0;
				}
				
				if ($split_sprites && $curr_y + $h > $MAX_H)
				{
					$curr_x = 0;
					$curr_y = 0;
					$max_h = 0;
					my $fn = $name."_spritesheet$spritesheet_idx.png";
					$spritesheet_idx += 1;
					$image->Scale(width => $w * $scale, height => $h * $scale) if $scale != 1.0;
					$image->Write(dest_dir($dir) . "/" . $fn);
					print "Writing ".dest_dir($dir)."/$fn\n";
					$fn = $name."_spritesheet$spritesheet_idx.png";
					$spritesheet_sexp .= "$fn ";
				}
				
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

				# Store spritesheet information
				$used_images{$file} = [
					$curr_x * $scale, $curr_y * $scale,
					$w * $scale, $h * $scale
				];

				# For the next image...
				$curr_x += $w;

			}
			
			$sexp .= sprintf("\t\t\t    (%d %d %d %d %d)\n",
				$used_images{$file}->[0], $used_images{$file}->[1],
				$used_images{$file}->[2], $used_images{$file}->[3],
				$spritesheet_idx) if $is_spritesheet;
			
			undef $sprite;
		}
		$sexp .= ')';
		$sexp .= ')' if $is_spritesheet;
	}
	
	$sexp = ";; Ugly trash proudly produced by ./convert2spritesheet.pl
(supertux-sprite\n $spritesheet_sexp)) $sexp )";
	open my $fh, '>', dest_dir($dir).'/'.$name."_sheet.sprite" or die "Could not open sprite";
	print $fh $sexp;
	print "Writing ".dest_dir($dir).'/'.$name."_sheet.sprite\n";
	
	
	my ($w, $h) = $image->Get('width', 'height');
	$image->Scale(width => $w * $scale, height => $h * $scale) if $scale != 1.0;
	$image->Write(dest_dir($dir) . "/".$name."_spritesheet$spritesheet_idx.png");
	print "Writing ".dest_dir($dir).'/'.$name."_spritesheet$spritesheet_idx.png\n";
}

sub parse_sprite
{
	my ($filename, $dir, $name) = @_;
	
	my $sprite = Data::SExpression->new({
		fold_dashes => 0,
		use_symbol_class => 1,
	});
	my $image = Image::Magick->new(size=>'10x10');
	$image->Set(quality=>$quality);
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
			# For consistency we keep fps as a string.
			my $fps = "0";
			my @hitbox;
			my $loops;
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
							$next = 3 if $action eq "hitbox";
							$next = 4 if $action eq "fps";
							$next = 5 if $action eq "loops";
							next;
						}
						$pname = $action if $next == 1;
						push(@images, $action) if $next == 2;
						push(@hitbox, $action) if $next == 3;
						$fps = $action if $next == 4;
						$loops = $action if $next == 5;
					}
				}
			}
			push(@actions, {
				name   => $pname,
				images => [@images],
				fps    => $fps,
				hitbox => [@hitbox],
				loops  => $loops,
			});
		}
	}
	
	parse_sprite_data($image, $dir, $name, \@actions);
	
	undef $image;
}

find(sub {
	my $filename = $_;
	if ($_ =~ /\.sprite/)
	{
		parse_sprite $File::Find::name, File::Spec->abs2rel($File::Find::dir, $src), basename($filename, ".sprite");
	}
}, $src);

