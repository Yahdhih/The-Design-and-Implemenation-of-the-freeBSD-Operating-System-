# code/04-filesystems/ — Mini-FS dans une image-fichier

Correspond à **Ch. 9 — FFS** (et concepts de Ch. 7).
Phase 3 — voir `PLAN.md` pour le calendrier.

---

## Projet : mini-FS dans une image-fichier (`mini_fs.c`)

Implémente un système de fichiers minimal dans un fichier ordinaire (image disque simulée).
C'est le projet classique qui ancre définitivement la structure d'un FS : superbloc, inodes, bitmap, répertoires.

### Architecture cible

```
image.bin (ex. 1 Mo)
  ┌─────────────────────────────────────────────┐
  │ Bloc 0 : superbloc                          │
  │   - magic number, taille totale, nb inodes  │
  │   - offset du bitmap, offset des inodes     │
  │   - offset des blocs de données             │
  ├─────────────────────────────────────────────┤
  │ Blocs 1..K : bitmap des blocs libres        │
  ├─────────────────────────────────────────────┤
  │ Blocs K+1..K+M : table d'inodes             │
  │   - inode 0 = répertoire racine /           │
  ├─────────────────────────────────────────────┤
  │ Blocs K+M+1..N : données                    │
  └─────────────────────────────────────────────┘
```

### Jalons progressifs

- [ ] **Jalon 1** : créer l'image, écrire/lire le superbloc, vérifier le magic number.
- [ ] **Jalon 2** : implémenter le bitmap de blocs (alloc_block, free_block).
- [ ] **Jalon 3** : implémenter les inodes (create_inode, read_inode, write_inode).
- [ ] **Jalon 4** : écrire et lire un fichier (open, write, read via les pointeurs directs de l'inode).
- [ ] **Jalon 5** : répertoires (create_dir, ls, lookup par nom).
- [ ] **Jalon 6** : blocs indirects (pour les fichiers > 12 blocs directs).
- [ ] **(Bonus)** : implémenter un superbloc de secours et un fsck minimal.

```sh
make mini_fs
./mini_fs init image.bin    # crée l'image
./mini_fs write image.bin /hello.txt "Bonjour FreeBSD"
./mini_fs read image.bin /hello.txt
# Attendu : "Bonjour FreeBSD"
./mini_fs ls image.bin /
# Attendu : hello.txt
```

**Critère « terminé quand »** : écrire un fichier, le lire, lister le répertoire racine.

---

## Sources FreeBSD clés

| Fichier | Ce qu'on cherche |
|---------|-----------------|
| `sys/ufs/ffs/fs.h` | struct fs (superbloc FFS), struct cg (groupe de cylindres) |
| `sys/ufs/ufs/inode.h` | struct inode, struct dinode |
| `sys/ufs/ufs/dir.h` | struct direct (entrée de répertoire) |
| `sys/ufs/ffs/ffs_alloc.c` | Allocation de blocs dans FFS |
| `sys/ufs/ffs/ffs_inode.c` | Lecture/écriture d'inodes |

---

## Concepts clés à comprendre via ce projet

- Pourquoi les inodes ne contiennent pas le nom de fichier ?
- Que se passe-t-il quand on `unlink()` un fichier encore ouvert ?
- Comment les liens durs (hard links) sont-ils implémentés avec les inodes ?
- Soft updates : dans quel ordre doit-on écrire bitmap / inode / données pour éviter les incohérences après un crash ?
