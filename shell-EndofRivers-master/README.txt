As per the project documentation, I will describe my approach to each requirement. I will go down the bullets in order in this README file.

Before doing any work, I scoured the internet for resources that would be useful. I found a few shells, played around with them, read through their source code, etc. This helped me understand the general flow of what a shell should provide. Whenever I came across a function I did not know, I would read over the documentation on it. For example, fgetc() and getchar() were both used in my shell. I saw them in use in the utils.h and in other sources around the internet. They were very useful in reading in the file and user input.

If run with no arguments.
-----------------------------------
This was my first success. Reading in input ended up being easy. Between documentation, basic C programming tutorials like GeeksforGeeks, and monitoring piazza I finished this in about an hour. After taking input, I had to tokenize the string. This was honestly pretty simple, basic tokenizing delim on spaces is easy. Read up on strtok() and wrapped that up pretty quickly. Then on to fork/exec, using the in class code and advice from a TA on path resolution, I used execvp() and was successful at running basic commands. I had to do some tweeking to make sure it exits properly then I was finished.

If run with one argument
-----------------------------------
This took the longest for me. I kept deleting my code on this when frusterating and starting over the next day which ended up setting me back. Once I got into the right headspace, I augmented my other code to work with a file input as well. This ended up successful. Passing in a bool to slightly alter the functions and I was finished quickly. I used the same tokenizing and exec function for both methods.

If run with more than one argument
-----------------------------------
I was already familiar with argc and argv main params so this was trivial. Not much to really say here.

Arbitrary Length
-----------------------------------
Simply reallocating the buffers when a limit was hit wasn't a new concept to me. I have done this in C++ in 202/341 and as a TA in 202 went writing projects/etc. I simply had to look up how to do it in C and I was finished.

Paths, Escape Sequences and quoting
------------------------------------
Using execvp deals with the pathing, I used unescape for the sequences and quoting. As I am writing this, I need to run tests to see if these are being supported properly. I know I have had success but I am unsure about the quoting.

Proper Exits
------------------------------------
I used exit(EXIT_SUCCESS) and exit(EXIT_FAILURE) when needed. I believe this satifies all the proper exits.
