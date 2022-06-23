AM_CFLAGS = -std=gnu11 -Wall -Wextra
AM_LDFLAGS =
AM_YFLAGS = -d -t

BUILT_SOURCES = src/provolone_parser.h
bin_PROGRAMS = provolone
provolone_CPPFLAGS = -I$(builddir)/src -I$(top_srcdir)/src
provolone_SOURCES = src/provolone.c src/provolone_parser.y src/provolone_lexer.l src/provolone_program.c src/provolone_compiler.c src/llist.c

