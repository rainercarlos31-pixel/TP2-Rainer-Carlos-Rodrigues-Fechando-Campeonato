#include <stdlib.h>
#include "teamlist.h"

typedef struct node {
    Team *team;
    struct node *next;
    struct node *prev;
} Node;

struct teamlist { Node *first; Node *last; int size; };

TeamList *tl_create(void) {
    TeamList *tl = malloc(sizeof(TeamList));
    if (tl == NULL) return NULL;
    tl->first = NULL; tl->last = NULL; tl->size = 0;
    return tl;
}

int tl_append(TeamList *tl, Team *t) {
    Node *n;
    if (tl == NULL || t == NULL) return 0;
    n = malloc(sizeof(Node));
    if (n == NULL) return 0;
    n->team = t; n->next = NULL; n->prev = tl->last;
    if (tl->last == NULL) tl->first = n;
    else tl->last->next = n;
    tl->last = n; tl->size++;
    return 1;
}

int tl_size(TeamList *tl) { return tl == NULL ? 0 : tl->size; }

Team *tl_get(TeamList *tl, int i) {
    Node *p; int j;
    if (tl == NULL || i < 0 || i >= tl->size) return NULL;
    if (i < tl->size / 2) {
        p = tl->first;
        for (j = 0; j < i; j++) p = p->next;
    } else {
        p = tl->last;
        for (j = tl->size - 1; j > i; j--) p = p->prev;
    }
    return p->team;
}

Team *tl_find_id(TeamList *tl, int id) {
    Node *p;
    if (tl == NULL) return NULL;
    for (p = tl->first; p != NULL; p = p->next)
        if (team_id(p->team) == id) return p->team;
    return NULL;
}

static int comes_before(Team *a, Team *b) {
    if (team_points(a) != team_points(b)) return team_points(a) > team_points(b);
    if (team_wins(a) != team_wins(b)) return team_wins(a) > team_wins(b);
    if (team_goal_difference(a) != team_goal_difference(b))
        return team_goal_difference(a) > team_goal_difference(b);
    if (team_goals_scored(a) != team_goals_scored(b))
        return team_goals_scored(a) > team_goals_scored(b);
    return team_id(a) < team_id(b);
}

/* Ordena por insercao, movendo os proprios nos da lista.
 * Assim, a colecao continua sendo duplamente encadeada, sem vetor auxiliar.
 */
void tl_sort(TeamList *tl) {
    Node *current, *next, *scan, *insert_before;
    if (tl == NULL || tl->size < 2) return;
    current = tl->first->next;
    while (current != NULL) {
        next = current->next;
        scan = tl->first;
        while (scan != current && !comes_before(current->team, scan->team))
            scan = scan->next;
        if (scan != current) {
            if (current->prev != NULL) current->prev->next = current->next;
            if (current->next != NULL) current->next->prev = current->prev;
            else tl->last = current->prev;
            insert_before = scan;
            current->prev = insert_before->prev;
            current->next = insert_before;
            if (insert_before->prev != NULL) insert_before->prev->next = current;
            else tl->first = current;
            insert_before->prev = current;
        }
        current = next;
    }
}

void tl_free(TeamList *tl) {
    Node *p, *next;
    if (tl == NULL) return;
    p = tl->first;
    while (p != NULL) {
        next = p->next;
        team_free(p->team);
        free(p);
        p = next;
    }
    free(tl);
}
