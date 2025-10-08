// credit.c — CS50x Week 1 (Credit)
// Implements Luhn’s algorithm and prints the card issuer.
// Valid outputs: AMEX, MASTERCARD, VISA, or INVALID.
//
// Build:  make credit
// Run:    ./credit
// Check:  check50 cs50/problems/2025/x/credit
// Style:  style50 credit.c

#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>

// ---- Function prototypes ----------------------------------------------------
bool luhn_valid(long number);
int count_digits(long number);
int first_digit(long number);
int first_two_digits(long number);
const char *detect_issuer(long number);

// ---- Main -------------------------------------------------------------------
int main(void)
{
    // Prompt for (potentially large) credit card number
    long number = get_long("Number: ");

    // First: structural validity via Luhn’s checksum
    if (!luhn_valid(number))
    {
        printf("INVALID\n");
        return 0;
    }

    // Second: brand rules (prefix + length)
    const char *brand = detect_issuer(number);
    printf("%s\n", brand);
    return 0;
}

// ---- Luhn’s algorithm -------------------------------------------------------
// Walk from rightmost digit; double every other digit; sum digits of products.
// A number is Luhn-valid iff the total sum is a multiple of 10.
bool luhn_valid(long number)
{
    if (number <= 0)
    {
        return false;
    }

    int sum = 0;
    bool double_next = false;

    while (number > 0)
    {
        int d = number % 10; // current rightmost digit
        number /= 10;

        if (double_next)
        {
            // Double and add the digits of the product (e.g., 14 -> 1 + 4)
            int x = d * 2;
            sum += (x / 10) + (x % 10);
        }
        else
        {
            sum += d;
        }

        double_next = !double_next; // flip for the next digit
    }

    return (sum % 10) == 0;
}

// Count the number of base-10 digits in a positive long
int count_digits(long number)
{
    if (number <= 0)
    {
        return 0;
    }

    int len = 0;
    while (number > 0)
    {
        number /= 10;
        len++;
    }
    return len;
}

// Extract the most significant (leftmost) digit
int first_digit(long number)
{
    while (number >= 10)
    {
        number /= 10;
    }
    return (int) number;
}

// Extract the two most significant digits (leftmost pair)
// e.g., 4003600000000014 -> 40
int first_two_digits(long number)
{
    while (number >= 100)
    {
        number /= 10;
    }
    return (int) number;
}

// Determine issuer per CS50 spec:
//  - AMEX:       length 15, starts with 34 or 37
//  - MASTERCARD: length 16, starts with 51–55
//  - VISA:       length 13 or 16, starts with 4
// Otherwise: INVALID
const char *detect_issuer(long number)
{
    int len = count_digits(number);
    int f1 = first_digit(number);
    int f2 = first_two_digits(number);

    if (len == 15 && (f2 == 34 || f2 == 37))
    {
        return "AMEX";
    }
    if (len == 16 && (f2 >= 51 && f2 <= 55))
    {
        return "MASTERCARD";
    }
    if ((len == 13 || len == 16) && f1 == 4)
    {
        return "VISA";
    }
    return "INVALID";
}
