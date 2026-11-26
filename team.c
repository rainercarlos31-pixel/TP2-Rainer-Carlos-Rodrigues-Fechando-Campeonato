#include <stdlib.h>
#include <string.h>
#include "team.h"

struct team {
    int id;
    char *name;
    int wins;
    int draws;
    int losses;
    int goals_scored;
    int goals_conceded;
};

Team *team_create(int id, const char *name) {
    Team *t;
    if (name == NULL) return NULL;
    t = malloc(sizeof(Team));
    if (t == NULL) return NULL;
    t->name = malloc(strlen(name) + 1);
    if (t->name == NULL) { free(t); return NULL; }
    strcpy(t->name, name);
    t->id = id;
    t->wins = t->draws = t->losses = 0;
    t->goals_scored = t->goals_conceded = 0;
    return t;
}

int team_id(Team *t) { return t == NULL ? -1 : t->id; }
const char *team_name(Team *t) { return t == NULL ? "" : t->name; }
int team_wins(Team *t) { return t == NULL ? 0 : t->wins; }
int team_draws(Team *t) { return t == NULL ? 0 : t->draws; }
int team_losses(Team *t) { return t == NULL ? 0 : t->losses; }
int team_goals_scored(Team *t) { return t == NULL ? 0 : t->goals_scored; }
int team_goals_conceded(Team *t) { return t == NULL ? 0 : t->goals_conceded; }
int team_goal_difference(Team *t) {
    return t == NULL ? 0 : t->goals_scored - t->goals_conceded;
}
int team_points(Team *t) {
    return t == NULL ? 0 : 3 * t->wins + t->draws;
}

/* Atualiza gols e resultado da equipe a partir de uma partida. */
void team_register_match(Team *t, int goals_scored, int goals_conceded) {
    if (t == NULL || goals_scored < 0 || goals_conceded < 0) return;
    t->goals_scored += goals_scored;
    t->goals_conceded += goals_conceded;
    if (goals_scored > goals_conceded) t->wins++;
    else if (goals_scored == goals_conceded) t->draws++;
    else t->losses++;
}

void team_free(Team *t) {
    if (t != NULL) { free(t->name); free(t); }
}
