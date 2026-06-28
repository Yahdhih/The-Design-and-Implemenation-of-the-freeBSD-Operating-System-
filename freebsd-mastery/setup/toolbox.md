# Toolbox — Méthodes et techniques concrètes

---

## Les 10 principes développés

### 1. Rappel actif (retrieval) avant tout

**Pourquoi ?** La relecture crée une illusion de maîtrise. La récupération forcée (essayer de se souvenir sans regarder) renforce les traces mémoire de façon incomparablement plus efficace.

**Comment faire :**
- Ouvre le fichier-jour → lis la consigne « Réveil mémoire » → ferme le livre, ferme tes notes.
- Écris ce que tu te rappelles sur une feuille ou dans `reference/notes/`.
- Ensuite seulement, compare avec la source.
- Les trous que tu découvres = les points à ancrer en priorité.

**Piège à éviter :** relire « pour te rafraîchir » avant de faire le rappel. C'est inefficace.

---

### 2. Répétition espacée (Leitner / Anki)

**Pourquoi ?** La mémoire oublie exponentiellement. Revoir au bon moment (juste avant d'oublier) est beaucoup plus efficace que revoir tous les jours.

**Intervalles :** carte créée au Jour J → revue à J+1, J+3, J+7, J+16, J+35.

**Comment faire :**
- Importe `flashcards/cards.tsv` dans Anki. Anki gère les intervalles automatiquement.
- Fais les cartes **dues** chaque matin, AVANT d'ouvrir le fichier-jour.
- Quand tu crées une carte, mets un exemple concret (pas une définition abstraite).
- Format des cartes : `Question<TAB>Réponse<TAB>tags`.

**Règle d'or des cartes :** une carte = un concept atomique. Si la réponse fait 5 lignes, découpe.

---

### 3. Feynman / enseigner

**Pourquoi ?** Expliquer à voix haute révèle immédiatement les trous dans ta compréhension.

**Comment faire :**
- Pour chaque concept majeur, explique-le à voix haute comme à un ami qui connaît le C mais pas les OS.
- Note chaque point où tu butes ou où tu dis « c'est un peu comme… » → c'est une zone à approfondir.
- Variante écrite : explique dans tes propres mots dans `reference/notes/jour-NNN.md`.

**Indicateur de maîtrise :** tu peux expliquer sans regarder tes notes, avec des exemples concrets.

---

### 4. Entrelacement lecture ↔ code

**Pourquoi ?** Lire et coder activent des processus cognitifs différents. Alterner renforce les deux et évite la fatigue de lecture.

**Comment faire :**
- Ne jamais enchaîner plus de 2-3 jours de lecture pure.
- Après 2 jours de lecture → 1 jour de code ou de trace de sources.
- Le code force à préciser ce qui est resté flou après la lecture.

**Dans ce plan :** le calendrier respecte déjà cette règle. Si tu modifies l'ordre, garde l'entrelacement.

---

### 5. Génération (predict-then-verify)

**Pourquoi ?** Prédire active tes connaissances existantes et crée un « espace » que la lecture vient remplir. Les corrections sont ancrées plus profondément que les informations nouvelles.

**Comment faire :**
- Avant chaque section de lecture, écris 2 lignes : « je pense que cette section va expliquer… »
- Lis la section.
- Reviens sur ta prédiction : qu'est-ce qui était juste ? qu'est-ce qui était faux ou incomplet ?
- Note la correction explicitement.

**Variante pour le code :** avant de lire `kern_fork.c`, écris le pseudo-code que tu attendes.

---

### 6. Construire pour comprendre

**Règle d'or :** *si je ne peux pas le réimplémenter ou le redessiner de mémoire, je ne l'ai pas compris.*

**Comment faire :**
- Pour chaque sous-système : implémente une version jouet en C.
- Les jalons sont progressifs : d'abord « ça compile et fait quelque chose », puis « ça gère les cas d'erreur », puis « ça ressemble au vrai ».
- Ne vise pas la perfection : une implémentation jouet partielle qui force à réfléchir vaut mieux qu'un code copié-collé qui fonctionne.
- Exemples dans ce plan : mini-shell, allocateur bump→free list→slab, chaîne de mbuf, transport stop-and-wait→fenêtre glissante, mini-FS dans une image.

---

### 7. Lire le vrai code noyau — stratégie descendante

**Pourquoi ?** Le code source est la vérité ultime. Le livre explique le *quoi* et le *pourquoi* ; le code montre le *comment*.

**Stratégie de navigation descendante :**

1. **Trouve le point d'entrée** — un appel système (`syscall`), une fonction d'interface (`socket()`, `fork()`).
2. **Lis d'abord les définitions de struct** — avant de lire le code, lis les `.h` : `struct socket`, `struct proc`, `struct mbuf`. Dessine les champs clés.
3. **Descends la chaîne d'appels** — `socket()` → `kern_socket()` → `socreate()` → … Note chaque appel sur une feuille.
4. **Pose 2 questions ciblées** avant de lire (ex. : « où sont initialisés les buffers send/recv ? »). Lis pour répondre à ces questions, pas pour tout comprendre.
5. **Annote le flux de contrôle** — dans ton éditeur ou sur papier, note quelle branche fait quoi.
6. **Utilise grep/ctags/cscope** pour naviguer rapidement :
   ```sh
   grep -rn "socreate" ~/freebsd-src/sys/kern/
   # Dans vim avec ctags : Ctrl+] sur un nom de fonction
   ```

**Règle des 2 questions :** ne lis jamais une source sans avoir 2 questions en tête. Sinon, tu lis sans t'engager.

---

### 8. Double codage (dessiner)

**Pourquoi ?** Les cerveaux retiennent mieux les informations présentées en texte ET en image simultanément.

**Comment faire :**
- Pour chaque structure de données importante → dessine ses champs et ses pointeurs.
- Pour chaque mécanisme → dessine le flux de contrôle ou la machine d'état.
- Exemples prioritaires :
  - Chaîne de mbufs (Phase 2 essentiel)
  - Buffers send/recv d'un socket (`sb_mb`, `sb_cc`, `sb_hiwat`)
  - Machine d'état TCP (CLOSED → SYN_SENT → ESTABLISHED → …)
  - `vm_map` → `vm_map_entry` → `vm_object` → `vm_page`
  - Inodes + bitmap + superbloc (FFS)
- Méthode : dessine sur papier, prends une photo, mets-la dans `reference/diagrams/`.
- ASCII art dans `reference/diagrams/*.txt` fonctionne aussi.

---

### 9. Consolidation quotidienne (les 15 dernières minutes)

**Ce que tu fais chaque fin de session :**
1. **Résumé 3-5 puces** dans tes propres mots → `reference/notes/jour-NNN.md`.
2. **2-3 nouvelles cartes** → `flashcards/cards.tsv` (exemple concret par carte).
3. **1 schéma** → `reference/diagrams/` (même ébauche rapide).
4. **1 question ouverte** → note ce que tu n'as pas compris ou ce qui te surprend.

**Piège :** sauter la consolidation parce que tu es fatigué. C'est là que l'apprentissage se fixe.

---

### 10. Revue hebdomadaire (1 jour sur 7)

**Règle : pas de matière neuve.** Seulement consolidation et récupération.

**Déroulé (voir `reviews/weekly-review-template.md`) :**
1. Redessine de mémoire les 2-3 schémas clés de la semaine (sans regarder les notes).
2. Réexplique 3 concepts à voix haute (Feynman).
3. Balaie toutes les cartes créées dans la semaine (Anki ou manuel).
4. Mini-quiz de 10 questions auto-générées depuis tes notes.
5. Bilan : qu'est-ce qui résiste encore ? Crée des cartes supplémentaires pour ces points.

---

## Stratégie de débogage comme apprentissage

Le débogage d'un programme qui interagit avec le noyau est une session d'apprentissage à lui seul.

```sh
# Sous FreeBSD — observer les syscalls en temps réel :
truss ./mon_programme

# Trace vers fichier, décode ensuite :
ktrace ./mon_programme
kdump | less

# GDB / LLDB — poser un breakpoint sur un syscall wrapper :
lldb ./echo_server
(lldb) break set -n recv
(lldb) run
# Inspecte les arguments : socket fd, buffer, len, flags

# Sous Linux — équivalent :
strace ./mon_programme
```

**Exercice type :** lance `truss ./echo_server` pendant un test avec `nc`. Identifie dans la trace :
- `socket()`, `bind()`, `listen()`, `accept()`, `recv()`, `send()`, `close()`.
- Retrouve ces appels dans `sys/kern/uipc_socket.c`.

---

## Conventions de dessin ASCII

```
Chaîne de mbufs :
  +--------+    +--------+    +--------+
  | mbuf 1 |--->| mbuf 2 |--->| mbuf 3 |---> NULL
  | data   |    | data   |    | data   |
  | m_next |    | m_next |    | m_next |
  +--------+    +--------+    +--------+

Structure de données (ex. struct socket) :
  struct socket
  ┌──────────────────┐
  │ so_type          │  SOCK_STREAM / SOCK_DGRAM
  │ so_state         │  SS_ISCONNECTED, SS_ISDISCONNECTING…
  │ so_rcv (sockbuf) │──► buffer de réception
  │ so_snd (sockbuf) │──► buffer d'envoi
  │ so_proto         │──► struct protosw (TCP/UDP/…)
  └──────────────────┘

Machine d'état :
  [CLOSED] --SYN_SENT--> [SYN_SENT] --SYN_ACK--> [ESTABLISHED]
```

---

## Intervalles de répétition espacée (référence rapide)

| Leitner boîte | Intervalle | Usage |
|---------------|-----------|-------|
| Boîte 1 | J+1 | Carte nouvelle ou ratée |
| Boîte 2 | J+3 | Carte réussie une fois |
| Boîte 3 | J+7 | Carte réussie deux fois |
| Boîte 4 | J+16 | Carte réussie trois fois |
| Boîte 5 | J+35 | Carte quasi-maîtrisée |

*Anki gère tout ça automatiquement. Le tableau ci-dessus sert si tu travailles manuellement.*
