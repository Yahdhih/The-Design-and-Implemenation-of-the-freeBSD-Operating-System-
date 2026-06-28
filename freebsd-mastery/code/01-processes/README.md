# code/01-processes/ — Processus, fork/exec/wait, signaux, mini-shell

Correspond à **Ch. 4 — Gestion des processus** (et appuis sur Ch. 3).

---

## Projets

### Jalon 1 — fork/exec/wait basique (`fork_exec.c`)

**Objectif** : observer concrètement la création de processus et la récupération du statut.

```c
// Comportement attendu :
// - Le fils exécute "/bin/ls -l"
// - Le père attend et affiche le code de retour
```

```sh
make fork_exec
./fork_exec
# Attendu : listing du répertoire courant, puis "fils terminé avec code 0"
```

Sources FreeBSD de référence : `sys/kern/kern_fork.c`, `sys/kern/kern_exec.c`, `sys/kern/kern_wait.c`.

Questions de trace à répondre :
1. Dans `kern_fork.c`, quelle fonction réalise la copie des descripteurs de fichiers du père vers le fils ?
2. Dans `kern_exec.c`, à quel moment l'ancien espace d'adressage du processus est-il libéré ?

---

### Jalon 2 — Signaux (`signals.c`)

**Objectif** : installer un handler de signal, comprendre la livraison asynchrone.

```c
// Comportement attendu :
// - Installe un handler pour SIGINT et SIGTERM
// - Boucle en attendant un signal
// - SIGINT : affiche "reçu SIGINT, continuons"
// - SIGTERM : affiche "reçu SIGTERM, au revoir" et quitte proprement
```

```sh
make signals
./signals &
kill -INT $!   # doit afficher le message sans quitter
kill -TERM $!  # doit quitter proprement
```

---

### Jalon 3 — Mini-shell (`minishell.c`)

**Objectif** : un shell minimal qui montre que fork/exec/wait + signaux suffisent à construire un shell réel.

Fonctionnalités à implémenter progressivement :
- [ ] Lecture d'une commande (readline ou fgets)
- [ ] Parsing basique (tokenisation sur les espaces)
- [ ] fork + exec + wait
- [ ] Commandes internes : `cd`, `exit`
- [ ] Redirection stdout (`>`) et stdin (`<`)
- [ ] Pipe entre deux commandes (`cmd1 | cmd2`)
- [ ] Gestion de SIGINT (Ctrl-C ne tue pas le shell)
- [ ] Contrôle de tâches : `cmd &` (job en arrière-plan)

```sh
make minishell
./minishell
$ ls -l | grep .c
$ echo hello > /tmp/test.txt
$ cat < /tmp/test.txt
```

**Critère « terminé quand »** : `ls | wc -l`, `echo hello > /tmp/x && cat /tmp/x`, et Ctrl-C ne tue pas le shell.

---

## Sources FreeBSD clés

| Fichier | Ce qu'on cherche |
|---------|-----------------|
| `sys/kern/kern_fork.c` | `fork1()` : copie de proc, COW, héritage des fd |
| `sys/kern/kern_exec.c` | `kern_execve()` : chargement de l'image, remplacement de vmspace |
| `sys/kern/kern_wait.c` | `kern_wait()` : attente et récupération du statut |
| `sys/kern/kern_sig.c`  | `kern_kill()`, `sigaction()`, livraison des signaux |
| `sys/kern/sched_ule.c` | `sched_fork()` : initialisation du scheduling pour le fils |
| `sys/sys/proc.h`       | `struct proc`, `struct thread`, champs clés |

---

## Schémas à dessiner

1. Arbre de processus après 3 fork() imbriqués : PIDs, relations père/fils.
2. Chemin d'un SIGTERM : kill() → noyau → livraison → handler utilisateur.
3. Cycle de vie d'un processus : NEW → RUNNABLE → RUNNING → SLEEPING → ZOMBIE → DEAD.
