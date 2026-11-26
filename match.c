#include <stdlib.h>
#include "match.h"

struct match { int id; int team1; int team2; int goals1; int goals2; };

Match *match_create(int id, int team1, int team2, int goals1, int goals2) {
    Match *m;
    if (goals1 < 0 || goals2 < 0 || team1 == team2) return NULL;
    m = malloc(sizeof(Match));
    if (m == NULL) return NULL;
    m->id = id; m->team1 = team1; m->team2 = team2;
    m->goals1 = goals1; m->goals2 = goals2;
    return m;
}
int match_id(Match *m) { return m == NULL ? -1 : m->id; }
int match_team1(Match *m) { return m == NULL ? -1 : m->team1; }
int match_team2(Match *m) { return m == NULL ? -1 : m->team2; }
int match_goals1(Match *m) { return m == NULL ? 0 : m->goals1; }
int match_goals2(Match *m) { return m == NULL ? 0 : m->goals2; }
int match_goal_difference(Match *m) {
    if (m == NULL) return 0;
    return abs(m->goals1 - m->goals2);
}
void match_free(Match *m) { free(m); }
