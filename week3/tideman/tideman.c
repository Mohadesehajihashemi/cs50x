#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

typedef struct
{
    string name;
    int votes;
} candidate;

typedef struct
{
    int winner;
    int loser;
} pair;

candidate candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int candidate_count;
int pair_count;

// Prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// Helpers
bool creates_cycle(int winner, int loser);
bool reaches(int from, int target);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    // Clear graph
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
        int ranks[candidate_count];

        // Get ranks for this voter
        for (int rank = 0; rank < candidate_count; rank++)
        {
            string name = get_string("Rank %i: ", rank + 1);

            if (!vote(rank, name, ranks))
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
        if (strcmp(candidates[i].name, name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int winner = ranks[i];
            int loser = ranks[j];
            preferences[winner][loser]++;
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
                pairs[pair_count++] = (pair) {.winner = i, .loser = j};
            }
            else if (ji > ij)
            {
                pairs[pair_count++] = (pair) {.winner = j, .loser = i};
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int best = i;
        for (int j = i + 1; j < pair_count; j++)
        {
            int margin_best = preferences[pairs[best].winner][pairs[best].loser] -
                              preferences[pairs[best].loser][pairs[best].winner];
            int margin_j = preferences[pairs[j].winner][pairs[j].loser] -
                           preferences[pairs[j].loser][pairs[j].winner];
            if (margin_j > margin_best)
            {
                best = j;
            }
        }
        if (best != i)
        {
            pair tmp = pairs[i];
            pairs[i] = pairs[best];
            pairs[best] = tmp;
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int w = pairs[i].winner;
        int l = pairs[i].loser;

        if (!creates_cycle(w, l))
        {
            locked[w][l] = true;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    for (int c = 0; c < candidate_count; c++)
    {
        bool has_incoming = false;
        for (int r = 0; r < candidate_count; r++)
        {
            if (locked[r][c])
            {
                has_incoming = true;
                break;
            }
        }
        if (!has_incoming)
        {
            printf("%s\n", candidates[c].name);
            return;
        }
    }
}

// DFS helpers to detect cycle
bool reaches(int from, int target)
{
    if (from == target)
    {
        return true;
    }
    for (int v = 0; v < candidate_count; v++)
    {
        if (locked[from][v] && reaches(v, target))
        {
            return true;
        }
    }
    return false;
}

bool creates_cycle(int winner, int loser)
{
    // If by locking winner->loser we can reach winner from loser, a cycle would be created
    if (reaches(loser, winner))
    {
        return true;
    }
    return false;
}
