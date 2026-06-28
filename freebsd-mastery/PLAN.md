# PLAN — Curriculum complet FreeBSD Mastery

> Les durées sont des **estimations** basées sur ~5-10 pages de lecture active par session de 40 min.
> **Recalibre** dès que tu as rempli `reference/toc.md` avec les vraies pages.
> Priorité absolue : IPC / sockets / réseau (Phase 2).

---

## Vue d'ensemble des phases

| Phase | Thème | Durée estimée | Jours |
|-------|-------|---------------|-------|
| 0 | Mise en route & socle conceptuel | ~1 semaine | 1-7 |
| 1 | Noyau, processus, mémoire, E/S | ~7-8 semaines | 8-62 |
| 2 | **IPC & Réseau (PRIORITÉ)** | ~9-10 semaines | 63-133 |
| 3 | Stockage & systèmes de fichiers | ~6-7 semaines | 134-182 |
| 4 | Sécurité & opération | ~4 semaines | 183-210 |
| 5 | Synthèse & capstone | ~3 semaines | 211-231 |

**Horizon réaliste à 1 h/jour :**
- Phases 0→2 (socle + réseau/sockets) ≈ **4-5 mois** → vrai niveau « savoir s'en servir » sur la priorité.
- Arc complet + capstone ≈ **8-12 mois**.
- Personne ne « maîtrise » un noyau entier d'un bloc : expertise pointue là où tu travailles, bonne littératie ailleurs.

---

## Phase 0 — Mise en route & socle conceptuel (~7 jours)

**Objectif** : environnement opérationnel, carte mentale de FreeBSD, premiers appels système tracés.

| Jour | Type | Contenu | Projet code | Sources FreeBSD |
|------|------|---------|------------|-----------------|
| 1 | SETUP | Setup complet + calibration TOC + Ch. 1 §1 | — | — |
| 2 | LECTURE | Ch. 1 — Historique & origines BSD | — | — |
| 3 | LECTURE | Ch. 1 fin + Ch. 2 §1-2 (architecture) | — | — |
| 4 | LECTURE | Ch. 2 §3-4 (composants, appels système) | — | — |
| 5 | CODE | hello-trace : compile + truss/ktrace | `code/common/` | — |
| 6 | TRACE | Exploration syscalls, mode user/kernel | `code/common/hello_trace.c` | — |
| 7 | REVUE | Revue hebdo Phase 0 | — | — |

---

## Phase 1 — Le noyau & les processus (~55 jours, jours 8-62)

**Objectif** : comprendre le squelette du noyau, le cycle de vie des processus, la gestion mémoire et la couche VFS. Bases indispensables pour la Phase 2 (mbuf, buffers de socket, sockets-comme-fichiers).

### Semaine 2 (jours 8-14) — Ch. 3 Services du noyau

| Jour | Type | Contenu | Projet | Sources FreeBSD |
|------|------|---------|--------|-----------------|
| 8 | LECTURE | Ch. 3 §1 — Structure du noyau, organisation | — | `sys/sys/param.h`, `sys/kern/init_main.c` |
| 9 | LECTURE | Ch. 3 §2 — Synchronisation, verrous, mutexes | — | `sys/sys/lock.h`, `sys/kern/kern_mutex.c` |
| 10 | LECTURE | Ch. 3 §3 — sleep/wakeup, interruptions, timers | — | `sys/kern/kern_synch.c` |
| 11 | CODE | fork/exec/wait basique en C | `code/01-processes/` | — |
| 12 | LECTURE | Ch. 4 §1 — Structure du processus, `struct proc` | — | `sys/sys/proc.h` |
| 13 | LECTURE | Ch. 4 §2 — Création de processus, fork | — | `sys/kern/kern_fork.c` |
| 14 | REVUE | Revue hebdo — Ch. 3 + Ch. 4 début | — | — |

### Semaine 3 (jours 15-21) — Ch. 4 Gestion des processus (suite)

| Jour | Type | Contenu | Projet | Sources FreeBSD |
|------|------|---------|--------|-----------------|
| 15 | LECTURE | Ch. 4 §3 — exec, chargement de programme | — | `sys/kern/kern_exec.c` |
| 16 | LECTURE | Ch. 4 §4 — Signaux | — | `sys/kern/kern_sig.c` |
| 17 | CODE | Mini-shell partie 1 : fork/exec/wait | `code/01-processes/minishell.c` | — |
| 18 | CODE | Mini-shell partie 2 : signaux, job control | `code/01-processes/minishell.c` | — |
| 19 | TRACE | Trace kern_fork.c + kern_exec.c | — | `sys/kern/kern_fork.c`, `kern_exec.c` |
| 20 | LECTURE | Ch. 4 §5 — Terminaison, ordonnancement ULE | — | `sys/kern/sched_ule.c` |
| 21 | REVUE | Revue hebdo — Ch. 4 complet | — | — |

