dnl -*-Autoconf-*-
dnl Process this file with autoconf to generate configure.
dnl
dnl The AX_* macros are defined in files in the top-level m4
dnl directory.

#####################################################################
# Set up Version
#####################################################################
m4_define([geda_major_version],   [2])
m4_define([geda_minor_version],   [1])
m4_define([geda_micro_version],   [0])
m4_define([geda_mutation],        [14])

m4_define([geda_version],
[geda_major_version.geda_minor_version.geda_micro_version.geda_mutation])

#####################################################################
# Set up configuration system
#####################################################################

AC_INIT([gEDA/gaf], [geda_version], [https://bugs.launchpad.net/geda], [geda-gaf])
AC_PREREQ([2.60])dnl

AC_CONFIG_SRCDIR([version.h.in])
AC_CONFIG_AUX_DIR([build-tools])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

# Honor aclocal flags
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

# The current configuration builds with Automake 1.9, but Distro checks
# are performed using Automake 1.11,
# use update-alternatives --config automake to switch on debian systems
# to generate with Automake 1.9 use:
#AM_INIT_AUTOMAKE

# to generate with Automake 1.11 use:
AM_INIT_AUTOMAKE([parallel-tests color-tests])

AX_SET_AM_OPTIONS

AC_GNU_SOURCE # This enable extensions on GNU/Linux platforms (used by intl)

#####################################################################
# Do version number magic
#####################################################################

AX_GIT_VERSION([20151204])

# This is used for keeping the ChangeLog files up-to-date
AC_SUBST([CHANGELOG_BASE], [1.0-20151204])

#####################################################################
# Windows/MinGW/Cygwin support
#####################################################################

AX_WINDOWS_FLAGS

#####################################################################
# Check for compiling & linking tools
#####################################################################
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP

m4_ifdef([AM_PROG_AR], [AM_PROG_AR]) dnl Workaround for Automake 1.11

AC_LIBTOOL_WIN32_DLL
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Last flag added is first flag on command line
AX_GCC_FLAGS([-Wl,--as-needed])
AX_GCC_FLAGS([-Wformat-security])
AX_GCC_FLAGS([-fstack-protector])
AX_GCC_FLAGS([-D_FORTIFY_SOURCE=2])
AX_GCC_FLAGS([-O2 -Wall])

#####################################################################
# Check for existence of utilitity Programs required for building
#####################################################################

AX_CHECK_PROGRAMS

#####################################################################
# Check Debug Options and enable Debugging Tools
#####################################################################

AX_CHECK_DEBUGGING

#####################################################################
# Check for standard header files and specific functions
#####################################################################

AX_GEDA_STD_CHECKS

#####################################################################
# Internationalisation
#####################################################################

AM_NLS
AM_GNU_GETTEXT
AM_GNU_GETTEXT_VERSION([0.16])dnl
AX_DESKTOP_I18N

#####################################################################
# Linker flags...
#####################################################################
AX_CHECK_LD_FLAGS

#####################################################################
# Check for libraries that use pkg-config
#####################################################################

PKG_PROG_PKG_CONFIG

#####################################################################
# Guile stuff
#####################################################################

AX_CHECK_GUILE([1.8.0])

#####################################################################
# GLIB
#####################################################################

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.16.0], ,
  AC_MSG_ERROR([glib-2.0 2.16.0 or later is required.]))dnl

#####################################################################
# GTK, GDK, Pango & Cairo stuff
#####################################################################

AX_CHECK_GTK([2.16.0])

AX_CHECK_CAIRO

PKG_CHECK_MODULES(PANGOCAIRO, [pangocairo >= 0.26], ,
  AC_MSG_ERROR([PANGOCAIRO 0.26 or later is required.]))dnl

AX_CHECK_PANGO

#####################################################################
# Multitasking and Threading ( Not checked by GTK above )
#####################################################################
AX_CHECK_X11

#####################################################################
# Python stuff
#####################################################################

PYTHON_MIN_VERSION=2.6.1
PYTHON_MUTAGEN_MIN_VERSION=1.8

AX_CHECK_PYTHON($PYTHON_MIN_VERSION)

#####################################################################
# Other Optional things
#####################################################################

# Assertions
AX_OPTION_ASSERT

# Viewing documentation
AX_OPTION_URI_VIEWER

# API documentation
AX_OPTION_DOXYGEN

# XDG desktop files
AX_OPTION_XDG_DATA

# KDE 3 desktop files
AX_OPTION_KDE3_DATA

# XDG database updates
AX_OPTION_XDG_DB

# libstroke support
AX_OPTION_STROKE

# gattrib
AX_OPTION_GATTRIB

#####################################################################
# Source Directories and Target Locations
#####################################################################

# Setup the symbol source directories
AX_SYM_DIRS

# Setup Target installation directories
AX_GEDA_DIRS

#####################################################################
# Setup version number for libraries
#####################################################################

