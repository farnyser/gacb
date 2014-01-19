GACB
====

Proof of concept of a genetic algorithm to break the Vigenere cipher.


C version
----------

$ cd C

$ bash make.sh

$ cat test.txt | ./case lower | ./cipher azertyuiopqsdfghjklmwxcvbn0123456789 key42 | ./gacb 


Python version
--------------

$ cd Python

$ python pygacb.py mykey