### Semaine 4 (jours 22-28) — Ch. 6 Gestion mémoire VM

| Jour | Type | Contenu | Projet | Sources FreeBSD |
|------|------|---------|--------|-----------------|
| 22 | LECTURE | Ch. 6 §1 — Architecture VM, vm_map, vm_object | — | `sys/vm/vm_map.h`, `sys/vm/vm_object.h` |
| 23 | LECTURE | Ch. 6 §2 — Pagination à la demande, vm_fault | — | `sys/vm/vm_fault.c` |
| 24 | LECTURE | Ch. 6 §3 — Allocateur UMA (slab) | — | `sys/vm/uma_core.c`, `sys/vm/uma.h` |
| 25 | CODE | Expériences mmap + mesure défauts de page | `code/02-memory/mmap_exp.c` | — |
| 26 | CODE | Allocateur jouet niveau 1 : bump allocator | `code/02-memory/alloc_toy.c` | — |
| 27 | CODE | Allocateur jouet niveau 2 : free list | `code/02-memory/alloc_toy.c` | — |
| 28 | REVUE | Revue hebdo — Ch. 6 | — | — |

### Semaine 5 (jours 29-35) — Ch. 6 VM (fin) + Ch. 7 E/S overview

| Jour | Type | Contenu | Projet | Sources FreeBSD |
|------|------|---------|--------|-----------------|
| 29 | LECTURE | Ch. 6 §4 — Swap, paging, vm_pageout | — | `sys/vm/vm_pageout.c` |
| 30 | CODE | Allocateur jouet niveau 3 : slab simple | `code/02-memory/alloc_toy.c` | `sys/vm/uma_core.c` |
| 31 | TRACE | Navigation sys/vm/ : vm_map.c, vm_fault.c | — | `sys/vm/` |
| 32 | LECTURE | Ch. 7 §1 — Vue d'ensemble E/S, descripteurs | — | `sys/kern/kern_descrip.c` |
| 33 | LECTURE | Ch. 7 §2 — VFS, vnodes | — | `sys/kern/vfs_lookup.c`, `sys/sys/vnode.h` |
| 34 | LECTURE | Ch. 7 §3 — Opérations vnode, VOP | — | `sys/kern/vfs_vnops.c` |
| 35 | REVUE | Revue hebdo — Ch. 7 | — | — |

### Semaines 6-8 (jours 36-56) — Projets 01-processes, 02-memory, 03-io-vfs

| Jour | Type | Contenu | Projet | Sources FreeBSD |
|------|------|---------|--------|-----------------|
| 36 | CODE | Mini-shell : amélioration (pipes internes) | `code/01-processes/` | — |
| 37 | TRACE | Trace d'un open/read/write sous truss | — | `sys/kern/vfs_syscalls.c` |
| 38 | CODE | FUSE mini-FS : opérations de base | `code/03-io-vfs/` | `sys/kern/vfs_*.c` |
| 39 | CODE | FUSE mini-FS : répertoires | `code/03-io-vfs/` | — |
| 40 | CODE | FUSE mini-FS : lecture/écriture | `code/03-io-vfs/` | — |
| 41 | TRACE | Navigation sys/kern/vfs_*.c | — | `sys/kern/vfs_*.c` |
| 42 | REVUE | Revue hebdo — Projets Phase 1 | — | — |
| 43-56 | MIXTE | Approfondissement + buffer phase 1 | `code/01-03/` | — |
| 57-62 | MIXTE | Consolidation Phase 1 + transition Phase 2 | — | — |

---

## Phase 2 — IPC & Réseau — PRIORITÉ MAXIMALE (~70 jours, jours 63-133)

**Objectif** : maîtriser en profondeur la pile réseau FreeBSD, les sockets, les mbufs, TCP/IP — en recoupant avec ton étude parallèle des RFC 793/9293, 2616/9110, 1034-1035, 6265.

### Ch. 12 — IPC & Sockets (jours 63-84)

