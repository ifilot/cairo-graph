#**************************************************************************
# *   Makefile                                                             *
# *                                                                        *
# *   AUTHOR:  Ivo Filot <ivo@ivofilot.nl>                                 *
# *   PROJECT: MINIGRAPH                                                   *
# *                                                                        *
# *   This program is free software; you can redistribute it and/or modify *
# *   it under the terms of the GNU General Public License as published by *
# *   the Free Software Foundation, version 2                              *
# *                                                                        *
# *   This program is distributed in the hope that it will be useful, but  *
# *   WITHOUT ANY WARRANTY; without even the implied warranty of           *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    *
# *   General Public License for more details.                             *
# *                                                                        *
# *   You should have received a copy of the GNU General Public License    *
# *   along with this program; if not, write to the Free Software          *
# *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA            *
# *   02110-1301, USA.                                                     *
# *                                                                        *
# **************************************************************************/

# set compiler and compile options
EXEC = mg
CXX = g++                                # use the GNU C++ compiler
OPTS = -O3 -Wall -g -Wno-write-strings   # use some optimization, report all warnings and enable debugging
CFLAGS = $(OPTS)                         # add compile flags
LDFLAGS =  -lcairo -lpcrecpp             # specify link flags here

# set a list of directories
INCDIR =./include
OBJDIR = ./obj
BINDIR = ./bin
SRCDIR = ./src

# set the include folder where the .h files reside
CFLAGS += -I$(INCDIR) -I$(INCDIR_LAMMPS) -I$(SRCDIR)

# add here the source files for the compilation
SOURCES = minigraph.cpp plotter.cpp graph.cpp lexical_casts.cpp parser.cpp

# create the obj variable by substituting the extension of the sources
# and adding a path
_OBJ = $(SOURCES:.cpp=.o)
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

#
# Add additional directives for OSX
#
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
CFLAGS += -I/opt/local/include
LDFLAGS += -L/opt/local/lib -lpcre
endif

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJ)
	$(CXX) -o $(BINDIR)/$(EXEC) $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

test: $(BINDIR)/$(EXEC)
	$(BINDIR)/$(EXEC) -i test/dataset01.dat -o test_m1.png
	cat test/dataset01.dat | $(BINDIR)/$(EXEC) -o test_m2.png

doc: tex/manual.tex
	pdflatex tex/manual.tex
	pdflatex tex/manual.tex
	mv manual.pdf doc/
	rm -v manual.aux manual.log manual.toc

clean:
	rm -vf $(BINDIR)/$(EXEC) $(OBJ) *.png
