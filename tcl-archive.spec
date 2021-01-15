#
# openSUSE spec file for package tcl-archive
#

%{!?directory:%define directory /usr}
%define buildroot %{_tmppath}/%{name}
%define packagename archive

Name:     tcl-archive
Version:  0.4
Release:  0
License:  BSD-2-Clause
Summary:  A Tcl wrapper for LibArchive
Url:      https://github.com/ray2501/tcl-archive
Group:    Development/Libraries/Tcl
Source:   %{name}-%{version}.tar.gz
BuildRequires: autoconf
BuildRequires: gcc
BuildRequires: make
BuildRequires: tcl-devel >= 8.5
BuildRequires: libarchive-devel
Requires:      tcl >= 8.5
Requires:      libarchive13
BuildRoot:     %{buildroot}

%description
A Tcl wrapper for LibArchive.

%prep
%setup -q -n %{name}-%{version}

%build
./configure \
	--prefix=%{directory} \
	--exec-prefix=%{directory} \
	--libdir=%{directory}/%{_lib}
make 

%install
make DESTDIR=%{buildroot} pkglibdir=%{tcl_archdir}/%{packagename}%{version} install

%clean
rm -rf %buildroot

%files
%defattr(-,root,root)
%doc README.md LICENSE
%{tcl_archdir}/%{packagename}%{version}

