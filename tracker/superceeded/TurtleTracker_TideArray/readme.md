# TurtleTracker_TideArray


## Generating Documentation
The Arduino sketch is documented using [Doxygen](https://www.doxygen.nl/index.html).


`\\\ inline comments are placed before code to appear in the documentation`

`\\\< comments are placed after the code to appear in the documentation`

The doxyfile contains the settings to generate the documentation. To generate the documentation navigate to the code folder and run `Doxygen`

This creates an HTML folder, to view the documentation open index.html in a browser.


## Installing Doxygen

Installation of Doxygen on mac is as follows. For other operating systems refer to [installation instructions](https://www.doxygen.nl/manual/install.html)

First, install a package manager, I'm using [Homebrew](http://brew.sh) for MacOS. To install

On MacOS install Homebrew by opening a terminal and paste the following.


`/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)`

To install Doxygen for generating documentation from code and graphviz, a dependancy for generating flow diagrams run the following commands:


`brew install doxygen`
`brew install graphviz`
