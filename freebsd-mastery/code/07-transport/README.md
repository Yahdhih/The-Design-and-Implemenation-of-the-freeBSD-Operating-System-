# code/07-transport/ — Transport fiable, fenêtre glissante, machine d'état TCP

Correspond à **Ch. 14 — Couche transport**.
Croise directement les RFC 793/9293 (TCP) et 768 (UDP).

---

## Projets

### Jalon 1 — Stop-and-wait sur UDP (`rudp_sw.c`)

**Objectif** : comprendre pourquoi le protocole le plus simple possible est inefficace, en le vivant.

```c
// Protocole :
// - Émetteur : envoie un segment numéroté, attend ACK, timeout = 500ms, retransmet si timeout
// - Récepteur : reçoit, envoie ACK du numéro reçu
// - Si ACK manqué (simulé par drop aléatoire) → retransmission
```

```sh
make rudp_sw
./rudp_sw receiver 5000 &
./rudp_sw sender 127.0.0.1 5000 /etc/hosts --loss 10%
# Attendu : transfert du fichier avec messages de retransmission
# Mesurer le débit : comparer avec scp ou netcat sans perte
```

**Critère** : le fichier est transféré correctement même avec 10% de pertes simulées.

---

### Jalon 2 — Fenêtre glissante (`rudp_win.c`)

**Objectif** : implémenter Go-Back-N ou Selective Repeat pour comprendre le pipeline TCP.

```c
// Fenêtre W = 4 segments
// Émetteur : envoie jusqu'à W segments sans attendre les ACKs
// Récepteur : accuse réception cumulatif (Go-Back-N) ou sélectif
// Timer par segment non acquitté
```

**Critère** : débit au moins 3× supérieur à stop-and-wait sur un lien avec 50ms RTT simulé.

**Question clé** : quel est le débit maximal théorique avec W=4, 50ms RTT, 1500B segments ?
`débit_max = (W × taille_segment) / RTT = 4 × 1500 × 8 / 0.05 = 960 kbps`

---

### Jalon 3 — Machine d'état TCP (`tcp_fsm_toy.c`)

**Objectif** : modéliser la machine d'état TCP et simuler un handshake/teardown.

```c
// États : CLOSED, LISTEN, SYN_SENT, SYN_RECEIVED, ESTABLISHED,
//         FIN_WAIT_1, FIN_WAIT_2, TIME_WAIT, CLOSE_WAIT, LAST_ACK
// Événements : SYN_RECV, SYN_ACK_RECV, ACK_RECV, FIN_RECV, SEND_SYN, etc.
// Implémenter comme une table de transitions (état × événement → nouvel état + action)
```

```sh
make tcp_fsm_toy
./tcp_fsm_toy --simulate connect   # simule un handshake 3 voies
./tcp_fsm_toy --simulate close     # simule un teardown 4 voies
# Doit afficher chaque transition d'état avec le segment correspondant
```

**Croisement RFC** : RFC 793 §3.2 (diagramme d'états), RFC 9293 §3.3 (mise à jour).

**Critère** : les transitions CLOSED→SYN_SENT→ESTABLISHED et ESTABLISHED→FIN_WAIT_1→TIME_WAIT→CLOSED sont correctes.

---

## Sources FreeBSD clés

| Fichier | Ce qu'on cherche |
|---------|-----------------|
| `sys/netinet/tcp_fsm.h` | Constantes des états TCP (TCPS_CLOSED, TCPS_ESTABLISHED…) |
| `sys/netinet/tcp_var.h` | `struct tcpcb` : bloc de contrôle TCP par connexion |
| `sys/netinet/tcp_input.c` | `tcp_input()` : réception d'un segment, transitions d'état |
| `sys/netinet/tcp_output.c` | `tcp_output()` : construction et émission d'un segment |
| `sys/netinet/tcp_timer.c` | Timers TCP (RTO, TIME_WAIT, keepalive) |
| `sys/netinet/udp_usrreq.c` | Comparaison : la simplicité d'UDP vs la complexité TCP |

**Questions de trace prioritaires** :
1. Dans `tcp_input.c`, comment la machine d'état est-elle codée ? Cherche la structure de switch/case sur l'état courant.
2. Dans `tcp_var.h`, identifie les champs `snd_una`, `snd_nxt`, `snd_wnd`. Lequel correspond à la fenêtre de congestion ?

---

## Schéma OBLIGATOIRE

Machine d'état TCP complète avec toutes les transitions, les segments échangés, et les états TIME_WAIT / CLOSE_WAIT annotés (ce sont les plus souvent mal compris).
