# Glossaire — FreeBSD Mastery

> Enrichis ce glossaire au fil de ta lecture. Format : **terme** — définition courte + source.

---

## A

**allocateur slab (UMA)** — allocateur mémoire noyau FreeBSD basé sur des « slabs » d'objets pré-alloués de même taille. Réduit la fragmentation et accélère les allocations fréquentes (ex. : mbufs, vnodes). Source : `sys/vm/uma_core.c`.

**appel système (syscall)** — interface entre le code utilisateur et le noyau. Déclenché par l'instruction `SYSCALL` (x86-64), dispatché via la table `sysent[]`. Exemples : `read()`, `write()`, `socket()`, `fork()`.

## B

**bhyve** — hyperviseur natif FreeBSD, intégré au noyau. Permet de faire tourner des VMs directement sur FreeBSD sans logiciel tiers.

**bump allocator** — allocateur trivial : un pointeur qui avance. Allocation O(1), libération impossible individuellement. Point de départ des projets `code/02-memory/`.

## C

**Capsicum** — modèle de sécurité à capacités de FreeBSD. Après `cap_enter()`, un processus ne peut accéder qu'aux ressources via des descripteurs à capacités limitées. Source : `sys/kern/sys_capability.c`.

**COW (Copy-On-Write)** — technique où deux processus partagent les mêmes pages physiques après `fork()`. La copie physique n'a lieu qu'à la première écriture. Rend `fork()` très rapide.

**cscope** — outil de navigation dans les sources C : trouve où une fonction est définie, où elle est appelée, où un symbole est utilisé.

**ctags** — génère un index de symboles (fonctions, struct, macros) pour naviguer dans les sources depuis vim/emacs/VSCode.

## D

**demand paging** — stratégie de gestion mémoire où les pages ne sont chargées en mémoire physique que lorsqu'elles sont effectivement accédées (faute de page). Source : `sys/vm/vm_fault.c`.

**descripteur de fichier (fd)** — entier côté processus référençant une entrée dans la table de descripteurs noyau. Pointe vers une `struct file`, elle-même pointant vers une ressource (vnode, socket, pipe…). Source : `sys/kern/kern_descrip.c`.

## F

**faute de page (page fault)** — exception matérielle déclenchée lorsqu'un accès mémoire touche une page non présente en RAM ou non mappée. Le noyau la gère dans `vm_fault()`.

**FFS (Fast File System)** — système de fichiers principal de FreeBSD, descendant du BSD FFS de Kirk McKusick. Basé sur les groupes de cylindres, inodes, et bitmap de blocs. Source : `sys/ufs/ffs/`.

**fork()** — appel système qui crée un processus fils identique au père (avec COW sur les pages). Source : `sys/kern/kern_fork.c`.

## G

**groupe de cylindres (cylinder group)** — unité organisationnelle de FFS regroupant des blocs de données et leurs métadonnées (bitmap, inodes) pour améliorer la localité.

## I

**inode** — structure décrivant un fichier dans UFS/FFS : permissions, propriétaire, taille, pointeurs vers les blocs de données. Ne contient PAS le nom de fichier (dans le répertoire).

**IPC (Inter-Process Communication)** — mécanismes de communication entre processus : pipes, FIFO, sockets Unix, sockets réseau, mémoire partagée, sémaphores, files de messages.

## J

**jail** — mécanisme de virtualisation légère FreeBSD : isole un ensemble de processus dans un sous-arbre FS + interfaces réseau propres. Plus léger qu'une VM, plus fort qu'un `chroot`.

## K

**kdump** — commande FreeBSD qui décode le fichier binaire produit par `ktrace`. Affiche les syscalls, signaux, et E/S tracés.

**kevent** — structure décrivant un événement dans le système `kqueue`. Contient : identifiant, filtre (EVFILT_READ, EVFILT_WRITE…), flags, et données.

**kqueue** — mécanisme de notification d'événements scalable de FreeBSD (et macOS). Remplace `select`/`poll` pour les serveurs gérant de nombreuses connexions. Source : `sys/kern/kern_event.c`.

