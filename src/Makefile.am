## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = proyecto2_bmc_alineamientos.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(PROYECTO2_BMC_ALINEAMIENTOS_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = proyecto2_bmc_alineamientos

proyecto2_bmc_alineamientos_SOURCES = \
	main.c \
	proyecto2-bmc-alineamientos.h \
	proyecto2-bmc-alineamientos.c \
	Globals.h \
	Globals.c \
	AfterMatrixFilling.h \
	AfterMatrixFilling.c \
	Cell.c \
	Cell.h \
	CellFillingSupport.c \
	CellFillingSupport.h \
	AlignmentOptions.h \
	FillingAlgorithms.c \
	FillingAlgorithms.h	


proyecto2_bmc_alineamientos_LDFLAGS = \
	-Wl,--export-dynamic

proyecto2_bmc_alineamientos_LDADD = $(PROYECTO2_BMC_ALINEAMIENTOS_LIBS)




EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)

