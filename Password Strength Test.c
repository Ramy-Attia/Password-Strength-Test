
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
// Function to check if the password contains repeated characters
bool has_repeated_chars(const char *password) {
    int len = strlen(password);  // calculates the length of the password
    for (int i = 0; i < len - 2; i++) {
        if (password[i] == password[i+1] && password[i] == password[i+2]) {  // checks if there are three repeated characters in sequence
            return true;  // returns true if the sequence of repeated characters is found
        }
    }
    return false;  // returns false if no repeated sequence is found
}

// Function to check if the password contains letters, numbers, and symbols
bool contains_required_elements(const char *password) {
    bool has_letter = false, has_digit = false, has_symbol = false;  // variables to check for the presence of letters, digits, and symbols
    for (int i = 0; i < strlen(password); i++) {
        if (isalpha(password[i])) has_letter = true;  // checks if the character is a letter
        else if (isdigit(password[i])) has_digit = true;  // checks if the character is a digit
        else has_symbol = true;  // assumes the character is a symbol if it's neither a letter nor a digit
    }
    return has_letter && has_digit && has_symbol;  // returns true only if the password has letters, digits, and symbols
}

// Function to check optional tests
int check_optional_tests(const char *password) {
    bool has_lower = false, has_upper = false;  // variables to check for lowercase and uppercase letters
    int digits = 0, symbols = 0;  // variables to count digits and symbols in the password

    for (int i = 0; i < strlen(password); i++) {
        if (islower(password[i])) has_lower = true;  // checks if the character is a lowercase letter
        if (isupper(password[i])) has_upper = true;  // checks if the character is an uppercase letter
        if (isdigit(password[i])) digits++;  // increments the digit count if the character is a digit
        if (!isalnum(password[i])) symbols++;  // increments the symbol count if the character is not alphanumeric
    }

    int passed_tests = 0;  // variable to keep track of passed optional tests
    if (has_lower) passed_tests++;  // counts if the password has at least one lowercase letter
    if (has_upper) passed_tests++;  // counts if the password has at least one uppercase letter
    if (digits >= 2) passed_tests++;  // counts if the password has two or more digits
    if (symbols >= 2) passed_tests++;  // counts if the password has two or more symbols

    return passed_tests;  // returns the number of optional tests passed
}

// Function to process each password and apply tests
void process_password(const char *password, int min_len, int max_len, bool allow_phrases, int min_phrase_len, bool require_optional, int min_optional_tests) {
    int len = strlen(password);  // calculates the length of the password
    bool is_passphrase = strchr(password, ' ') != NULL;  // checks if the password contains spaces (indicating it's a passphrase)
    bool is_strong = true;  // variable to track if the password is strong

    printf("Processing password/phrase: \"%s\"\n", password);  // prints the password/phrase being processed

    if (is_passphrase) {  // checks if the input is a passphrase
        if (!allow_phrases) {  // checks if passphrases are not allowed
            printf("Failed: Passphrases are not allowed.\n");
            is_strong = false;  // marks the passphrase as not strong
        } else if (len < min_phrase_len) {  // checks if the passphrase is shorter than the allowed minimum length
            printf("Failed: Passphrase must be at least %d characters long.\n", min_phrase_len);
            is_strong = false;  // marks the passphrase as not strong due to insufficient length
        } else {
            printf("This is a valid passphrase.\n");  // prints message indicating passphrase is valid
            is_strong = true;  // marks the passphrase as strong
        }
    } else {  // checks for regular passwords
        if (len < min_len) {  // checks if the password is shorter than the allowed minimum length
            printf("Failed: The password must be at least %d characters long.\n", min_len);
            is_strong = false;  // marks the password as not strong due to insufficient length
        }
        if (len > max_len) {  // checks if the password exceeds the allowed maximum length
            printf("Failed: The password must be fewer than %d characters long.\n", max_len);
            is_strong = false;  // marks the password as not strong due to excessive length
        }
        if (has_repeated_chars(password)) {  // checks if the password contains repeated sequences of characters
            printf("Failed: The password may not contain a sequence of three or more repeated characters.\n");
            is_strong = false;  // marks the password as not strong due to repeated characters
        }
        if (!contains_required_elements(password)) {  // checks if the password contains letters, digits, and symbols
            printf("Failed: The password must contain letters, numbers, AND symbols.\n");
            is_strong = false;  // marks the password as not strong due to missing required elements
        }

        if (require_optional) {  // checks if optional tests are required
            int passed_optional_tests = check_optional_tests(password);  // checks how many optional tests the password passes
            if (passed_optional_tests < min_optional_tests) {  // checks if the password passes fewer optional tests than required
                printf("Failed: Password did not pass enough optional tests (%d/%d passed).\n", passed_optional_tests, min_optional_tests);
                is_strong = false;  // marks the password as not strong due to failing optional tests
            }
        }
    }

    printf("Password/phrase is %sstrong.\n\n", is_strong ? "" : "not ");  // prints the result: whether the password/phrase is strong or not
}

// Function to load and process passwords from a file
void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");  // opens the password file for reading
    if (file == NULL) {  // checks if the file was opened successfully
        perror("Could not open file");  // prints an error message if the file could not be opened
        return;
    }

    // Reading configurations from the file header
    int max_len, min_len, allow_phrases, min_phrase_len = 0, require_optional, min_optional_tests = 0;
    fscanf(file, "%d %d %d", &max_len, &min_len, &allow_phrases);  // reads max length, min length, and whether passphrases are allowed

    if (allow_phrases == 1) {  // if passphrases are allowed, read the minimum phrase length
        fscanf(file, "%d", &min_phrase_len);
    }

    fscanf(file, "%d", &require_optional);  // reads if optional tests are required

    if (require_optional == 1) {  // if optional tests are required, read the minimum number of optional tests to pass
        fscanf(file, "%d", &min_optional_tests);
    }

    printf("Configurations from file %s:\n", filename);  // prints the configuration settings for the file
    printf("Max length: %d, Min length: %d, Allow phrases: %d, Min phrase length: %d\n", max_len, min_len, allow_phrases, min_phrase_len);
    printf("Require optional tests: %d, Min optional tests to pass: %d\n\n", require_optional, min_optional_tests);

    char password[101];  // buffer to store each password/phrase (assumes length will not exceed 100 characters)
    while (fgets(password, sizeof(password), file) != NULL) {  // reads each password/phrase from the file
        password[strcspn(password, "\n")] = 0;  // removes the newline character at the end of the password/phrase
        process_password(password, min_len, max_len, allow_phrases, min_phrase_len, require_optional, min_optional_tests);  // processes the password/phrase
    }

    fclose(file);  // closes the file after processing all passwords/phrases
}

int main() {
    // Processing each file individually with its own configuration
    process_file("Proposed passwords #1.txt");  // processes the first password file
    process_file("Proposed passwords #2.txt");  // processes the second password file
    process_file("Proposed passwords #3.txt");  // processes the third password file

    return 0;
}