**ktrace** — outil FreeBSD de traçage noyau léger. Écrit les événements dans un fichier binaire. Décodé par `kdump`. Voir `truss` pour une version temps réel.

## M

**mbuf (memory buffer)** — structure de base de la pile réseau FreeBSD pour stocker les données de paquets. Les mbufs sont chaînés pour représenter des paquets de taille variable sans copies. Source : `sys/sys/mbuf.h`, `sys/kern/uipc_mbuf.c`.

**MAC (Mandatory Access Control)** — module de contrôle d'accès obligatoire de FreeBSD. Permet d'implémenter des politiques de sécurité personnalisées (ex. : MLS, Biba). Source : `sys/security/mac/`.

**mutex** — verrou d'exclusion mutuelle noyau FreeBSD. Types : `MTX_DEF` (dormant autorisé), `MTX_SPIN` (spin lock, pas de dormance). Source : `sys/kern/kern_mutex.c`.

## N

**noyau monolithique** — architecture où tous les services OS (gestion mémoire, FS, réseau, pilotes) s'exécutent en espace noyau dans un seul espace d'adressage. FreeBSD est un noyau monolithique (avec modules chargeables).

## P

**PRU (Protocol User Requests)** — interface entre la couche socket générique et les protocoles spécifiques (TCP, UDP…). Fonctions comme `PRU_CONNECT`, `PRU_SEND`, `PRU_ACCEPT`. Source : `sys/kern/uipc_socket.c`.

**proc (struct proc)** — structure noyau représentant un processus FreeBSD. Contient : PID, UID/GID, descripteurs, espace d'adressage (p_vmspace), threads (p_threads), signal mask… Source : `sys/sys/proc.h`.

## S

**sched_ule** — ordonnanceur par défaut de FreeBSD. Préemptif, multi-niveaux, conscient de la topologie CPU (NUMA, SMP). Source : `sys/kern/sched_ule.c`.

**sleep/wakeup** — primitives de synchronisation noyau. Un thread peut dormir (`tsleep()`) en attendant un événement identifié par un « canal » (pointeur). `wakeup()` réveille tous les threads en attente sur ce canal. Source : `sys/kern/kern_synch.c`.

**sockbuf** — buffer d'envoi ou de réception d'un socket (`so_snd`, `so_rcv`). Chaîne de mbufs avec compteurs de taille et limites (`sb_hiwat`). Source : `sys/sys/socketvar.h`.

**socket (struct socket)** — structure noyau représentant un endpoint de communication. Contient : type, état, protocole, sockbufs d'envoi/réception, file d'attente des connexions. Source : `sys/sys/socketvar.h`.

**soft updates** — mécanisme FFS garantissant la cohérence du système de fichiers sans journaling, via un ordonnancement partiel des écritures sur disque.

## T

**thread (struct thread)** — unité d'exécution dans FreeBSD. Un processus peut avoir plusieurs threads partageant son espace d'adressage. Contient : contexte CPU, pile noyau, état (TDS_RUNNING, TDS_SLEEPING…). Source : `sys/sys/proc.h`.

**truss** — équivalent FreeBSD de `strace`. Trace les appels système en temps réel avec leurs arguments et valeurs de retour.

## U

**ULE** — voir `sched_ule`.

**UMA (Universal Memory Allocator)** — allocateur slab du noyau FreeBSD. Voir `allocateur slab`.

## V

**VFS (Virtual File System)** — couche d'abstraction fournissant une interface uniforme pour tous les systèmes de fichiers. Basée sur les vnodes.

**vm_map** — structure représentant l'espace d'adressage virtuel d'un processus. Contient une liste triée de `vm_map_entry`. Source : `sys/vm/vm_map.h`.

**vm_object** — abstraction d'une source de données pouvant être mappée en mémoire (fichier, zone anonyme, device…). Source : `sys/vm/vm_object.h`.

**vnode** — représentation abstraite d'un objet fichier dans le VFS. Contient des pointeurs vers les opérations spécifiques au FS sous-jacent (vops). Source : `sys/sys/vnode.h`.

## Z

**ZFS** — système de fichiers avancé intégré à FreeBSD. COW, checksums intégrés, snapshots, pools de stockage. Source : `sys/contrib/openzfs/`.
