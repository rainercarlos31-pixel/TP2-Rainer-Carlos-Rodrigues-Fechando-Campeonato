#ifndef MATCH_H
#define MATCH_H

typedef struct match Match;
Match *match_create(int id, int team1, int team2, int goals1, int goals2);
int match_id(Match *m);
int match_team1(Match *m);
int match_team2(Match *m);
int match_goals1(Match *m);
int match_goals2(Match *m);
int match_goal_difference(Match *m);
void match_free(Match *m);

#endif
