# Password-Strength-Test
This repository contains a C program designed to assess the strength of passwords and passphrases based on a set of predefined rules. The program was developed as a project for the COP 3515 course.

Program Overview
XYZ Corporation faced a security breach due to weak employee passwords. To solve this issue, the program implements a password strength meter that evaluates whether a password or passphrase is strong according to specific criteria. The tool reads proposed passwords from a file and checks them against mandatory and optional tests.

Key Features:
Password Strength Evaluation:

Required Tests:
Minimum and maximum length enforcement.
Prevention of repeated sequences of three or more characters.
Verification that passwords contain letters, numbers, and symbols.
Passphrase Detection:

Detects passphrases (strings with spaces) and evaluates their strength.
Can enforce a minimum passphrase length if allowed.
Optional Tests (configurable):

Check for lowercase and uppercase letters.
Check for at least two digits and two special characters.

How to Run:
Clone the repository.
Compile the C file using a C compiler:
gcc password_strength_test.c -o password_strength_test
Run the executable with an input file containing passwords:
./password_strength_test input_file.txt
