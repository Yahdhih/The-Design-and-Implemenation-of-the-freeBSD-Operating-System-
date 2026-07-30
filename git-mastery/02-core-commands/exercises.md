# Exercices — Module 02 : Core Commands

---

## Exercice 1 — Staging interactif avec git add -p

```sh
# Setup
mkdir /tmp/git-core && cd /tmp/git-core
git init

# Crée un fichier avec plusieurs sections
cat > config.py << 'EOF'
# Configuration de l'application

DEBUG = True         # à changer en prod
DATABASE_URL = "sqlite:///dev.db"

MAX_CONNECTIONS = 10
TIMEOUT = 30

SECRET_KEY = "dev-key-insecure"  # à remplacer
EOF

git add config.py
git commit -m "config initiale"

# Modifie plusieurs choses indépendantes dans le même fichier
cat > config.py << 'EOF'
# Configuration de l'application

DEBUG = False        # corrigé pour la prod
DATABASE_URL = "postgresql://prod:5432/app"

MAX_CONNECTIONS = 100  # augmenté
TIMEOUT = 30

SECRET_KEY = "dev-key-insecure"  # toujours pas changé
EOF

# But : faire 2 commits séparés :
# - commit 1 : "passer DEBUG à False et changer DATABASE_URL"
# - commit 2 : "augmenter MAX_CONNECTIONS"

git add -p config.py
# Pour chaque hunk : y (stage) ou n (ignore)
# Stage les hunks de DEBUG + DATABASE_URL → y
# Ignore le hunk MAX_CONNECTIONS → n

git diff --staged     # vérifie ce qui est stagé
git commit -m "passer en config production (DEBUG=False, prod DB)"

git add config.py
git commit -m "augmenter MAX_CONNECTIONS à 100"

git log --oneline    # doit afficher 2 commits séparés
```

---

## Exercice 2 — Comprendre git diff

```sh
cd /tmp/git-core

# Crée un fichier, commite, puis modifie à deux endroits
echo "ligne A" > fichier.txt
echo "ligne B" >> fichier.txt
echo "ligne C" >> fichier.txt
git add fichier.txt
git commit -m "version initiale"

# Modifie la ligne B et stage-la
sed -i 's/ligne B/ligne B (modifiée)/g' fichier.txt  # sur macOS: sed -i '' ...
git add fichier.txt

# Modifie la ligne C mais ne la stage pas
sed -i 's/ligne C/ligne C (non stagée)/g' fichier.txt

# Les 3 états :
git diff              # working tree vs index : montre la modif de C
git diff --staged     # index vs HEAD : montre la modif de B
git diff HEAD         # working tree vs HEAD : montre B et C

# Quel est l'état de l'index ?
git ls-files --stage  # SHA du blob dans l'index
```

**Réfléchis** : si tu fais `git commit` maintenant, quelle version de `fichier.txt` est commitée ?

---

## Exercice 3 — git reset dans tous ses modes

```sh
mkdir /tmp/git-reset && cd /tmp/git-reset
git init

# Crée 4 commits
for i in 1 2 3 4; do
  echo "version $i" > fichier.txt
  git add fichier.txt
  git commit -m "commit $i"
done

git log --oneline
# → d  commit 4
# → c  commit 3
# → b  commit 2
# → a  commit 1

# ---- Test --soft ----
git reset --soft HEAD~1
git status         # fichier.txt est stagé (les modifs du commit 4 sont dans l'index)
git log --oneline  # seulement 3 commits
git commit -m "commit 4 bis"  # recommite

# ---- Test --mixed (par défaut) ----
git reset HEAD~1
git status         # fichier.txt est modifié mais PAS stagé
git log --oneline  # seulement 3 commits
git add fichier.txt && git commit -m "commit 4 ter"

# ---- Test --hard ----
git reset --hard HEAD~1
git status         # clean (rien)
git log --oneline  # seulement 3 commits
# Le contenu de fichier.txt est revenu à la version du commit 3
cat fichier.txt    # → "version 3"

# Mais tout n'est pas perdu ! git reflog montre tout :
git reflog         # tu peux voir "commit 4 ter" et récupérer son SHA
```

---

## Exercice 4 — git revert sur un commit passé

```sh
mkdir /tmp/git-revert && cd /tmp/git-revert
git init

# Simule un historique avec un bug introduit
echo "def hello(): return 'hello'" > app.py
git add app.py && git commit -m "feat: fonction hello"

echo "import os; os.system('rm -rf /')" >> app.py  # bug catastrophique
git add app.py && git commit -m "feat: ajout import os"

echo "def goodbye(): return 'bye'" >> app.py
git add app.py && git commit -m "feat: fonction goodbye"

git log --oneline
# c  feat: fonction goodbye
# b  feat: ajout import os    ← LE BUG
# a  feat: fonction hello

# Récupère le SHA du commit "ajout import os" (notons-le B)
BAD_SHA=$(git log --oneline | grep "import os" | cut -d' ' -f1)

# Revert le commit du bug SANS toucher au commit goodbye
git revert $BAD_SHA --no-edit
# Git crée un commit "Revert feat: ajout import os"

git log --oneline
# d  Revert "feat: ajout import os"
# c  feat: fonction goodbye
# b  feat: ajout import os
# a  feat: fonction hello

cat app.py  # le bug est supprimé, goodbye est toujours là
```

---

## Exercice 5 — Scénario réel : corriger un commit sans en créer un nouveau

```sh
mkdir /tmp/git-amend && cd /tmp/git-amend
git init

# Tu commites avec une faute dans le message
echo "calcul de prix" > prix.py
git add prix.py
git commit -m "feat: ajout calcul de pris"  # faute !

# Correction du message :
git commit --amend -m "feat: ajout calcul de prix"
git log --oneline   # le message est corrigé

# Tu as aussi oublié d'ajouter un fichier :
echo "# tests" > test_prix.py
git add test_prix.py
git commit --amend --no-edit   # ajoute test_prix.py au même commit
git show --stat                 # montre les 2 fichiers dans le commit
```

---

## Exercice 6 — .gitignore avancé

```sh
mkdir /tmp/git-ignore && cd /tmp/git-ignore
git init

# Crée des fichiers de différents types
touch app.py app.pyc secret.env node_modules/package.json dist/bundle.js

# Crée un .gitignore
cat > .gitignore << 'EOF'
*.pyc
*.env
node_modules/
dist/
EOF

git status
# Seuls app.py et .gitignore doivent apparaître comme untracked

# Vérifie pourquoi un fichier est ignoré :
git check-ignore -v secret.env
git check-ignore -v node_modules/package.json

# Liste tous les fichiers ignorés :
git ls-files --ignored --exclude-standard --others

# Et si tu VEUX suivre un fichier normalement ignoré ?
echo "*.log" >> .gitignore
echo "données importantes" > important.log
git add -f important.log  # force l'ajout
# OU dans .gitignore : !important.log
```

---

## Récapitulatif

Après ces exercices, tu dois savoir :
- [ ] Stager des changements partiels avec `git add -p`
- [ ] Distinguer `git diff`, `git diff --staged`, `git diff HEAD`
- [ ] Choisir entre `--soft`, `--mixed`, `--hard` selon la situation
- [ ] Utiliser `git revert` pour annuler un commit déjà partagé
- [ ] Corriger un commit avec `--amend`
- [ ] Écrire un `.gitignore` qui couvre les cas courants

**Suite → [../03-branching-merging/README.md](../03-branching-merging/README.md)**