| Jour | Type | Contenu | Projet | Sources FreeBSD |
|------|------|---------|--------|-----------------|
| 63 | LECTURE | Ch. 12 §1 — Vue d'ensemble IPC, struct socket | — | `sys/sys/socketvar.h`, `sys/sys/socket.h` |
| 64 | LECTURE | Ch. 12 §2 — Buffers de socket (sockbuf) | — | `sys/kern/uipc_socket.c` |
| 65 | LECTURE | Ch. 12 §3 — Domaines de protocole, PRU | — | `sys/kern/uipc_domain.c` |
| 66 | LECTURE | Ch. 12 §4 — Sockets Unix (AF_UNIX) | — | `sys/kern/uipc_usrreq.c` |
| 67 | CODE | Serveur/client TCP echo (AF_INET) | `code/05-ipc-sockets/echo_server.c` | — |
| 68 | CODE | Boucle d'événements : select/poll | `code/05-ipc-sockets/event_loop.c` | — |
| 69 | CODE | Boucle d'événements : kqueue/kevent | `code/05-ipc-sockets/event_kqueue.c` | — |
| 70 | CODE | Comparatif select/poll/kqueue : benchmark | `code/05-ipc-sockets/bench_io.c` | — |
| 71 | TRACE | Navigation uipc_socket.c : socket(), bind(), listen() | — | `sys/kern/uipc_socket.c` |
| 72 | TRACE | Navigation uipc_socket.c : accept(), connect() | — | `sys/kern/uipc_socket.c` |
| 73 | LECTURE | Ch. 12 §5 — Mbufs : structure, chaînes | — | `sys/sys/mbuf.h`, `sys/kern/uipc_mbuf.c` |
| 74 | CODE | Dessin + implémentation jouet de chaîne mbuf | `code/05-ipc-sockets/mbuf_toy.c` | `sys/sys/mbuf.h` |
| 75 | LECTURE | Ch. 12 §6 — Routage entre socket et protocole | — | `sys/net/route.c` |
| 76 | CODE | Serveur HTTP/1.1 minimal en C — partie 1 (parsing) | `code/05-ipc-sockets/http_server.c` | — |
| 77 | REVUE | Revue hebdo — Ch. 12 | — | — |
| 78 | CODE | Serveur HTTP/1.1 — partie 2 (réponses, keep-alive) | `code/05-ipc-sockets/http_server.c` | — |
| 79 | CODE | Serveur HTTP/1.1 — partie 3 (concurrence, kqueue) | `code/05-ipc-sockets/http_server.c` | — |
| 80-84 | MIXTE | Approfondissement sockets + trace complète | — | `uipc_socket.c`, `uipc_mbuf.c` |

### Ch. 13 — Couche réseau (jours 85-105)

| Jour | Type | Contenu | Projet | Sources FreeBSD |
|------|------|---------|--------|-----------------|
| 85 | LECTURE | Ch. 13 §1 — Architecture réseau, ifnet | — | `sys/net/if.h`, `sys/net/if_var.h` |
| 86 | LECTURE | Ch. 13 §2 — IP : structure, fragmentation | — | `sys/netinet/ip.h`, `sys/netinet/ip_input.c` |
| 87 | LECTURE | Ch. 13 §3 — IP output, routage | — | `sys/netinet/ip_output.c` |
| 88 | CODE | Parser en C des en-têtes IP depuis paquets bruts | `code/06-network-layer/ip_parser.c` | — |
| 89 | CODE | Implémentation du checksum IP/TCP | `code/06-network-layer/checksum.c` | — |
| 90 | CODE | Capture/dissection avec tcpdump/tshark | `code/06-network-layer/` | — |
| 91 | TRACE | Navigation ip_input.c + ip_output.c | — | `sys/netinet/ip_input.c`, `ip_output.c` |
| 92 | LECTURE | Ch. 13 §4 — ICMP, ARP | — | `sys/netinet/icmp_var.h` |
| 93-98 | MIXTE | Projets réseau + approfondissement | `code/06-network-layer/` | — |
| 99-105 | MIXTE | Consolidation couche réseau | — | — |

### Ch. 14 — Couche transport (jours 106-133)

| Jour | Type | Contenu | Projet | Sources FreeBSD |
|------|------|---------|--------|-----------------|
| 106 | LECTURE | Ch. 14 §1 — UDP, structure, usrreq | — | `sys/netinet/udp_usrreq.c`, `udp_var.h` |
| 107 | LECTURE | Ch. 14 §2 — TCP : survol, états | — | `sys/netinet/tcp_var.h`, `sys/netinet/tcp_fsm.h` |
| 108 | LECTURE | Ch. 14 §3 — TCP input, connexion | — | `sys/netinet/tcp_input.c` |
| 109 | LECTURE | Ch. 14 §4 — TCP output, fenêtre, congestion | — | `sys/netinet/tcp_output.c` |
| 110 | CODE | Transport fiable jouet / UDP : stop-and-wait | `code/07-transport/rudp_sw.c` | — |
| 111 | CODE | Transport fiable jouet : fenêtre glissante | `code/07-transport/rudp_win.c` | — |
| 112 | CODE | Machine d'état TCP partielle (handshake) | `code/07-transport/tcp_fsm_toy.c` | `sys/netinet/tcp_fsm.h` |
| 113 | TRACE | Navigation tcp_input.c + tcp_output.c | — | `sys/netinet/tcp_input.c`, `tcp_output.c` |
| 114 | REVUE | Revue hebdo — Ch. 14 | — | — |
| 115-133 | MIXTE | Approfondissement TCP + cross-référence RFC 793/9293 | `code/07-transport/` | — |

