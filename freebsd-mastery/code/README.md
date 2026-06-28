# code/ — Projets C du cours FreeBSD Mastery

## Organisation

```
code/
  common/          utilitaires partagés, Makefile générique
  01-processes/    fork, exec, wait, signaux, mini-shell
  02-memory/       mmap, allocateurs jouet (bump → slab)
  03-io-vfs/       FUSE mini-FS, trace d'opérations fichier
  04-filesystems/  mini-FS dans une image-fichier
  05-ipc-sockets/  PRIORITÉ — sockets TCP, kqueue, HTTP server, mbuf toy
  06-network-layer/ parser IP/TCP, checksum, dissection paquets
  07-transport/    transport fiable sur UDP, machine d'état TCP
  08-security/     Capsicum, jails (sur VM FreeBSD)
  99-capstone/     serveur HTTP concurrent + Capsicum + cache disque
```

---

## Le motif « jouet puis vrai »

Pour chaque sous-système :
1. **Jouet** : implémente une version minimale en C qui capture l'essence du concept.
2. **Vrai** : lis le code source FreeBSD correspondant, réponds à 2 questions ciblées.
3. **Comparaison** : note les différences entre ta version jouet et la vraie (gestion d'erreurs, cas limites, performance).

Ce motif force à avoir un modèle mental avant de lire le vrai code, rendant la lecture bien plus efficace.

---

## Build

Chaque sous-dossier a son propre `Makefile` (ou peut utiliser `code/common/Makefile`).

```sh
# Compiler depuis un sous-dossier :
cd code/05-ipc-sockets
make

# Ou depuis la racine du dépôt :
make -C code/05-ipc-sockets echo_server

# Nettoyage :
make -C code/05-ipc-sockets clean
```

Flags de compilation toujours actifs : `-Wall -Wextra -g` (warnings maximaux + symboles de debug).

---

## Notes d'environnement

- **Cible principale** : FreeBSD 14.x/15.x (VM recommandée).
- `kqueue`, Capsicum, jails, `truss`, `ktrace` : FreeBSD uniquement.
- `kqueue` fonctionne aussi sur macOS pour les projets `05-ipc-sockets/` et `06-07`.
- Les projets `08-security/` nécessitent impérativement la VM FreeBSD.
- Compilateur : `cc` (alias de clang sur FreeBSD/macOS) ou `gcc` sur Linux.

---

## Règle d'or (à se répéter)

> *Si je ne peux pas le réimplémenter ou le redessiner de mémoire, je ne l'ai pas compris.*

Le code que tu écris ici n'est pas de la production. C'est un outil de compréhension.
Bugs, simplifications, et limitations sont attendus et bienvenus — note-les dans les README.