# Set up libgeda with the correct ld version number
AX_LIBGEDA([51:2:0])

# Set up libgedadraw with the correct ld version number
AX_LIBGEDADRAW([0:3:0])

# Set up libgedacairo with the correct ld version number
AX_LIBGEDACAIRO([1:6:1])

# Set up libgedathon with the correct ld version number
AX_LIBGEDATHON([0:7:0])

# Set up libgedauio with the correct ld version number
AX_LIBGEDAUIO([0:7:0])

#####################################################################
# Generate output
#####################################################################
#
# Note: 1.) Files requiring @varible@ substitution are to be included
#           in this list.

AC_CONFIG_FILES([Makefile
                 bitmaps/Makefile
                 include/Makefile
                 intl/Makefile

                 libgeda/Makefile
                 libgeda/libgeda.pc
                 libgeda/po/domain.mak
                 libgeda/po/Makefile.in
                 libgeda/shell/Makefile
                 libgeda/data/Makefile
                 libgeda/docs/Makefile
                 libgeda/docs/images/Makefile
                 libgeda/include/Makefile
                 libgeda/etc/Makefile
                 libgeda/scheme/Makefile
                 libgeda/share/Makefile
                 libgeda/src/file/Makefile
                 libgeda/src/geda/Makefile
                 libgeda/src/guile/Makefile
                 libgeda/src/iface/Makefile
                 libgeda/src/math/Makefile
                 libgeda/src/object/Makefile
                 libgeda/src/scheme/Makefile
                 libgeda/src/struct/Makefile
                 libgeda/src/utility/Makefile
                 libgeda/src/Makefile

                 libgedacairo/Makefile
                 libgedacairo/libgedacairo.pc
                 libgedacairo/docs/Makefile
                 libgedacairo/docs/images/Makefile
                 libgedacairo/include/Makefile
                 libgedacairo/src/Makefile

                 libgedadraw/Makefile
                 libgedadraw/libgedadraw.pc
                 libgedadraw/docs/Makefile
                 libgedadraw/docs/images/Makefile
                 libgedadraw/include/Makefile
                 libgedadraw/src/Makefile

                 libgedathon/Makefile
                 libgedathon/geda.pc
                 libgedathon/libgedathon.pc
                 libgedathon/docs/Makefile
                 libgedathon/docs/images/Makefile
                 libgedathon/include/Makefile
                 libgedathon/info/Makefile
                 libgedathon/module/objects/Makefile
                 libgedathon/module/Makefile
                 libgedathon/scripts/Makefile
                 libgedathon/src/Makefile
                 libgedathon/tests/Makefile

                 libgedauio/Makefile
                 libgedauio/libgedauio.pc
                 libgedauio/po/Makefile.in
                 libgedauio/docs/Makefile
                 libgedauio/docs/images/Makefile
                 libgedauio/include/Makefile
                 libgedauio/src/dialogs/Makefile
                 libgedauio/src/widgets/Makefile
                 libgedauio/src/Makefile

                 gaf/Makefile
                 gaf/po/Makefile.in
                 gaf/docs/Makefile
                 gaf/include/Makefile
                 gaf/src/Makefile

                 gschem/Makefile
                 gschem/po/Makefile.in
                 gschem/bitmap/Makefile
                 gschem/etc/Makefile
                 gschem/examples/Makefile
                 gschem/data/Makefile
                 gschem/docs/Makefile
                 gschem/docs/images/Makefile
                 gschem/docs/dialogs/Makefile
                 gschem/include/Makefile
                 gschem/scripts/Makefile
                 gschem/src/base/Makefile
                 gschem/src/guile/Makefile
                 gschem/src/iface/Makefile
                 gschem/src/math/Makefile
                 gschem/src/object/Makefile
                 gschem/src/x/Makefile
                 gschem/src/Makefile
                 gschem/tests/Makefile
                 gschem/scheme/Makefile

                 gattrib/Makefile
                 gattrib/po/Makefile.in
                 gattrib/data/Makefile
                 gattrib/design/Makefile
                 gattrib/docs/Makefile
                 gattrib/include/Makefile
                 gattrib/include/gtksheet/Makefile
                 gattrib/etc/Makefile
                 gattrib/etc/system-gattribrc
                 gattrib/src/Makefile
                 gattrib/src/gtksheet/Makefile

                 gsymcheck/docs/Makefile
                 gsymcheck/include/Makefile
                 gsymcheck/etc/Makefile
                 gsymcheck/etc/system-gsymcheckrc
                 gsymcheck/po/Makefile.in
                 gsymcheck/src/Makefile
                 gsymcheck/tests/Makefile
                 gsymcheck/Makefile

                 gnetlist/Makefile
                 gnetlist/po/Makefile.in
                 gnetlist/src/Makefile
                 gnetlist/include/Makefile
                 gnetlist/scheme/Makefile
                 gnetlist/examples/Makefile
                 gnetlist/tests/Makefile
                 gnetlist/tests/common/Makefile
                 gnetlist/tests/hierarchy/Makefile
                 gnetlist/tests/hierarchy2/Makefile
                 gnetlist/tests/drc2/Makefile
                 gnetlist/tests/scripts/Makefile

                 gnetlist/docs/Makefile
                 gnetlist/docs/vams/Makefile
                 gnetlist/examples/vams/Makefile
                 gnetlist/examples/vams/vhdl/Makefile
                 gnetlist/examples/vams/vhdl/basic-vhdl/Makefile
                 gnetlist/examples/vams/vhdl/new-vhdl/Makefile
                 gnetlist/examples/switcap/Makefile
                 gnetlist/etc/Makefile
                 gnetlist/etc/system-gnetlistrc
                 gnetlist/utils/Makefile
                 gnetlist/scripts/Makefile
                 gnetlist/scripts/annotate.sh
                 gnetlist/scripts/bompp.sh
                 gnetlist/scripts/bom_xref.sh

                 utils/Makefile
                 utils/docs/Makefile
                 utils/examples/Makefile
                 utils/examples/gmk_sym/Makefile
                 utils/examples/gsch2pcb/Makefile
                 utils/examples/tragesym/Makefile
                 utils/gmake/Makefile
                 utils/include/Makefile
                 utils/gsch2pcb/Makefile
                 utils/gsch2pdf/Makefile
                 utils/gschemdoc/Makefile
                 utils/gschlas/Makefile
                 utils/gxyrs/Makefile
                 utils/etc/Makefile
                 utils/etc/system-gschlasrc
                 utils/scripts/Makefile

                 utils/scripts/garchive.py
                 utils/scripts/geda-install.sh
                 utils/scripts/geda-install-bin.sh

                 utils/src/Makefile
                 utils/tests/Makefile
                 utils/tests/gxyrs/Makefile
                 utils/tests/gxyrs/inputs/Makefile
                 utils/tests/gxyrs/outputs/Makefile
                 utils/tests/refdes_renum/Makefile
                 utils/tests/refdes_renum/inputs/Makefile
                 utils/tests/refdes_renum/outputs/Makefile

                 symbols/Makefile

                 docs/Makefile
                 docs/images/Makefile
                 docs/scheme-api/Makefile
                 docs/symbols/Makefile
                 docs/toplevel/Makefile
                 docs/toplevel/gedadocs.html
                 docs/wiki/Makefile

                 examples/Makefile
                 examples/gTAG/Makefile
                 examples/lightning_detector/Makefile
                 examples/RF_Amp/Makefile
                 examples/RF_Amp/model/Makefile
                 examples/RF_Amp/sym/Makefile
                 examples/TwoStageAmp/Makefile
])
AC_OUTPUT

