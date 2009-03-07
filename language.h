
The WITH language is a very simple language for writing simple programs.
WITH was designed by Richard Marks in 2009 for the purpose of exploring programming language design.

There is only a single function in the WITH language, and it is called "with".
The language is implementation dependent, and any code written for one implementation may or may not execute for another implementation.


WITH <TARGET> DO [CONDITIONS] <ACTION> [<PARAMETERS>];

What you may use for the TARGET, ACTION, and PARAMETERS is implementation-dependent.

I will define a few TARGETs with a few ACTIONs so that you can get started with writing your own implementation of WITH.

%START TARGET
%ID system
%ACTIONS exit wait
%END TARGET

%START ACTION
%PARENT system
%ID exit
%SOURCE system-exit.cpp
%PARAMS NONE
%END ACTION

%START ACTION
%PARENT system
%ID wait
%SOURCE system-wait.cpp
%PARAMS INTEGER
%END ACTION

%START TARGET
%ID io
%ACTIONS read write
%END TARGET

%START ACTION
%PARENT io
%ID read
%SOURCE io-read.cpp
%PARAMS STRING_REFERENCE
%END ACTION

%START ACTION
%PARENT io
%ID write
%SOURCE io-write.cpp
%PARAMS STRING
%END ACTION

Now that we are finished defining our TARGETs and ACTIONs, we can start to use them to write a simple
Hello-World program, that writes the words "Hello, World!" on the screen, and then waits 5 seconds before ending.

# hello.wth
# A simple Hello-World program in the WITH programming language
# by Richard Marks <ccpsceo@gmail.com>
with io do write "Hello, World!";
with system do wait 5000;
with system do exit;

A little more complex example, is another Hello-World program, this time asks you for your name, and greets you.
The program then waits for 5 seconds before a goodbye message is written on the screen, and the program ends.

We need another target and action:

%START TARGET
%ID string
%ACTIONS cat
%END TARGET

%START ACTION
%PARENT string
%ID cat
%SOURCE string-cat.cpp
%PARAMS STRING STRING STRING_REFERENCE
%END ACTION


# hello2.wth
# Another Hello-World program in the WITH programming language
# by Richard Marks <ccpsceo@gmail.com>

# declare a few new variables of type STRING
$name;
$greeting;

# ask the user for their name
with io do write "What is your name?";

# get the user's name and store it into the $name variable
with io do read @$name;

# concatenate the user's name to the end of the string "Hello, "
# and store the result into the $greeting variable
with string do cat "Hello, " $name @$greeting;

# concatenate the string "!" to the end of the string in the $greeting variable
# and store the result into the $greeting variable
with string do cat "!" $greeting @$greeting;

# write the greeting string on the screen
with io do write $greeting;

# wait 5 seconds and end the program after we say goodbye to the user
with system do wait 5000;
with io do write "Goodbye!";
with system do exit;


That was pretty easy right?
Lets have a look at the optional CONDITIONS construct of the WITH function.

There are 2 types of conditions; DO IF, and DO IFNOT.

A CONDITION consists of 3 things; a SUBJECT, an OPERATOR, and a VALUE.

The SUBJECT can be any PROPERTY of a TARGET.
We haven't seen any TARGETs with properties yet, so I'll define one for you a little later.

The OPERATOR can be any of the following:
	<	- the SUBJECT is less-than the VALUE
	>	- the SUBJECT is greather-than the VALUE
	=	- the SUBJECT is equal-to the VALUE
	<=	- the SUBJECT is less-than or equal-to the VALUE
	>=	- the SUBJECT is greater-than or equal-to the VALUE
	!=	- the SUBJECT is not equal to the VALUE

VALUE can be any literal value such as an integer or a string, or any SUBJECT.

How about we write a simple number guessing game in WITH?

In order to do this, we need to first identify what TARGETs and ACTIONs we will need.

* generate a random integer
* write text on the screen
* get an integer from the user

I'll assume that we already have all the defined TARGETs and ACTIONs from before,
and I'll add the new ones we need.

%UPDATE TARGET
%ID io
%ACTIONS readint
%END TARGET

%START ACTION
%PARENT io
%ID readint
%SOURCE io-readint.cpp
%PARAMS INTEGER_REFERENCE
%END ACTION

%UPDATE TARGET
%ID system
%ACTIONS randomint
%END TARGET

%START ACTION
%PARENT system
%ID randomint
%SOURCE system-randomint.cpp
%PARAMS INTEGER INTEGER INTEGER_REFERENCE
%END ACTION

Notice that we can add new ACTIONs to existing TARGETs by changing the START directive to UPDATE.
We have all that we need now to write a number guessing game in the WITH programming language!
Lets get started!


# guess.wth
# Simple number guessing game in the WITH programming language
# by Richard Marks <ccpsceo@gmail.com>

