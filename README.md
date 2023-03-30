# ParseLi
ParseLi is a library for parsing "input files".  These input files can be theoretically for anything, or you can leave the input file blank and specify variables using a dictionary object.  An input file is loaded using the provided ReadConfig() function.  

### EXAMPLE INPUT FILE
In the top level directory is a working example input file `Example.in`. 

### COMMENTS
A comment line is any line starting with a '#'.  You should be able to write whatever you want in a comment line, the parser will simply ignore it. <br>
Example: <br>
>    #This is a comment

The above line will leave a comment in your input file which will not be parsed.  This may be useful for removing or modifying variables from an input file while leaving a trace of what was modified or removed.

### INTEGERS
An integer is any number without a decimal point. <br>
Example: <br>
>    SOMEINT 1

The above line will create a variable named "SOMEINT" with a value of 1.

### DOUBLES
A double is a number with a decimal place.  ParseLi stores numbers with decimal points via the `double` C++ type (i.e. a double-precision floating point number).  <br>
Example: <br>
>    SOMEFLOAT 1.0 <br>
>    AnotherFloat 1.0e-5

Both of the above formats are accepted and will return the expected result.  

### STRINGS
A string is a collection of characters in some order.  ParseLi stores this with the standard C++ `string` type.  <br>
Example: <br>
>    SOMESTRING Something <br>
>    ANOTHERSTRING Something Else

Both of the above formats are accepted and will return the expected result.  Note that string values may not cross multiple lines. 

### INCLUDING OTHER INPUT FILES
Using the `include` keyword, you can specify a filename to be loaded in addition to the file currently being loaded.  When the `include` keyword is processed, processing of the current file pauses until the `include`d file has been loaded. <br>
Example: <br>
>    include Somefile

Note that the filename must not contain any spaces.

### ENFORCING A VALUE
In tandem with `include`, there is a keyword for `enforce` so that `include`d files can be made to match certain parameters.  This is especially useful if an `include`d file only applies to certain conditions on the selected input file and any other condition will cause problems.  <br>
Using `enforce` creates a string type object, so if you intend to enforce a `double`, it must be specified twice: once as a double, and once as an `enforce`.  The same applies to integers.  <br>
Example: <br>
>    enforce SomeImportantProperty1 5 #this will not create an integer object <br>
>    enforce SomeImportantProperty2 SomeString
<br>
Currently enforcement does not work as intended and does not protect against future values overwriting current values.  This is not a high priority to be fixed, however.  

# FORMATTING TIPS
The formatting is mostly up to you, however you **should avoid leaving comments on the same line as variables** to avoid that variable being incorrectly written or mistakenly set as the wrong type.  

# READING AN INPUT FILE
First, you must create a dictionary object.  
This is performed using the built-in `Dict` class which is described in the header.  
You may read an input file by calling the `ReadConfig` function which returns `true` upon a successful read.  You are expected to supply the filename, a pointer to a dictionary object, and (optionally) a boolean to define whether to print debug messages.

# ADDING VALUES TO DICTIONARY
If you want to add values to your dictionary which weren't previously defined, you may do so by using the `Dict::add` function which should automatically detect whether you are supplying a `double`, an `int`, or an `std::string`.  
When defining a custom value to add, it is possible to specify the variable name with a space (for example: `MY VARIABLE`), however this is bad practise.  

# READING VALUES FROM DICTIONARY
To return a value from the dictionary object, simply call either `Dict::GetDouble`, `Dict::GetInt`, or `Dict::GetString` (depending on what you are trying to get).  
Note that you must supply a key which matches exactly the variable definition from your input file.  
If ParseLi cannot find the value for the corresponding key, it will throw an exception notifying you of the error.  