AX_EXPAND_DIRS

dnl ==========================================================================
echo "
Configuration:      ${PACKAGE} Ver ${VERSION} ${DATE_VERSION}
                 -------------------------------------
                        Compiler:     ${CC} ${GCC_CFLAGS}
                        CPPFLAGS:     ${CPPFLAGS}
                          CFLAGS:     ${CFLAGS}
                        CXXFLAGS:     ${CXXFLAGS}
                         LDFLAGS:     ${LDFLAGS}
                         LibTool:     LT_PACKAGE_VERSION
                   PYTHON_CFLAGS:     ${PYTHON_CFLAGS}
                  PYTHON_LDFLAGS:     ${PYTHON_LDFLAGS}

                           Guile:     ${GUILE_PKG_NAME}
                           Cairo:     ${CAIRO_PKG}
                           Pango:     ${PANGO_PKG}
                          Python:     ${PYTHON_VERSION}
                       PyModules:     ${PYTHON_MODDIR}

                  LibGeda_Domain:     ${LIBGEDA_GETTEXT_DOMAIN}
                    LibGedaCairo:     ${LIBGEDACAIRO_SHLIB_VERSION}
                      LibGedaUIO:     ${LIBGEDAUIO_SHLIB_VERSION}"
echo "
                     Destination:     ${EXPANDED_PREFIX}
                     exec_prefix:     ${EXPANDED_EXEPREFIX}
                          libdir:     ${EXPANDED_LIBDIR}
                      libexecdir:     ${EXPANDED_LIBEXECDIR}
                          bindir:     ${EXPANDED_BINDIR}

                      RC scripts:     ${GEDARCDIR}		f_path_sys_config()
                            Data:     ${GEDADATADIR}		f_path_sys_data()
                    Documentaion:     ${GEDADOCDIR}     	f_path_sys_doc()
                       Log files:     ${GEDALOGDIR}

              PCB data directory:     ${PCBDATADIR}
                PCB M4 directory:     ${PCBM4DIR}
         PCB Libraries directory:     ${PCBLIBPATH}
"

