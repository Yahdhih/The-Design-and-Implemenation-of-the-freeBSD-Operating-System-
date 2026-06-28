# Consignes de récupération — Page blanche par chapitre

> Ces consignes servent à la section « Réveil mémoire » de chaque journée.
> Méthode : ferme le livre et tes notes, écris ou dessine de mémoire pendant 5 min, puis compare.

---

## Chapitre 1 — Historique

1. **Frise BSD** : dessine une frise chronologique de Unix → BSD → 386BSD → FreeBSD/NetBSD/OpenBSD. Note 3 dates clés et ce qu'elles marquent.
2. **Licences** : explique en 2 phrases la différence entre la licence BSD et GPL. Pourquoi FreeBSD a choisi BSD ?
3. **Motivations** : cite 3 raisons pour lesquelles FreeBSD a divergé de Linux en termes de philosophie de développement.
4. **Aujourd'hui** : cite 3 usages réels de FreeBSD en production (serveurs, appliances, consoles...).

---

## Chapitre 2 — Vue d'ensemble de la conception

1. **Schéma d'architecture** : dessine de mémoire le diagramme des couches de FreeBSD (espace utilisateur, bibliothèques, appels système, noyau, matériel). Place 5 sous-systèmes aux bons niveaux.
2. **Chemin d'un syscall** : retrace pas à pas le chemin d'un `read()` depuis l'instruction SYSCALL jusqu'au retour en mode utilisateur. Note les structures traversées.
3. **struct proc** : liste de mémoire 6 champs de `struct proc` et leur rôle. Où est définie cette struct ?
4. **Mbuf chain** : dessine une chaîne de 3 mbufs représentant un paquet TCP. Montre m_next, m_data, m_len.
5. **IPC types** : liste 5 mécanismes IPC disponibles dans FreeBSD, avec un cas d'usage pour chacun.

---

## Chapitre 3 — Services du noyau

1. **Synchronisation** : explique la différence entre un mutex `MTX_DEF` et un mutex `MTX_SPIN`. Quand utiliser l'un plutôt que l'autre ?
2. **sleep/wakeup** : trace le chemin d'un appel `tsleep()` : que se passe-t-il jusqu'à ce que le thread soit réveillé par `wakeup()` ?
3. **Interruptions** : explique la différence entre une interruption matérielle et une softirq (ithread vs taskqueue) dans FreeBSD.
4. **Timers** : décris le mécanisme de callout FreeBSD. Comment planifie-t-on un callback temporisé dans le noyau ?

---

## Chapitre 4 — Gestion des processus

1. **fork() complet** : dessine de mémoire le chemin de `fork()` depuis l'appel utilisateur jusqu'au retour dans père et fils. Note les fichiers sources (`kern_fork.c`).
2. **exec() complet** : même exercice pour `exec()` : que se passe-t-il avec l'espace d'adressage, les signaux, les descripteurs de fichiers ?
3. **États d'un processus** : dessine la machine d'état des états d'un thread FreeBSD (TDS_RUNNING, TDS_SLEEPING, TDS_CANRUN…). Quels événements déclenchent les transitions ?
4. **Signaux** : explique le chemin d'un signal SIGTERM depuis l'émetteur (`kill(pid, SIGTERM)`) jusqu'à l'exécution du handler ou la terminaison du processus.
5. **Mini-shell** : de mémoire, écris le pseudo-code d'un shell minimal gérant : lecture de commande, fork, exec, wait, redirection stdout.

---

## Chapitre 6 — Gestion de la mémoire virtuelle

1. **Hiérarchie VM** : dessine la hiérarchie vm_map → vm_map_entry → vm_object → vm_page. Note un exemple concret pour chaque niveau.
2. **Faute de page** : trace le chemin complet d'une faute de page COW après `fork()`. Quel fichier source ?
3. **UMA/slab** : explique en 3 étapes comment UMA alloue un objet depuis une zone. Qu'est-ce qu'un slab ?
4. **mmap** : que se passe-t-il exactement dans le noyau lors d'un `mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_ANONYMOUS, -1, 0)` ? Quand la page physique est-elle allouée ?

