// substitution.c — CS50x Week 2 (Substitution)
// Usage: ./substitution KEY
// KEY must be 26 alphabetic characters, each letter exactly once (case-insensitive).
// Prompts for plaintext and prints "ciphertext: <...>".

#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static bool valid_key(string key);
static char subst_char(char c, string key);

int main(int argc, string argv[])
{
    // Validate command-line arguments and key
    if (argc != 2 || !valid_key(argv[1]))
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Prompt for plaintext
    string plain = get_string("plaintext:  ");

    // Encrypt and print
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        printf("%c", subst_char(plain[i], argv[1]));
    }
    printf("\n");
    return 0;
}

// Check length=26, alphabetic only, and no duplicates (case-insensitive)
static bool valid_key(string key)
{
    if (key == NULL || strlen(key) != 26)
        return false;

    bool seen[26] = {false};
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha((unsigned char) key[i]))
            return false;
        int idx = toupper((unsigned char) key[i]) - 'A';
        if (seen[idx])
            return false;
        seen[idx] = true;
    }
    return true;
}

// Substitute preserving case; non-letters unchanged
static char subst_char(char c, string key)
{
    if (isupper((unsigned char) c))
    {
        int idx = c - 'A';
        return (char) toupper((unsigned char) key[idx]);
    }
    if (islower((unsigned char) c))
    {
        int idx = c - 'a';
        return (char) tolower((unsigned char) key[idx]);
    }
    return c;
}
