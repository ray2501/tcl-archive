# -*- tcl -*-
# Tcl package index file, version 1.1
#
if {[package vsatisfies [package provide Tcl] 9.0-]} {
    package ifneeded archive 0.4 \
	    [list load [file join $dir libtcl9archive0.4.so] [string totitle archive]]
} else {
    package ifneeded archive 0.4 \
	    [list load [file join $dir libarchive0.4.so] [string totitle archive]]
}
