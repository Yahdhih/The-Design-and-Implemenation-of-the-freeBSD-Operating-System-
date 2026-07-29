# Module 01 — Comment Git stocke les données

> **Objectif** : comprendre le modèle interne de Git. Quand tu sais ce qu'il y a dans `.git/`, tu n'as plus peur de Git.

---

## 1. Git est une base de données clé-valeur

Git ne stocke pas des « diffs » ni un historique de fichiers modifiés.
Git stocke des **objets immuables** identifiés par leur hash SHA-1 (40 caractères hexadécimaux).

La clé = le SHA-1 du contenu.
La valeur = le contenu compressé (zlib).

```sh
# Crée un objet blob manuellement :
echo "bonjour" | git hash-object --stdin
# → affiche quelque chose comme : 8ab686eafeb1f44702738c8b0f24f2567c36da6d

# Avec -w, l'écrit dans .git/objects/ :
echo "bonjour" | git hash-object -w --stdin
ls .git/objects/8a/
# → b686eafeb1f44702738c8b0f24f2567c36da6d
```

Le SHA-1 est déterministe : **le même contenu donne toujours le même hash**, peu importe la machine.

---

## 2. Les 4 types d'objets Git

### Blob — contenu d'un fichier

Un blob stocke le contenu brut d'un fichier (sans son nom).

```
blob <taille>\0<contenu>
```

```sh
git cat-file -t <sha>   # → "blob"
git cat-file -p <sha>   # → contenu du fichier
```

### Tree — contenu d'un répertoire

Un tree stocke une liste de pointeurs vers des blobs (fichiers) et d'autres trees (sous-dossiers), avec les noms et permissions.

```
100644 blob <sha>   fichier.txt
040000 tree <sha>   sous-dossier/
```

```sh
git cat-file -p HEAD^{tree}   # tree du dernier commit
git ls-tree HEAD               # version lisible
git ls-tree -r HEAD            # récursif
```

### Commit — un snapshot + métadonnées

Un commit pointe vers un tree (snapshot complet du projet), un ou plusieurs parents, et contient auteur/date/message.

```
tree   <sha du tree racine>
parent <sha du commit précédent>
author  Nom <email> timestamp timezone
committer Nom <email> timestamp timezone

Message du commit
```

```sh
git cat-file -p HEAD    # affiche le commit HEAD
git log --format="%H %T %P"  # SHA commit, SHA tree, SHA parent
```

### Tag annoté — un tag avec métadonnées

Pointe vers un commit, avec auteur/date/message du tag.

```sh
git cat-file -p v1.0    # si tag annoté
```

---

## 3. Schéma : comment un commit est lié aux données

```
Commit C2 ──────────────────────────────────────────────
  │ tree ──► Tree racine
  │              ├── "src/"  ──► Tree src/
  │              │                  └── "main.c" ──► Blob (contenu de main.c)
  │              └── "README.md" ──► Blob (contenu de README.md)
  │ parent ──► Commit C1
  │ author = Alice
  │ message = "ajout main.c"
```

**Point clé** : Git ne stocke PAS les diffs entre versions.
Il stocke des **snapshots complets** à chaque commit.
Si un fichier n'a pas changé entre C1 et C2, le même blob est réutilisé (même SHA = même objet).

---

## 4. Les Refs — des pointeurs nommés vers des commits

Un ref est juste un fichier texte contenant un SHA-1.

```sh
cat .git/refs/heads/main      # → SHA-1 du dernier commit de main
cat .git/refs/heads/feature   # → SHA-1 du dernier commit de feature
cat .git/HEAD                 # → "ref: refs/heads/main" (branche courante)
```

```
.git/
├── HEAD                    ← ref symbolique → branche courante
├── refs/
│   ├── heads/
│   │   ├── main            ← branche main → SHA commit
│   │   └── feature         ← branche feature → SHA commit
│   ├── remotes/
│   │   └── origin/
│   │       └── main        ← branche distante origin/main
│   └── tags/
│       └── v1.0            ← tag (ou SHA direct)
```

**Une branche = un fichier texte de 41 octets (SHA + newline).**
C'est pour ça que créer une branche dans Git est instantané.

---

## 5. HEAD — où tu es maintenant

HEAD est une ref symbolique qui pointe soit sur une branche, soit directement sur un commit (mode « detached HEAD »).

```sh
cat .git/HEAD
# → ref: refs/heads/main       (mode normal)
# → a3f8c29...                 (mode detached)
```

En mode detached HEAD, tu n'es sur aucune branche. Les commits que tu fais ne sont rattachés à aucune branche et peuvent être perdus lors du prochain garbage collect.

---

## 6. L'index (staging area) — la zone de préparation

L'index est une structure binaire dans `.git/index` qui représente l'état **prévu** du prochain commit.

```
git add fichier.txt
```
→ Git calcule le SHA-1 du contenu, crée un blob dans `.git/objects/`, et ajoute une entrée dans l'index.

```
git commit
```
→ Git crée un tree depuis l'index, crée un commit pointant vers ce tree + le parent.

```sh
git ls-files              # fichiers dans l'index
git ls-files --stage      # SHA de chaque fichier dans l'index
```

---

## 7. Structure complète de .git/

```
.git/
├── HEAD               ← branche/commit courant
├── config             ← configuration du repo
├── index              ← staging area (binaire)
├── COMMIT_EDITMSG     ← message du dernier commit
├── objects/
│   ├── 00/ 01/ ...    ← objets (blob, tree, commit, tag)
│   │   └── <38 chars> ← les 38 derniers chars du SHA
│   ├── info/
│   └── pack/          ← objets compressés ensemble (packfiles)
├── refs/
│   ├── heads/         ← branches locales
│   ├── remotes/       ← branches distantes
│   └── tags/          ← tags
├── logs/
│   ├── HEAD           ← historique de HEAD (reflog)
│   └── refs/heads/    ← historique de chaque branche
├── hooks/             ← scripts de hooks (pre-commit, etc.)
└── info/
    └── exclude        ← gitignore local (non versionné)
```

---

## 8. Garbage collection et packfiles

Au fil du temps, `.git/objects/` accumule des fichiers. Git les compresse périodiquement en **packfiles** (`.git/objects/pack/`) qui stockent les objets sous forme de deltas.

```sh
git gc              # compresse manuellement
git count-objects -v  # statistiques des objets
```

---

## À retenir

| Concept | Réalité |
|---------|---------|
| Branche | Fichier texte = SHA d'un commit |
| Commit | Objet = tree + parent + métadonnées |
| Tree | Objet = liste (nom, mode, sha) |
| Blob | Objet = contenu brut d'un fichier |
| Staging area | `.git/index` = snapshot prévu |
| Merge | Crée un commit avec 2 parents |
| Rebase | Recrée des commits sur une nouvelle base |

---

**Exercices → [exercises.md](exercises.md)**
