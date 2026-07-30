# Module 02 — Commandes du quotidien en profondeur

> **Objectif** : maîtriser les commandes les plus utilisées avec une compréhension de ce qu'elles font vraiment.

---

## 1. git add — alimenter l'index

```sh
git add fichier.txt          # stage un fichier entier
git add src/                 # stage tout un dossier
git add .                    # stage tout depuis le dossier courant
git add -p                   # staging interactif (chunk par chunk)
git add -u                   # stage seulement les fichiers déjà trackés (modifiés/supprimés)
```

### git add -p : l'outil le plus sous-utilisé

`-p` te montre chaque « hunk » (bloc de changements) et te demande quoi en faire :

```
y = stage ce hunk
n = ignorer ce hunk
s = découper en plus petits hunks
e = éditer le hunk manuellement
q = quitter
```

**Usage** : tu as modifié 3 choses dans le même fichier mais tu veux faire 2 commits séparés → `git add -p`.

---

## 2. git status — comprendre l'état

```sh
git status          # complet
git status -s       # court (2 colonnes : index | working tree)
```

Les 2 colonnes de `git status -s` :
```
M  fichier.txt    ← modifié dans l'index (stagé), pas dans le working tree
 M fichier.txt    ← modifié dans le working tree, pas stagé
MM fichier.txt    ← modifié dans les deux
A  nouveau.txt    ← nouveau fichier stagé
?? inconnu.txt    ← fichier non tracké
```

---

## 3. git diff — comparer des états

```sh
git diff                    # working tree vs index
git diff --staged           # index vs HEAD (= ce qui sera commité)
git diff HEAD               # working tree + index vs HEAD
git diff HEAD~2             # vs 2 commits avant
git diff main..feature      # diff entre 2 branches
git diff main...feature     # diff depuis l'ancêtre commun
git diff --stat             # résumé (noms de fichiers + lignes ±)
git diff --word-diff        # diff mot par mot (utile pour la prose)
```

### La différence `..` vs `...`

```
A---B---C  (main)
     \
      D---E  (feature)

main..feature   = E - C  (diff entre les deux pointes)
main...feature  = E - B  (diff depuis l'ancêtre commun B)
```

---

## 4. git commit — créer un snapshot

```sh
git commit -m "message"             # commit direct
git commit                          # ouvre l'éditeur (pour message long)
git commit -am "message"            # stage les fichiers trackés + commit
git commit --amend                  # modifie le dernier commit
git commit --amend --no-edit        # ajoute du contenu sans changer le message
git commit --allow-empty -m "msg"   # commit vide (utile pour déclencher CI)
```

### Bonnes pratiques de message de commit

Format conventionnel (Conventional Commits) :
```
<type>(<scope>): <description courte>

<corps optionnel — explique le POURQUOI>

<footer optionnel — références issues, BREAKING CHANGE>
```

Types courants : `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

```
feat(auth): ajouter login OAuth2

Remplace le login par email/mot de passe.
Voir #142 pour la discussion.

