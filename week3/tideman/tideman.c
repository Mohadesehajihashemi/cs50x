cat > ~/cs50x/week3/tideman/tideman.c <<'EOF'
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Max number of candidates
#define MAX_CANDIDATES 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX_CANDIDATES][MAX_CANDIDATES];

// locked[i][j] means i is locked in over j (edge i -> j)
bool locked[MAX_CANDIDATES][MAX_CANDIDATES];

// Each pair has a winner and a loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX_CANDIDATES];

// Array of pairs
pair pairs[MAX_CANDIDATES * (MAX_CANDIDATES - 1) / 2];

int pair_count;
int candidate_count;

// Prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// Helpers
int victory_strength(pair p);
bool reaches(int from, int target);
bool would_create_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;

    int voter_count = get_int("Number of voters: ");
    for (int i = 0; i < voter_count; i++)
    {
        int ranks[MAX_CANDIDATES];

        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update global preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int higher = ranks[i];
            int lower  = ranks[j];
            preferences[higher][lower]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int ij = preferences[i][j];
            int ji = preferences[j][i];
            if (ij > ji)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser  = j;
                pair_count++;
            }
            else if (ji > ij)
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser  = i;
                pair_count++;
            }
        }
    }
}

int victory_strength(pair p)
{
    return preferences[p.winner][p.loser] - preferences[p.loser][p.winner];
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    bool swapped = true;
    while (swapped)
    {
        swapped = false;
        for (int i = 0; i < pair_count - 1; i++)
        {
            if (victory_strength(pairs[i]) < victory_strength(pairs[i + 1]))
            {
                pair tmp = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}

// DFS from 'from' to see if we can reach 'target'
bool reaches(int from, int target)
{
    if (from == target)
    {
        return true;
    }
    for (int v = 0; v < candidate_count; v++)
    {
        if (locked[from][v])
        {
            if (reaches(v, target))
            {
                return true;
            }
        }
    }
    return false;
}

bool would_create_cycle(int winner, int loser)
{
    return reaches(loser, winner);
}

// Lock pairs without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int w = pairs[i].winner;
        int l = pairs[i].loser;
        if (!would_create_cycle(w, l))
        {
            locked[w][l] = true;
        }
    }
}

// Print source of the graph
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool has_incoming = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                has_incoming = true;
                break;
            }
        }
        if (!has_incoming)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
EOF
