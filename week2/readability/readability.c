// readability.c — CS50x Week 2 (Readability)
// Computes the Coleman–Liau index for a given text and prints:
// "Before Grade 1", "Grade 16+", or "Grade X".
//
// Build:  make readability
// Run:    ./readability
// Check:  check50 cs50/problems/2025/x/readability
// Style:  style50 readability.c
// Submit: submit50 cs50/problems/2025/x/readability

#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Count letters: A–Z or a–z
int count_letters(string s)
{
    int letters = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (isalpha((unsigned char) s[i]))
        {
            letters++;
        }
    }
    return letters;
}

// Count words: number of spaces + 1 (for non-empty text)
int count_words(string s)
{
    int words = 0;
    bool in_text = false;

    for (int i = 0; s[i] != '\0'; i++)
    {
        if (!in_text && s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
        {
            in_text = true;
            words = 1; // at least one word encountered
        }
        else if (s[i] == ' ')
        {
            // Increment on transitions from non-space to space
            // (a simple heuristic sufficient for this problem)
            // We’ll avoid counting consecutive spaces as multiple words
            // by only counting spaces that are preceded by a non-space.
            if (i > 0 && s[i - 1] != ' ')
            {
                words++;
            }
        }
    }

    return words;
}

// Count sentences: '.', '!', or '?'
int count_sentences(string s)
{
    int sentences = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        char c = s[i];
        if (c == '.' || c == '!' || c == '?')
        {
            sentences++;
        }
    }
    return sentences;
}

int main(void)
{
    // Prompt for input text
    string text = get_string("Text: ");

    // Count components
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Guard against division by zero (empty input)
    if (words == 0)
    {
        printf("Before Grade 1\n");
        return 0;
    }

    // L = average letters per 100 words
    // S = average sentences per 100 words
    double L = (double) letters / words * 100.0;
    double S = (double) sentences / words * 100.0;

    // Coleman–Liau index
    double idx = 0.0588 * L - 0.296 * S - 15.8;
    int grade = (int) lround(idx);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", grade);
    }
}
