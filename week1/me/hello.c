// hello.c — asks for a name, then greets the user
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for the user's name
    string name = get_string("What's your name? ");

    // Print "hello, <name>"
    printf("hello, %s\n", name);
}