# include the code in stdio.wth and string.wth into this file
# so that we can reuse our previously defined TARGETs and ACTIONs.
%USE stdio;
%USE string;

# declare the variables we will use
$number;
$guessCounter;
$guess;

# define the function that we need to have to create the infinite loop
# the function takes no parameters, and is called "main".
%START FUNCTION
%ID main
%PARAMS NONE
	# will exit the function if the guessCounter variable's value is >= five
	with main do if $guessCounter >= 5 return;
	# get the guess from the user
	with io do newline;
	with io do write "What is your guess?";
	with io do read @$guess;
	# tell the user if they are too low or too high for their guess
	with io do if $guess < $number write "Nope! Too low!";
	with io do if $guess > $number write "Nope! Too high!";
	# exit the function if the guess is correct
	with main do if $guess = $number return;
	# increment the guessCounter variable
	with $guessCounter do inc;

%END FUNCTION

# get a random integer from 1 to 100 and store it in the $number variable
with system do randomint 1 100 @$number;
# a little intro text
with io do write "GUESSING GAME";
with io do write "Created by Richard Marks <ccpsceo@gmail.com> with the WITH programming language.";
with io do newline;
with io do write "I'm thinking of a number between 1 and 100...";
with io do write "Guess my number in 5 or less tries, and you win!";
# we are telling the main function to be called until we specifically return from the function.
with do do while @main;
# finish up
with io do if $guess = $number write "You guessed my number! You Win!";
with io do if $guess != $number write "You did not guess my number! You Lose!";
with system do wait 5000;
with io do write "Thanks for playing!";
with system do exit;

################################################################################

Directives:

%<START|UPDATE> <TARGET|ACTION|FUNCTION|CLASS>
%END <TARGET|ACTION|FUNCTION|CLASS>
%ID <identifier>
%PARENT <identifier>
%SOURCE <implementation source filename>
%PARAMS <parameter list>
%ACTIONS <action list>
%TARGETS <target list>
%USE <library name> [AS <new alias for library name>] ;

Action Parameter Types:

INTEGER
INTEGER_REFERENCE
STRING
STRING_REFERENCE
REAL
REAL_REFERENCE
VARARG

variable declaration:

$variableName;

The address-of operator '@' is used when we need to use references to a variable or function.

$name;
with std do readline @$name;


$user;
with std do write "What is your name?";
with std do readline @$user;
with std do print "Hello, %s" $user;
with sys do exit 0;

System Library:


################################################################################
# stdlib.wth
# WITH Standard Library
# Developed by Richard Marks <ccpsceo@gmail.com>
# (C) Copyright 2009, CCPS Solutions
# http://www.ccpssolutions.com
################################################################################
%START TARGET
%ID std
%ACTIONS print scan read write readline newline stoi stor rtos itos rnd
	%START ACTION
		%ID print
		%SOURCE std-print.cpp
		%PARAMS STRING VARARG
	%END ACTION
	

Standard Library:

################################################################################
# stdlib.wth
# WITH Standard Library
# Developed by Richard Marks <ccpsceo@gmail.com>
# (C) Copyright 2009, CCPS Solutions
# http://www.ccpssolutions.com
################################################################################
%START TARGET
%ID std
%ACTIONS print scan read write readline newline stoi stor rtos itos rnd
	%START ACTION
		%ID print
		%SOURCE std-print.cpp
		%PARAMS STRING VARARG
	%END ACTION
	
	%START ACTION
		%ID scan
		%SOURCE std-scan.cpp
		%PARAMS STRING VARARG
	%END ACTION
	
	%START ACTION
		%ID read
		%SOURCE std-read.cpp
		%PARAMS STRING_REFERENCE
	%END ACTION
	
	%START ACTION
		%ID write
		%SOURCE std-write.cpp
		%PARAMS STRING
	%END ACTION
	
	%START ACTION
		%ID readline
		%SOURCE std-readline.cpp
		%PARAMS STRING_REFERENCE
	%END ACTION
	
	%START ACTION
		%ID newline
		%SOURCE std-newline.cpp
		%PARAMS NONE
	%END ACTION
	
	%START ACTION
		%ID stoi
		%SOURCE std-stoi.cpp
		%PARAMS STRING INTEGER_REFERENCE
	%END ACTION
	
	%START ACTION
		%ID stor
		%SOURCE std-stor.cpp
		%PARAMS STRING REAL_REFERENCE
	%END ACTION
	
	%START ACTION
		%ID rtos
		%SOURCE std-rtos.cpp
		%PARAMS REAL STRING_REFERENCE
	%END ACTION
	
	%START ACTION
		%ID itos
		%SOURCE std-itos.cpp
		%PARAMS INTEGER STRING_REFERENCE
	%END ACTION
	
	%START ACTION
		%ID rnd
		%SOURCE std-rnd.cpp
		%PARAMS INTEGER INTEGER INTEGER_REFERENCE
	%END ACTION
	
%END TARGET
















