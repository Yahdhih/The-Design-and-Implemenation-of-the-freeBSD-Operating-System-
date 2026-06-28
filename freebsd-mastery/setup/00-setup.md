# Setup — Environnement de travail FreeBSD Mastery

Suis ces étapes **une seule fois** avant le Jour 1. La dernière étape (calibration de la TOC) te prend 10 min avec le livre en main.

---

## Étape 1 — Compilateur et outils de base

### Sur FreeBSD (VM recommandée)
```sh
# Les outils suivants sont déjà présents sur un système de base FreeBSD :
cc --version          # clang (FreeBSD default)
make --version
gdb --version || lldb --version
git --version

# Outils de trace noyau (spécifiques FreeBSD) :
which truss           # trace les appels système en temps réel
which ktrace          # trace vers fichier binaire, décodé par kdump

# Outils réseau :
pkg install -y wireshark tshark tcpdump
```

### Sur macOS (hôte de développement secondaire)
```sh
# Xcode Command Line Tools (cc = clang, make, lldb inclus) :
xcode-select --install

# Homebrew pour les outils manquants :
brew install gdb tshark wireshark

# Note : truss et ktrace n'existent pas sur macOS.
# Équivalents : dtruss (limité) ou dtrace.
# Pour tout ce qui est FreeBSD-spécifique, utilise la VM.
```

### Sur Linux (fallback)
```sh
sudo apt install build-essential gdb git strace ltrace tcpdump tshark wireshark
# strace = équivalent de truss ; ltrace = trace les appels libc
```

---

## Étape 2 — VM FreeBSD (fortement recommandé)

> **Pourquoi une VM ?** `kqueue`, `truss`, `ktrace`, Capsicum, jails, MAC, et les appels système correspondent exactement aux sources du livre. Sur Linux ou macOS, certains projets du cours ne sont pas faisables.

### Option A — QEMU (Linux/macOS, gratuit)
```sh
# Télécharge l'image FreeBSD 14.x ou 15.x :
# https://download.freebsd.org/releases/amd64/amd64/ISO-IMAGES/
# (prends la .qcow2 ou .img pour QEMU)

qemu-system-x86_64 \
  -m 2048 \
  -cpu host \
  -hda FreeBSD-14.x-amd64.qcow2 \
  -net nic -net user,hostfwd=tcp::2222-:22 \
  -nographic
# Puis : ssh -p 2222 user@localhost
```

### Option B — VirtualBox (macOS/Windows, plus simple)
```sh
# Télécharge FreeBSD .vdi depuis https://www.freebsd.org/where/
# Crée une VM (FreeBSD 64-bit, 2 Go RAM, 20 Go disque)
# Active le dossier partagé pour partager ton code avec la VM.
```

### Option C — bhyve (sur FreeBSD hôte)
```sh
# Voir le FreeBSD Handbook, chapitre bhyve.
```

---

## Étape 3 — Récupération des sources FreeBSD

> Le livre couvre l'ère FreeBSD 9/10. Les sources récentes ont évolué, mais la structure est reconnaissable. Lis le code récent en **gardant l'écart en tête** : c'est aussi un exercice critique.

```sh
# Clone complet (volumineux ~1.5 Go) :
git clone --depth 1 https://github.com/freebsd/freebsd-src.git ~/freebsd-src

# Alternative : navigation en ligne (plus léger) :
# https://github.com/freebsd/freebsd-src
# https://cgit.freebsd.org/src/

# Sur la VM FreeBSD, les sources sont aussi disponibles via :
fetch https://download.freebsd.org/releases/amd64/14.x-RELEASE/src.txz
tar -C / -xvf src.txz
# Les sources sont alors dans /usr/src/
```

---

## Étape 4 — Navigation dans les sources (outils)

```sh
# ctags (génère un index de symboles pour vim/emacs) :
pkg install -y universal-ctags    # FreeBSD
brew install universal-ctags      # macOS

cd ~/freebsd-src/sys
ctags -R .
# Puis dans vim : Ctrl+] pour aller à la définition, Ctrl+O pour revenir.

# cscope (navigation croisée : qui appelle quoi) :
pkg install -y cscope
cd ~/freebsd-src/sys
find . -name "*.c" -o -name "*.h" > cscope.files
cscope -b -q -k
# cscope -d pour lancer l'interface interactive.

# grep rapide dans les sources :
grep -rn "struct socket" ~/freebsd-src/sys/sys/
grep -rn "uipc_socket" ~/freebsd-src/sys/kern/

# VSCode / CLion / Neovim avec LSP (clangd) :
# Génère compile_commands.json avec bear ou cmake, puis configure clangd.
```

---

## Étape 5 — Anki (répétition espacée)

```sh
# Installe Anki : https://apps.ankiweb.net/
# (Desktop recommandé pour l'import .tsv)

# Import du paquet de démarrage :
# Dans Anki : Fichier > Importer > sélectionne flashcards/cards.tsv
# Séparateur : Tabulation
# Coche "Autoriser le HTML dans les champs"
# Deck : "FreeBSD-Mastery"
```

---

## Étape 6 — Mise en place du dépôt et du Makefile

```sh
cd freebsd-mastery/code/common
make          # doit compiler sans erreur (aucun .c à compiler pour l'instant)
make clean    # doit nettoyer sans erreur

# Test du workflow complet :
cat > /tmp/test_hello.c << 'EOF'
#include <stdio.h>
int main(void) { printf("hello\n"); return 0; }
EOF
cc -Wall -Wextra -g -o /tmp/test_hello /tmp/test_hello.c && /tmp/test_hello
# Attendu : "hello"
```

---

## Étape 7 — CALIBRATION DE LA TABLE DES MATIÈRES (obligatoire)

> **Ouvre ton exemplaire du livre** et remplis `reference/toc.md` avec les vraies pages.
> Cette étape prend ~10 min et calibre tout le reste du plan.

La table `reference/toc.md` est un squelette prêt à remplir. Pour chaque chapitre :
1. Note la page de début et de fin (couverture non incluse).
2. Calcule le nombre de pages.
3. Ajuste mentalement le rythme : **5-10 pages actives / session de 40 min**.

Exemples de calcul :
- Chapitre de 30 pages → 3-6 jours de lecture.
- Chapitre de 80 pages → 8-16 jours de lecture.
- Les projets de code s'intercalent entre les blocs de lecture (voir `PLAN.md`).

---

## Étape 8 — Vérification finale

Coche chaque case :

- [ ] `cc -Wall -Wextra -g` disponible et fonctionne
- [ ] `git` disponible
- [ ] VM FreeBSD démarrée OU plan de travail sur macOS documenté
- [ ] Sources FreeBSD clonées ou accessibles en ligne
- [ ] `ctags` ou `cscope` configuré
- [ ] Anki installé + `flashcards/cards.tsv` importé
- [ ] `reference/toc.md` rempli avec les vraies pages
- [ ] `code/common/Makefile` compile sans erreur

**Une fois toutes les cases cochées → ouvre `days/day-001.md` et commence.**
