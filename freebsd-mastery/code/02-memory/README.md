# code/02-memory/ — Gestion mémoire virtuelle, mmap, allocateurs

Correspond à **Ch. 6 — Gestion de la mémoire virtuelle**.

---

## Projets

### Jalon 1 — Expériences mmap (`mmap_exp.c`)

**Objectif** : observer la pagination à la demande, mesurer les fautes de page.

```c
// Expériences à implémenter :
// 1. mmap(NULL, large_size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0)
//    → mesurer le temps de la première écriture (faute de page) vs accès suivants
// 2. mmap d'un fichier réel → lire, modifier, observer avec madvise(MADV_DONTNEED)
// 3. MAP_SHARED entre père et fils : observer la communication via mémoire partagée
```

```sh
make mmap_exp
./mmap_exp
# Attendu : temps de premier accès > temps d'accès suivants (demand paging visible)
```

Sur FreeBSD, observer les fautes de page avec :
```sh
vmstat -s | grep "pages paged in"
# ou pendant l'exécution :
vmstat 1
```

**Questions à répondre** :
1. Lors du premier `mmap()`, le noyau alloue-t-il des pages physiques immédiatement ?
2. Quelle est la différence entre `MAP_PRIVATE` et `MAP_SHARED` vis-à-vis de `fork()` ?

---

### Jalon 2 — Allocateur bump (`alloc_toy.c` — niveau 1)

**Objectif** : comprendre l'allocation mémoire la plus simple possible.

```c
// Interface à implémenter :
void *bump_alloc(size_t n);   // avance un pointeur, retourne l'adresse précédente
void  bump_reset(void);       // remet le pointeur à zéro (libère tout d'un coup)
// Backing : un grand bloc mmap() alloué au démarrage (ex. 1 Mo)
```

**Critère** : alloue 1000 blocs de taille variable, vérifie qu'aucun ne se chevauche.

---

### Jalon 3 — Allocateur free list (`alloc_toy.c` — niveau 2)

**Objectif** : permettre la libération individuelle des blocs.

```c
// Ajouter :
void  fl_free(void *ptr);    // remet le bloc dans la free list
// Chaque bloc a un en-tête : { size_t size; struct block *next; }
// fl_alloc() cherche d'abord dans la free list (first-fit), sinon bump
```

**Critère** : alloue 100 blocs, libère les pairs, réalloue → doit réutiliser les blocs libérés.

---

### Jalon 4 — Slab simplifié (`alloc_toy.c` — niveau 3)

**Objectif** : comprendre pourquoi UMA/slab est plus efficace pour les objets de taille fixe.

```c
// Interface à implémenter :
typedef struct slab_zone slab_zone_t;
slab_zone_t *slab_zone_create(size_t obj_size, int n_per_slab);
void        *slab_alloc(slab_zone_t *z);
void         slab_free(slab_zone_t *z, void *obj);
// Chaque zone gère des slabs de N objets de taille fixe.
// Free list par zone, pas globale.
```

**Critère** : alloue/libère 10000 objets de 64 octets en < 1 ms (mesure avec `clock_gettime`).

---

## Sources FreeBSD clés

| Fichier | Ce qu'on cherche |
|---------|-----------------|
| `sys/vm/vm_map.c` | `vm_map_insert()`, `vm_map_find()` : gestion de l'espace d'adressage |
| `sys/vm/vm_fault.c` | `vm_fault()` : traitement des fautes de page, COW |
| `sys/vm/vm_object.c` | `vm_object_allocate()` : création d'objets VM |
| `sys/vm/uma_core.c` | `uma_zalloc()`, `uma_zfree()` : allocateur slab |
| `sys/vm/uma.h` | Interface publique d'UMA |
| `sys/vm/vm_pageout.c` | `vm_pageout()` : démon de pagination |

**Questions de trace prioritaires** :
1. Dans `uma_core.c`, comment fonctionne le cache per-CPU (pcpu) d'UMA ? À quel moment consulte-t-il la zone globale ?
2. Dans `vm_fault.c`, quelle est la différence de traitement entre une faute sur une page anonyme et une faute sur une page file-backed ?

---

## Schémas à dessiner

1. `vm_map` → liste de `vm_map_entry` (avec adresses de début/fin, protection, backing object).
2. Hiérarchie UMA : zone → slab → objets, avec le pcpu cache intercalé.
3. Comparatif performances : bump vs free list vs slab, sur 10000 allocs de 64 B.
