#include <stdlib.h>
#include "matchlist.h"

typedef struct match_node { Match *match; struct match_node *next; } MatchNode;
struct matchlist { MatchNode *first; MatchNode *last; int size; };

MatchList *ml_create(void) {
    MatchList *ml = malloc(sizeof(MatchList));
    if (ml == NULL) return NULL;
    ml->first = NULL; ml->last = NULL; ml->size = 0;
    return ml;
}
/* Insere no final para preservar a ordem original do arquivo CSV. */
int ml_append(MatchList *ml, Match *m) {
    MatchNode *n;
    if (ml == NULL || m == NULL) return 0;
    n = malloc(sizeof(MatchNode));
    if (n == NULL) return 0;
    n->match = m; n->next = NULL;
    if (ml->last == NULL) ml->first = n;
    else ml->last->next = n;
    ml->last = n; ml->size++;
    return 1;
}
int ml_size(MatchList *ml) { return ml == NULL ? 0 : ml->size; }
Match *ml_get(MatchList *ml, int i) {
    MatchNode *p; int j;
    if (ml == NULL || i < 0 || i >= ml->size) return NULL;
    p = ml->first;
    for (j = 0; j < i; j++) p = p->next;
    return p->match;
}
void ml_free(MatchList *ml) {
    MatchNode *p, *next;
    if (ml == NULL) return;
    p = ml->first;
    while (p != NULL) { next = p->next; match_free(p->match); free(p); p = next; }
    free(ml);
}
