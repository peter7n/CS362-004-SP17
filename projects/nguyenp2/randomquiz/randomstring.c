Author: Peter Nguyen
Date: 4/30/17
CS362-400
Random Testing Quiz 2

In developing this solution, it is important to understand that in creating a random tester, purely random values do not work. One must generate random values that the tested function is expecting. Also, random values must be augmented with "values of interest" to the function being tested.

To display the "error" message in testme.c, first inputChar() must generate random characters in the sequence "[({ ax})]" in order to "unlock" states 1 through 9. Being in state 9 is a precondition for printing the "error" message. Once in state 9, inputString() must generate random string sequences that eventually spell out "reset."

Therefore, inputChar() and inputString() cannot generate purely random values or else the preconditions will never be met for printing the error message. The inputChar() function therefore generates random values in the set of "[({ ax})]" and the inputString() function generates random values only in the set of "reset" As previously stated, the random testers must generate values that the tested function is expecting to receive.