---

## Phase 3 — Stockage & systèmes de fichiers (~49 jours, jours 134-182)

| Jour | Type | Contenu | Projet | Sources FreeBSD |
|------|------|---------|--------|-----------------|
| 134-145 | LECTURE | Ch. 9 — FFS : superbloc, inodes, bitmap, soft updates | — | `sys/ufs/ffs/`, `sys/ufs/ufs/` |
| 146-155 | CODE | Mini-FS dans une image-fichier (superbloc, inodes, dirs) | `code/04-filesystems/mini_fs.c` | — |
| 156-160 | LECTURE | Ch. 8 — Périphériques, drivers (vue légère) | — | `sys/dev/` |
| 161-167 | LECTURE | Ch. 10 — ZFS : COW, snapshots (conceptuel) | — | `sys/contrib/openzfs/` |
| 168-172 | LECTURE | Ch. 11 — NFS (lecture légère) | — | `sys/nfs/` |
| 173-175 | CODE | Expériences zpool/zfs en VM (optionnel) | — | — |
| 176-182 | REVUE | Revues + consolidation Phase 3 | — | — |

---

## Phase 4 — Sécurité & opération (~28 jours, jours 183-210)

| Jour | Type | Contenu | Projet | Sources FreeBSD |
|------|------|---------|--------|-----------------|
| 183-190 | LECTURE | Ch. 5 — Sécurité : Capsicum, MAC, jails | — | `sys/kern/sys_capability.c`, `sys/security/mac/` |
| 191-196 | CODE | Bac à sable Capsicum (cap_enter()), jails | `code/08-security/` | `sys/kern/sys_capability.c` |
| 197-202 | LECTURE | Ch. 15 — Démarrage/arrêt | — | `sys/kern/init_main.c` |
| 203-210 | MIXTE | Consolidation + expériences MAC | `code/08-security/` | `sys/security/mac/` |

---

## Phase 5 — Synthèse & capstone (~21 jours, jours 211-231)

| Jour | Type | Contenu | Projet | Sources |
|------|------|---------|--------|---------|
| 211-225 | CODE | Serveur HTTP concurrent (kqueue) + Capsicum + cache disque | `code/99-capstone/` | — |
| 226-231 | REVUE | Rappel global, redessins de mémoire, bilan final | — | — |

---

## Tableau récapitulatif Chapitre → Sources FreeBSD

| Chapitre | Thème | Sources FreeBSD clés |
|----------|-------|----------------------|
| Ch. 1 | Historique | — |
| Ch. 2 | Vue d'ensemble | `sys/sys/param.h`, `sys/kern/init_main.c` |
| Ch. 3 | Services noyau | `sys/kern/kern_mutex.c`, `kern_synch.c` |
| Ch. 4 | Processus | `sys/kern/kern_fork.c`, `kern_exec.c`, `kern_sig.c`, `sched_ule.c`, `sys/sys/proc.h` |
| Ch. 5 | Sécurité | `sys/kern/sys_capability.c`, `sys/security/mac/` |
| Ch. 6 | VM | `sys/vm/vm_map.c`, `vm_fault.c`, `uma_core.c`, `vm_pageout.c` |
| Ch. 7 | E/S / VFS | `sys/kern/vfs_*.c`, `sys/sys/vnode.h`, `sys/kern/kern_descrip.c` |
| Ch. 8 | Périphériques | `sys/dev/` |
| Ch. 9 | FFS | `sys/ufs/ffs/`, `sys/ufs/ufs/` |
| Ch. 10 | ZFS | `sys/contrib/openzfs/` |
| Ch. 11 | NFS | `sys/nfs/` |
| Ch. 12 | IPC / Sockets | `sys/kern/uipc_socket.c`, `uipc_usrreq.c`, `uipc_mbuf.c`, `sys/sys/socketvar.h`, `mbuf.h` |
| Ch. 13 | Réseau | `sys/netinet/ip_input.c`, `ip_output.c`, `sys/net/if.h` |
| Ch. 14 | Transport | `sys/netinet/tcp_input.c`, `tcp_output.c`, `tcp_var.h`, `tcp_fsm.h`, `udp_usrreq.c` |
| Ch. 15 | Boot/Arrêt | `sys/kern/init_main.c` |

---

## Notes de calibration

- Une fois `reference/toc.md` rempli, ajuste les blocs de lecture : 5-10 pages actives par 40 min.
- Si un chapitre est plus court que prévu → compresse les jours, avance ; s'il est plus long → étale.
- Les projets de code sont des jalons, pas des dates fixes. La compréhension prime sur le rythme.
- Les jours « buffer » et « mixte » sont intentionnels : chaque noyau OS cache des surprises.
