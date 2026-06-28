# code/06-network-layer/ — Couche réseau IP, checksum, dissection de paquets

Correspond à **Ch. 13 — Couche réseau**.
Recoupe ton étude des protocoles réseau (IP, ARP, ICMP).

---

## Projets

### Jalon 1 — Parser d'en-têtes IP/TCP (`ip_parser.c`)

**Objectif** : lire et décoder des paquets bruts pour ancrer les structures de données réseau.

```c
// Fonctions à implémenter :
// parse_eth_header(buf, len, struct eth_hdr *)
// parse_ip_header(buf, len, struct ip_hdr *)
// parse_tcp_header(buf, len, struct tcp_hdr *)
// print_packet_summary(...)
```

Source de paquets : fichier `.pcap` capturé avec `tcpdump -w`.

```sh
# Capturer des paquets :
tcpdump -i lo0 -w /tmp/capture.pcap -c 20 port 9000

# Parser avec notre outil :
make ip_parser
./ip_parser /tmp/capture.pcap
# Attendu : liste des paquets avec src/dst IP, ports, flags TCP
```

**Critère** : affiche correctement src IP, dst IP, src port, dst port, et les flags TCP (SYN, ACK, FIN) pour chaque paquet.

---

### Jalon 2 — Checksum IP et TCP (`checksum.c`)

**Objectif** : implémenter l'algorithme du checksum (RFC 791 §A.4, RFC 793 §3.1).

```c
// Algorithme : somme des mots de 16 bits en complément à 1, puis complément à 1 du résultat
uint16_t ip_checksum(const void *data, size_t len);
uint16_t tcp_checksum(const struct ip_hdr *ip, const void *tcp_seg, size_t tcp_len);
// (TCP checksum utilise un pseudo-header IP)
```

```sh
make checksum
./checksum
# Doit vérifier le checksum de paquets connus (valeur attendue fournie dans le code)
```

**Croisement RFC** : RFC 791 §3.1 (champ checksum IP), RFC 793 §3.1 (checksum TCP, pseudo-header).

**Critère** : vérifie correctement des paquets valides et détecte les paquets corrompus (flip d'un bit).

---

### Jalon 3 — Dissection avec tcpdump/tshark

**Objectif** : lire et interpréter des captures réseau pour corréler avec le code noyau.

```sh
# Capturer un handshake TCP complet :
tcpdump -i lo0 -n -S port 9000 &
./echo_client 127.0.0.1 9000 "hello"
# Lire la capture :
tshark -r /tmp/capture.pcap -T fields -e frame.number -e ip.src -e ip.dst \
       -e tcp.srcport -e tcp.dstport -e tcp.flags -e tcp.seq -e tcp.ack
```

Questions à répondre depuis la capture :
1. Quels sont les ISN (Initial Sequence Numbers) choisis par chaque côté ?
2. Combien de segments échange `./echo_client` pour envoyer "hello" et recevoir l'écho ?

---

## Sources FreeBSD clés

| Fichier | Ce qu'on cherche |
|---------|-----------------|
| `sys/netinet/ip_input.c` | `ip_input()` : réception d'un paquet IP, démultiplexage vers transport |
| `sys/netinet/ip_output.c` | `ip_output()` : construction et émission d'un paquet IP |
| `sys/netinet/in_cksum.c` | Implémentation du checksum en C optimisé |
| `sys/net/if.h` | `struct ifnet` : représentation d'une interface réseau |
| `sys/net/route.c` | `rtalloc1()` : lookup dans la table de routage |

**Questions de trace** :
1. Dans `ip_input.c`, comment le noyau décide-t-il si un paquet est destiné à la machine locale ou doit être routé ?
2. Dans `ip_output.c`, à quel moment le checksum IP est-il calculé et écrit dans l'en-tête ?
