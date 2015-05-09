# Minigraph

## Introduction
Minigraph is a tiny tool for converting raw XY data to miniature graphs. 
These graphs have been optimised to be small. A typical graph looks like:

![Example graph](https://github.com/ifilot/minigraph/blob/master/tex/img/example_graph.png "Example graph")

## Installation
Minigraph depends on a couple of libraries, which are normally directly available
by your favorite package manager.


* Cairo
* PCRECPP
* TCLAP

For instance, on Debian you can install these packages by:
```
sudo apt-get install libcairo2-dev libpcre3-dev libtclap-dev
```

To install the program, simply run:
```
make
```

To test the program, run:
```
make test
```

## Usage
The program requires either a stream or a file as input. Both of the commands
below should in principle work:
```
cat data.dat | mg -o graph.png -p -l
```
or
```
mg -i data.dat -o graph.png -p -l
```

You can tune the behaviour of minigraph by setting certain switches

Switch        | Behaviour
------------- | -------------
-l            | Plot lines
-p            | Plot points

## Todo
- [ ] Enable configuration of colors
- [ ] Add bargraphs
- [ ] Add splines

## License
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