BREAKING CHANGE: l'endpoint /api/login change de format
```

---

## 5. git log — naviguer dans l'historique

```sh
git log                              # log complet
git log --oneline                    # une ligne par commit
git log --oneline --graph --all      # graphe ASCII de toutes les branches
git log --oneline -10                # 10 derniers commits
git log --stat                       # fichiers modifiés par commit
git log -p                           # diff complet par commit
git log --author="Alice"             # commits d'Alice
git log --since="2024-01-01"         # depuis une date
git log --until="yesterday"
git log --grep="bug"                 # commits dont le message contient "bug"
git log -S "fonctionX"               # commits qui ont ajouté/supprimé "fonctionX"
git log main..feature                # commits dans feature mais pas dans main
git log --follow src/utils.js        # historique d'un fichier (avec renommages)
```

### Formats personnalisés

```sh
git log --format="%h %an %s"         # SHA court, auteur, sujet
git log --format="%H %T %P"          # SHA commit, tree, parent(s)
git log --pretty=format:"%C(yellow)%h%Creset %s" --graph --all
```

---

## 6. git show — inspecter un commit

```sh
git show                  # dernier commit + diff
git show HEAD~2           # commit d'avant-hier
git show <sha>            # commit précis
git show HEAD:fichier.txt # contenu d'un fichier tel qu'il était dans HEAD
git show v1.0:src/main.c  # fichier dans le tag v1.0
```

---

## 7. git restore — annuler des modifications

Remplace l'ancien `git checkout -- <fichier>`.

```sh
git restore fichier.txt             # annule les modifs working tree (depuis l'index)
git restore --staged fichier.txt    # unstage un fichier (retire de l'index)
git restore --source HEAD~2 fichier.txt  # restaure depuis 2 commits avant
git restore .                       # annule toutes les modifs working tree
```

---

## 8. git reset — déplacer HEAD et/ou l'index

C'est la commande la plus mal comprise. Elle déplace HEAD (et optionnellement met à jour l'index et le working tree).

```sh
# --soft : déplace HEAD seulement
git reset --soft HEAD~1
# → le commit est annulé, mais ses changements restent dans l'index (stagés)

# --mixed (par défaut) : déplace HEAD + reset l'index
git reset HEAD~1
# → commit annulé, changements dans le working tree (mais non stagés)

# --hard : déplace HEAD + reset index + reset working tree
git reset --hard HEAD~1
# → commit annulé, changements PERDUS (attention !)
```

### Schéma

```
                        HEAD  Index  WorkTree
git reset --soft HEAD~1  ←     =      =
git reset HEAD~1          ←     ←      =
git reset --hard HEAD~1   ←     ←      ←
```

### Usage courant de reset

```sh
# Défaire les 3 derniers commits en les gardant comme modifs :
git reset HEAD~3

# Annuler tous les stages (= tout unstager) :
git reset

# Défaire le dernier commit et le restager (pour le re-découper) :
git reset --soft HEAD~1
```

---

## 9. git revert — annuler sans réécrire l'historique

```sh
git revert HEAD         # crée un commit qui annule HEAD
git revert <sha>        # annule un commit précis
git revert HEAD~3..HEAD # annule les 3 derniers commits (3 commits de revert)
git revert -n HEAD      # prépare le revert sans commiter (--no-commit)
```

**Quand utiliser revert vs reset ?**
- `reset` : pour des commits **locaux** non poussés (réécrit l'historique).
- `revert` : pour des commits **déjà poussés** sur un remote partagé (ne réécrit pas l'historique).

---

## 10. .gitignore — ce qu'il faut savoir

```gitignore
# Ignorer un fichier précis
.env
secrets.json

# Ignorer tous les fichiers .log
*.log

# Ignorer un dossier
node_modules/
dist/
__pycache__/

# Ne PAS ignorer un fichier qui serait normalement ignoré
!important.log

# Ignorer dans tous les sous-dossiers
**/tmp/

# Ignorer seulement à la racine du repo
/build/
```

```sh
git check-ignore -v fichier.txt  # pourquoi ce fichier est ignoré
git ls-files --ignored --exclude-standard  # liste tous les fichiers ignorés
```

Gitignore global (pour tous tes repos) :
```sh
git config --global core.excludesFile ~/.gitignore_global
echo ".DS_Store" >> ~/.gitignore_global
echo "Thumbs.db" >> ~/.gitignore_global
```

---

## 11. git rm et git mv

```sh
git rm fichier.txt                  # supprime du working tree ET de l'index
git rm --cached fichier.txt         # retire de l'index seulement (garde le fichier)
git mv ancien.txt nouveau.txt       # renomme (équivalent mv + git rm + git add)
```

---

**Exercices → [exercises.md](exercises.md)**
