## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = $(GEDA_AUTO_MAKE_OPTIONS)

INC_GEDA = -I$(top_srcdir) -I$(top_srcdir)/include -I$(top_srcdir)/intl
INC_LOCAL =  -I$(srcdir)/../include
INC_LIBGEDA = -I$(top_srcdir)/libgeda/include

GEDA_INCLUDES = $(INC_GEDA) $(INC_LOCAL) $(INC_LIBGEDA)

bin_PROGRAMS = gnetlist

# don't forget all *.h files */
gnetlist_SOURCES = \
		g_netlist.c \
		g_rc.c \
		g_register.c \
		globals.c \
		gnetlist.c \
		i_vars.c \
		parsecmd.c \
		s_cpinlist.c \
		s_hierarchy.c \
		s_misc.c \
		s_net.c \
		s_netattrib.c \
		s_netlist.c \
		s_rename.c \
		s_traverse.c \
		vams_misc.c

AM_CFLAGS            = $(GEDA_INCLUDES) -I$(includedir)

gnetlist_CPPFLAGS    =
gnetlist_CFLAGS      = $(AM_CFLAGS)    $(GLIB_CFLAGS)  $(GCC_CFLAGS) \
		       $(MINGW_CFLAGS) $(GUILE_CFLAGS)

gnetlist_LDFLAGS     = $(GLIB_LIBS) $(GUILE_LIBS)
gnetlist_LDADD       = $(top_builddir)/libgeda/src/libgeda.la

localedir            = @datadir@/locale
DEFS                 = -DLOCALEDIR=\"$(localedir)\" @DEFS@

doxygen:
# Change to the docs directory and 'make doxygen' for gnetlist
if ENABLE_DOXYGEN
	(cd ../docs; $(MAKE) doxygen) || exit 1;
else
	@echo 'configure was not run with the --enable-doxygen switch. To generate'
	@echo 'Doxygen documentation for gnetlist, run configure with --enable-doxygen'
	@echo 'in the top of the source tree and then try again'
endif

MOSTLYCLEANFILES     = *.log *.ps core FILE *~
CLEANFILES           = *.log *.ps core FILE *~
DISTCLEANFILES       = *.log      core FILE *~
MAINTAINERCLEANFILES = *.log *.ps core FILE *~ Makefile.in