---

## Chapitre 7 — Vue d'ensemble du système d'E/S

1. **VFS layers** : dessine le chemin d'un `open("/etc/passwd", O_RDONLY)` depuis l'appel jusqu'à l'inode sur disque. Note les structures traversées (struct file, vnode, inode).
2. **vops** : liste 5 opérations vnode (vops) et décris ce que chacune fait. Qui les implémente ?
3. **Descripteurs et struct file** : dessine la relation entre la table de descripteurs d'un processus, la struct file, et le vnode.

---

## Chapitre 12 — IPC & Sockets (PRIORITÉ)

1. **struct socket** : dessine de mémoire les champs clés de `struct socket`. Note les 2 sockbufs, le protocole, l'état.
2. **Chemin de socket()** : trace le chemin de l'appel `socket(AF_INET, SOCK_STREAM, 0)` jusqu'à la création de la `struct socket`. Fichier source ? Fonctions traversées ?
3. **Chaîne de mbufs** : dessine une chaîne de mbufs représentant un segment TCP de 2000 octets. Montre les champs clés de chaque mbuf.
4. **kqueue vs select** : explique pourquoi `kqueue` est O(1) par événement alors que `select` est O(N). Dessine l'architecture de chacun.
5. **Serveur TCP** : écris de mémoire le squelette C d'un serveur TCP avec kqueue : socket, bind, listen, kevent loop, accept, read, write.

---

## Chapitre 13 — Couche réseau

1. **Chemin IP entrant** : trace le chemin d'un paquet IP entrant depuis l'interface réseau jusqu'à la couche transport. Fonctions noyau traversées ?
2. **Checksum IP** : explique l'algorithme du checksum IP (complément à 1 de la somme des mots 16 bits). Implémente-le de mémoire en C.
3. **Routage** : comment le noyau FreeBSD décide-t-il par quelle interface envoyer un paquet IP ? Quelle structure consulte-t-il ?

---

## Chapitre 14 — Couche transport

1. **Machine d'état TCP** : dessine de mémoire les états TCP (CLOSED, LISTEN, SYN_SENT, SYN_RECEIVED, ESTABLISHED, FIN_WAIT_1, FIN_WAIT_2, TIME_WAIT, CLOSE_WAIT, LAST_ACK). Place les transitions avec les segments échangés.
2. **Handshake 3 voies** : dessine la séquence SYN / SYN-ACK / ACK avec les numéros de séquence. Quel état côté serveur entre listen() et accept() ?
3. **Contrôle de flux TCP** : explique la différence entre la fenêtre de congestion (`cwnd`) et la fenêtre d'annonce de réception (`rwnd`). Lequel limite le débit en pratique ?
4. **Stop-and-wait vs fenêtre glissante** : explique pourquoi stop-and-wait est inefficace sur des liens à haute latence. Montre le calcul du débit théorique maximal avec une fenêtre de W segments.
5. **UDP vs TCP** : liste les 5 fonctionnalités que TCP ajoute par rapport à UDP, et cite un cas où UDP est préférable.

---

## Chapitre 9 — FFS

1. **Superbloc** : qu'est-ce que le superbloc d'un FFS ? Quelles informations contient-il ? Où est-il situé sur disque ?
2. **Inode** : dessine la structure d'un inode UFS : champs directs, indirect, double-indirect. Comment adresse-t-on un fichier de 1 Go ?
3. **Soft updates** : explique en 3 phrases pourquoi FFS sans soft updates peut laisser le FS dans un état incohérent après un crash, et comment soft updates résout ce problème.
4. **Groupe de cylindres** : pourquoi FFS groupe-t-il les inodes et blocs par cylindres ? Quel problème de performance cela résout-il ?
