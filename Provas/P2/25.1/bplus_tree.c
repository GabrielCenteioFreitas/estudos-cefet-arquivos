#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDER 4
#define MAX_KEYS (ORDER - 1)
#define MIN_KEYS (ORDER / 2)

typedef struct {
    int is_leaf;
    int num_keys;
    int keys[MAX_KEYS];
    long children[ORDER];
    long next;
} Node;

extern FILE *arquivo_dados;
extern void *arvore_bplus;
extern long root_offset;

long new_node(int is_leaf) {
    Node n = {0};
    n.is_leaf = is_leaf;
    n.next = 0;
    FILE *f = (FILE *)arvore_bplus;
    fseek(f, 0, SEEK_END);
    long pos = ftell(f);
    fwrite(&n, sizeof(Node), 1, f);
    return pos;
}

Node *load_node(long offset) {
    Node *n = malloc(sizeof(Node));
    FILE *f = (FILE *)arvore_bplus;
    fseek(f, offset, SEEK_SET);
    fread(n, sizeof(Node), 1, f);
    return n;
}

void save_node(Node *n, long offset) {
    FILE *f = (FILE *)arvore_bplus;
    fseek(f, offset, SEEK_SET);
    fwrite(n, sizeof(Node), 1, f);
}

static int insert_recursive(long node_off, int chave, long pos, int *promo_key, long *promo_child) {
    Node *n = load_node(node_off);
    int i, insert_pos;
    int inserted = 0;

    if (n->is_leaf) {
        // evita duplicata
        for (i = 0; i < n->num_keys; i++) {
            if (n->keys[i] == chave) { free(n); return 0; }
        }
        // inserir em ordem
        insert_pos = 0;
        while (insert_pos < n->num_keys && n->keys[insert_pos] < chave) insert_pos++;
        for (i = n->num_keys; i > insert_pos; i--) {
            n->keys[i] = n->keys[i-1];
            n->children[i] = n->children[i-1];
        }
        n->keys[insert_pos] = chave;
        n->children[insert_pos] = pos;
        n->num_keys++;
        inserted = 1;

        if (n->num_keys < ORDER) {
            save_node(n, node_off);
            free(n);
            *promo_child = 0;
        } else {
            Node *new_leaf = calloc(1, sizeof(Node));
            new_leaf->is_leaf = 1;
            int split = n->num_keys / 2;
            new_leaf->num_keys = n->num_keys - split;
            for (i = 0; i < new_leaf->num_keys; i++) {
                new_leaf->keys[i] = n->keys[split + i];
                new_leaf->children[i] = n->children[split + i];
            }
            n->num_keys = split;
            new_leaf->next = n->next;
            n->next = ftell((FILE *)arvore_bplus);
            long new_off = new_node(1);
            save_node(n, node_off);
            save_node(new_leaf, new_off);
            *promo_key = new_leaf->keys[0];
            *promo_child = new_off;
            free(new_leaf);
            free(n);
        }
    } else {
        insert_pos = 0;
        while (insert_pos < n->num_keys && chave >= n->keys[insert_pos]) insert_pos++;
        inserted = insert_recursive(n->children[insert_pos], chave, pos, promo_key, promo_child);
        if (*promo_child) {
            for (i = n->num_keys; i > insert_pos; i--) {
                n->keys[i] = n->keys[i-1];
                n->children[i+1] = n->children[i];
            }
            n->keys[insert_pos] = *promo_key;
            n->children[insert_pos+1] = *promo_child;
            n->num_keys++;
            if (n->num_keys < MAX_KEYS) {
                save_node(n, node_off);
                *promo_child = 0;
                free(n);
            } else {
                Node *new_int = calloc(1, sizeof(Node));
                new_int->is_leaf = 0;
                int split = n->num_keys / 2;
                new_int->num_keys = n->num_keys - split - 1;
                for (i = 0; i < new_int->num_keys; i++) {
                    new_int->keys[i] = n->keys[split+1+i];
                    new_int->children[i] = n->children[split+1+i];
                }
                new_int->children[new_int->num_keys] = n->children[n->num_keys+1];
                *promo_key = n->keys[split];
                n->num_keys = split;
                long new_off = new_node(0);
                save_node(n, node_off);
                save_node(new_int, new_off);
                *promo_child = new_off;
                free(new_int);
                free(n);
            }
        } else free(n);
    }
    return inserted;
}

int bplus_insert(void *arvore, int chave, long posicao) {
    arvore_bplus = arvore;
    int promo_key = 0;
    long promo_child = 0;
    int result = insert_recursive(root_offset, chave, posicao, &promo_key, &promo_child);
    if (promo_child) {
        long old_root = root_offset;
        long new_root = new_node(0);
        Node *s = load_node(new_root);
        s->is_leaf = 0;
        s->num_keys = 1;
        s->keys[0] = promo_key;
        s->children[0] = old_root;
        s->children[1] = promo_child;
        save_node(s, new_root);
        root_offset = new_root;
        free(s);
    }
    return result;
}

long find_leaf(int chave) {
    long off = root_offset;
    Node *n;
    while (1) {
        n = load_node(off);
        if (n->is_leaf) { free(n); return off; }
        int i = 0;
        while (i < n->num_keys && chave >= n->keys[i]) i++;
        off = n->children[i];
        free(n);
    }
}

int bplus_search(void *arvore, int chave, long *pos) {
    arvore_bplus = arvore;
    long leaf = find_leaf(chave);
    Node *n = load_node(leaf);
    for (int i = 0; i < n->num_keys; i++) {
        if (n->keys[i] == chave) { *pos = n->children[i]; free(n); return 1; }
    }
    free(n);
    return 0;
}

void bplus_traverse(void *arvore, void (*callback)(int, long)) {
    arvore_bplus = arvore;
    long off = root_offset;
    Node *n = load_node(off);
    while (!n->is_leaf) {
        long c = n->children[0]; free(n); n = load_node(c);
    }
    while (1) {
        for (int i = 0; i < n->num_keys; i++) callback(n->keys[i], n->children[i]);
        if (n->next == 0) break;
        long nxt = n->next; free(n); n = load_node(nxt);
    }
    free(n);
}