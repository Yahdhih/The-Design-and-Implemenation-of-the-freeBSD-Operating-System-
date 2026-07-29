# Exercices — Module 01 : Git Internals

Lance ces exercices dans l'ordre. Chaque commande est à exécuter dans ton terminal.

---

## Exercice 1 — Explorer les objets d'un repo existant

```sh
# 1. Crée un nouveau repo de test
mkdir /tmp/git-lab && cd /tmp/git-lab
git init

# 2. Crée un premier fichier et commite
echo "bonjour git" > hello.txt
git add hello.txt
git commit -m "premier commit"

# 3. Trouve le SHA du dernier commit
git log --oneline
# Copie le SHA (ex: a1b2c3d)

# 4. Inspecte le commit
git cat-file -t a1b2c3d      # → "commit"
git cat-file -p a1b2c3d      # → tree SHA + author + message

# 5. Inspecte le tree du commit
# Récupère le SHA du tree depuis la sortie précédente, ex: e4f5a6b
git cat-file -t e4f5a6b      # → "tree"
git cat-file -p e4f5a6b      # → "100644 blob <sha> hello.txt"

# 6. Inspecte le blob
# Récupère le SHA du blob, ex: 8ab686e
git cat-file -t 8ab686e      # → "blob"
git cat-file -p 8ab686e      # → "bonjour git"
```

**Question** : le SHA du blob de `hello.txt` est-il le même que `echo "bonjour git" | git hash-object --stdin` ?

```sh
echo "bonjour git" | git hash-object --stdin
# Compare avec le SHA du blob trouvé ci-dessus
```

---

## Exercice 2 — Voir les fichiers dans .git/

```sh
# Depuis /tmp/git-lab :

# 1. Regarde ce qu'il y a dans .git/objects/
find .git/objects -type f | head -20

# 2. Regarde HEAD
cat .git/HEAD
# → "ref: refs/heads/main"

# 3. Regarde le fichier de la branche main
cat .git/refs/heads/main
# → SHA du dernier commit

# 4. Vérifie que c'est bien le même SHA que git log
git log --oneline
```

**Observation** : modifie `hello.txt`, fais un deuxième commit. Combien d'objets apparaissent dans `.git/objects/` ?

```sh
echo "deuxième ligne" >> hello.txt
git add hello.txt
git commit -m "deuxième commit"

find .git/objects -type f | wc -l   # compte les objets
# (attend : 6 objets — 2 commits, 2 trees, 2 blobs)
```

---

## Exercice 3 — Comprendre que les branches sont des fichiers

```sh
# 1. Crée une nouvelle branche
git branch feature-test

# 2. Vérifie que c'est juste un fichier
cat .git/refs/heads/feature-test
# → même SHA que main (on n'a pas encore divergé)

# 3. Bascule sur feature-test et commite
git switch feature-test
echo "feature!" > feature.txt
git add feature.txt
git commit -m "ajout feature"

# 4. Regarde les deux refs
cat .git/refs/heads/main
cat .git/refs/heads/feature-test
# → deux SHA différents maintenant

# 5. Crée une branche coûte combien en temps ?
time git branch branche-instantanee
# → quasi 0ms. C'est juste la création d'un fichier texte.
```

---

## Exercice 4 — Detached HEAD

```sh
# 1. Trouve le SHA de ton premier commit
git log --oneline
# ex: a1b2c3d (premier commit)
#     b2c3d4e (deuxième commit)

# 2. Checkout sur un commit (pas une branche)
git checkout a1b2c3d
# Warning: HEAD is now in detached HEAD state

# 3. Vérifie HEAD
cat .git/HEAD
# → directement le SHA (pas "ref: refs/heads/...")

# 4. Fais un commit en detached HEAD
echo "commit orphelin" > orphelin.txt
git add orphelin.txt
git commit -m "commit en detached HEAD"

# 5. Reviens sur main
git switch main

# 6. Le commit orphelin est toujours là via git log ?
git log --oneline --all
# Il apparaît via reflog mais pas via une branche

# 7. Récupère-le si tu veux le garder
git branch sauvetage <sha-du-commit-orphelin>
```

---

## Exercice 5 — L'index en détail

```sh
# 1. Crée un nouveau fichier sans le stager
echo "non stagé" > temp.txt

# 2. Vérifie l'index (temp.txt absent)
git ls-files   # ne montre pas temp.txt

# 3. Stage le fichier
git add temp.txt

# 4. Vérifie l'index (temp.txt présent)
git ls-files           # montre temp.txt
git ls-files --stage   # montre aussi le SHA du blob

# 5. Modifie le fichier APRÈS le git add
echo "modifié après add" > temp.txt

# 6. Regarde git status
git status
# → temp.txt : Changes to be committed (version stagée)
# → temp.txt : Changes not staged (version modifiée)

# 7. Prouve qu'il y a 2 versions :
git diff            # working tree vs index (version modifiée vs stagée)
git diff --staged   # index vs HEAD (version stagée vs dernier commit)
```

**Question** : si tu fais `git commit` maintenant, quelle version de `temp.txt` est commitée ?

---

## Exercice 6 — Hash d'un objet manuellement

Git calcule le SHA-1 comme suit :
```
SHA1("blob <taille>\0<contenu>")
```

```sh
# Vérifie que tu obtiens le même résultat que Git :
python3 -c "
import hashlib
contenu = b'bonjour git\n'
header = f'blob {len(contenu)}\0'.encode()
sha = hashlib.sha1(header + contenu).hexdigest()
print(sha)
"

# Maintenant demande à Git :
echo "bonjour git" | git hash-object --stdin
# → doit être identique
```

---

## Récapitulatif

Après ces exercices, tu dois savoir répondre :
- [ ] Qu'est-ce qu'un blob ? Un tree ? Un commit ?
- [ ] Pourquoi `git branch` est-il instantané ?
- [ ] Qu'est-ce que HEAD ? Quand est-il « detached » ?
- [ ] Quelle est la différence entre `git diff` et `git diff --staged` ?
- [ ] Que stocke réellement `.git/objects/` ?

**Suite → [../02-core-commands/README.md](../02-core-commands/README.md)**
