#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for height in [1, 8]
    int h;
    do
    {
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);

    // Print double half-pyramids
    for (int i = 1; i <= h; i++)
    {
        // Left padding
        for (int s = 0; s < h - i; s++)
        {
            printf(" ");
        }

        // Left hashes
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        // Gap
        printf("  ");

        // Right hashes
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        printf("\n");
    }
}
