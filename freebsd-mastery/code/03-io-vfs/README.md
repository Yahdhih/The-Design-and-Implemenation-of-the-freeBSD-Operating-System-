# code/03-io-vfs/ — E/S, VFS, vnodes, FUSE

Correspond à **Ch. 7 — Vue d'ensemble du système d'E/S**.

---

## Projets

### Jalon 1 — Trace d'opérations fichier

**Objectif** : voir concrètement le chemin d'un `open/read/write/close` au niveau noyau.

```sh
# Sur FreeBSD :
truss -o /tmp/trace.txt cat /etc/motd
cat /tmp/trace.txt | grep -E "open|read|write|close"

# Questions à répondre depuis la trace :
# 1. Quel fd est retourné par open() ? Par combien de read() se termine la lecture ?
# 2. Y a-t-il des appels inattendus (mmap, mprotect) pour un simple cat ?
```

**Critère** : avoir répondu aux 2 questions et noté les réponses dans `reference/notes/`.

---

### Jalon 2 — FUSE mini-FS (`fuse_mini.c`)

**Objectif** : implémenter les opérations vnode de base via FUSE pour ancrer le modèle VFS.

FUSE (Filesystem in USErspace) permet d'écrire un FS en espace utilisateur qui implémente exactement les vops que FreeBSD attend. C'est le miroir parfait du VFS noyau.

```sh
# Installation de FUSE sur FreeBSD :
pkg install -y fusefs-libs3
kldload fusefs

# Installation sur macOS :
brew install macfuse
```

Opérations à implémenter (dans cet ordre) :
- [ ] `getattr` : retourne les métadonnées d'un fichier (stat)
- [ ] `readdir` : liste le contenu d'un répertoire
- [ ] `open` / `release` : ouvre et ferme un fichier
- [ ] `read` : lit le contenu d'un fichier (données en mémoire d'abord)
- [ ] `write` : écrit dans un fichier
- [ ] `mkdir` / `rmdir`
- [ ] `create` / `unlink`

```sh
make fuse_mini
mkdir /tmp/testmount
./fuse_mini /tmp/testmount
ls /tmp/testmount          # doit lister les fichiers virtuels
cat /tmp/testmount/hello   # doit retourner "Hello, VFS!"
fusermount -u /tmp/testmount  # (Linux) ou diskutil unmount /tmp/testmount (macOS)
```

**Critère** : `ls` et `cat` fonctionnent sur le FS monté.

---

## Sources FreeBSD clés

| Fichier | Ce qu'on cherche |
|---------|-----------------|
| `sys/kern/vfs_lookup.c` | `namei()` : résolution d'un chemin en vnode |
| `sys/kern/vfs_vnops.c`  | Wrappers VOP_READ, VOP_WRITE, VOP_OPEN… |
| `sys/kern/vfs_syscalls.c` | Implémentation de open(), read(), write() côté syscall |
| `sys/kern/kern_descrip.c` | Gestion de la table de descripteurs |
| `sys/sys/vnode.h`        | struct vnode, struct vop_vector |

**Questions de trace** :
1. Dans `vfs_lookup.c`, comment `namei()` gère-t-il les liens symboliques ? Y a-t-il une protection contre les boucles ?
2. Dans `vfs_vnops.c`, comment `VOP_READ` sait-il quelle implémentation appeler (UFS vs ZFS vs NFS) ?

---

## Schémas à dessiner

1. Chemin d'un `open("/tmp/foo", O_RDONLY)` : syscall → VFS → namei() → vnode → inode.
2. Relation struct file ↔ vnode ↔ inode pour un fichier ouvert par 2 processus.
3. Table des vops : liste des opérations et qui les implémente (UFS, NFS, devfs…).

*(Voir `reference/diagrams/` pour y stocker tes schémas.)*
