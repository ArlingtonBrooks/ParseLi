# ParseLi
ParseLi is a library for parsing "input files".  These input files can be theoretically for anything, however must contain some of the following elements: 

## COMMENTS
A comment line is any line starting with a '#'.  You should be able to write whatever you want in a comment line, the parser will simply ignore it.

## INTEGERS
An integer is any number without a decimal point. 
    Example: 
    SOMEINT 1
The above line will create a variable named "SOMEINT" with a value of 1.
