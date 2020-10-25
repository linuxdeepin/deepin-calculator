Name:           deepin-calculator
Version:        5.6.0.3
Release:        1%{?dist}
Summary:        An easy to use calculator for ordinary users
License:        GPLv3
URL:            https://github.com/linuxdeepin/deepin-calculator
Source0:        %{url}/archive/%{version}/%{name}-%{version}.tar.gz

BuildRequires:  qt5-linguist
BuildRequires:  cmake
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5Svg)
BuildRequires:  pkgconfig(dtkwidget) >= 2.0
BuildRequires:  pkgconfig(Qt5Multimedia)
BuildRequires:  pkgconfig(Qt5X11Extras)
BuildRequires:  pkgconfig(dframeworkdbus)
BuildRequires:  desktop-file-utils
Requires:       hicolor-icon-theme

%description
%{summary}.

%prep
%autosetup -p1
sed -i 's|59 Temple Place, Suite 330|51 Franklin Street, Fifth Floor|;
        s|Boston, MA 02111-1307 USA.|Boston, MA 02110-1335, USA.|' src/math/*.{c,h}

%build
%cmake
%cmake_build

%install
%cmake_install

%check
desktop-file-validate %{buildroot}%{_datadir}/applications/%{name}.desktop

%files
%doc README.md
%license LICENSE
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg

%changelog
