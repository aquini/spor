Name:      spor
Group:     Applications/System 
Summary:   Store file modes (permission/ownership) recursively 
Version:   1.0
Release:   0%{?dist}
License:   GPLv3+
URL:       http://code.google.com/p/spor
Source0:   http://spor.googlecode.com/files/spor-%{version}.tar.bz2 

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
Spor recursively walks into a given directory, storing file mode & ownership 
information in a flat-file database for future retrievals. It was firstly 
intended to use with backup & version control scripts, and it provides a 
simple and safe method to save and restore particular meta-data information 
of a given directory.

%prep
%setup -q -n %{name}

%build
%{__make} %{?_smp_mflags} CFLAGS="%{optflags}"

%install
%{__rm} -rf %{buildroot}
%{__install} -D -p -m 0755 %{name} %{buildroot}/%{_bindir}/%{name}
%{__install} -D -p -m 0644 %{name}.1 %{buildroot}/%{_mandir}/man1/%{name}.1

%clean
%{__rm} -rf %{buildroot}

%files
%defattr(-,root,root,-)
%doc readme.txt LICENSE
%{_bindir}/%{name}
%{_mandir}/man1/%{name}.1*

%changelog
* Sun Nov 07 2010 Rafael Azenha Aquini <aquini at linux dot com> - 1.0-0
- initial packaging.

