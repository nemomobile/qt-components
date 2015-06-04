Name:       qt-components-qt5

Summary:    Qt Quick UX Components
Version:    1.4.9
Release:    1
Group:      System/Libraries
License:    BSD
URL:        https://github.com/nemomobile/qt-components
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5OpenGL)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(mlite5)
BuildRequires:  desktop-file-utils
BuildRequires:  fdupes

%description
The Harmattan Qt Quick UX Components

%package devel
Summary:    Qt UX Components Devel
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
This package is part of Harmattan Qt Quick UX Components,
it contains the development files


%package gallery
Summary:    Qt Components gallery
Group:      Development/Applications
Requires:   %{name} = %{version}-%{release}

%description gallery
Provides test applications showcasing Qt Components widgets.


%prep
%setup -q -n %{name}-%{version}

%build
export QT_SELECT=5
./configure -meego -nomake tests
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
%fdupes  %{buildroot}/%{_libdir}

%files
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/Qt/labs/*
%{_libdir}/qt5/qml/com/meego/*
%{_libdir}/qt5/qml/com/nokia/*

%files devel
%defattr(-,root,root,-)
%{_datadir}/qt5/mkspecs/*

%files gallery
%defattr(-,root,root,-)
%{_bindir}/QmlComponentGallery5
%{_datadir}/applications/qmlcomponentgallery.desktop
