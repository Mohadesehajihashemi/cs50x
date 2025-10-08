#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for non-negative amount of change (in cents)
    int cents;
    do
    {
        cents = get_int("Change owed: ");
    }
    while (cents < 0);

    // U.S. coin denominations in cents (greedy works for these)
    int denoms[] = {25, 10, 5, 1};

    int coins = 0;
    for (int i = 0; i < 4; i++)
    {
        coins += cents / denoms[i]; // take as many of this coin as possible
        cents %= denoms[i];         // reduce the remainder
    }

    // Print the minimum number of coins
    printf("%d\n", coins);
}
