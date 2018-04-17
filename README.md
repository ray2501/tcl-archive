# tcl-archive

[LibArchive](https://www.libarchive.org/) project develops a portable, efficient C library that
can read and write streaming archives in a variety of formats. It also includes implementations
of the common tar, cpio, and zcat command-line tools that use the libarchive library.

It is a Tcl wrapper for LibArchive.

License
=====

BSD 2-Clause License


Commands
=====

`::archive::create` filename filter format filesList  
`::archive::extract` filename filter format doextractflag ?-path path? ?-fullpermissions boolean? ?-nooverwrite boolean? ?-passwd password?  
`::archive::version`  

::archive::create filter variable: b64encode, bzip2, compress, gzip, grzip, lrzip, lzip, lzma, lzop, none, uuencode and xz  
::archive::create format variable: 7zip, ar_bsd, ar_srv4, cpio, cpio_newc, gnutar, iso9660, mtree, mtree_classic, pax, 
pax_restricted, shar, shar_dump, ustar, v7tar, xar and zip  

`filesList` is a Tcl list of files. I think you can specify a folder at Linux or UNIX-like platform
(but don't do it at Windows platform).

::archive::extract filter variable: all, bzip2, compress, gzip, grzip, lrzip, lzip, lzma, lzop, none, rpm, uu and xz  
::archive::extract format variable: all, 7zip, ar, cab, cpio, gnutar, iso9660, lha, mtree, rar, tar, xar and zip  

`-path` option let you specify the output folder (absolute path).

`-passwd` option let you specify the passphrase to decrypt.
I think libarchvie added this feature >= 3.2.0 and only supported some formats (for example, zip).

`::archive::version` gets libarchive version.


UNIX BUILD
=====

I only test tcl-archive under openSUSE LEAP 42.3 and Ubuntu 14.04.

Users need install libarchive development files.
Below is an example for openSUSE:

	sudo zypper in libarchive-devel

Below is an example for Ubuntu:

	sudo apt-get install libarchive-dev

Building under most UNIX systems is easy, just run the configure script
and then run make. For more information about the build process, see the
tcl/unix/README file in the Tcl src dist. The following minimal example
will install the extension in the /opt/tcl directory.

	$ cd tcl-archive
	$ ./configure --prefix=/opt/tcl
	$ make
	$ make install

If you need setup directory containing tcl configuration (tclConfig.sh),
below is an example:

	$ cd tcl-archive
	$ ./configure --with-tcl=/opt/activetcl/lib
	$ make
	$ make install

WINDOWS BUILD
=====

## MSYS2/MinGW-W64

Users can use MSYS2 pacman to install libarchive package.

	$ pacman -S mingw-w64-x86_64-libarchive

Then just run the configure script and then run make.
