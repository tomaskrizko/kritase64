AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = kritase64
SUBIDRS = src
kritase64_SOURCES = src/main.cpp src/kritase64.cpp
dist_doc_DATA = README.md LICENSE.txt
dist_data_DATA = src/kritase64.hpp

clean-local:
	@rm config.status configure config.log
	@rm Makefile
	@rm -r autom4te.cache/
	@rm aclocal.m4
	@rm compile install-sh missing Makefile.in