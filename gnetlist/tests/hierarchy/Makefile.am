## Process this file with automake to produce Makefile.in

EXTRA_DIST = 1217.geda bottom.sch bottom.sym gnetlistrc.hierarchy gschemrc \
	     gschlasrc hierarchy.geda middle.sch middle.sym rock.sch rock.sym \
	     top.sch


# Temporarily disabled make check, since this is interfering with
# make distcheck
check_SCRIPTS = tests

BUILDDIR=$(top_builddir)/gnetlist/tests/hierarchy
SRCDIR=$(srcdir)
GNETLIST=$(top_builddir)/gnetlist/src/gnetlist

tests:

notests:
	rm -f $(BUILDDIR)/new_*
	cp -f $(SRCDIR)/gnetlistrc.hierarchy $(BUILDDIR)/gnetlistrc
	( TESTDIR=$(SRCDIR) \
	  GEDADATARC=$(top_builddir)/gnetlist/lib \
	  SCMDIR=${top_builddir}/gnetlist/scheme \
	  SYMDIR=$(top_srcdir)/symbols \
	    $(GNETLIST) \
	      -L $(top_srcdir)/libgeda/scheme \
	      -L $(top_builddir)/libgeda/scheme \
	      -o $(BUILDDIR)/new_hierarchy.geda \
	      -g geda $(SRCDIR)/top.sch )
	diff $(SRCDIR)/hierarchy.geda $(BUILDDIR)/new_hierarchy.geda;
	rm -f $(BUILDDIR)/gnetlistrc

MOSTLYCLEANFILES = new_* core *.log FILE *.ps *~ gnetlistrc
CLEANFILES = new_* core *.log FILE *.ps *~ gnetlistrc
DISTCLEANFILES = *.log core FILE *~ gnetlistrc
MAINTAINERCLEANFILES = new_* core *.log FILE *.ps *~ Makefile.in gnetlistrc
