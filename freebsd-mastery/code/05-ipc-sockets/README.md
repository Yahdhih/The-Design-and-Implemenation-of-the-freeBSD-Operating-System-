# code/05-ipc-sockets/ — IPC, Sockets, kqueue, HTTP — PRIORITÉ MAXIMALE

Correspond à **Ch. 12 — Communication inter-processus (IPC)**.
C'est le dossier le plus important du cours — il recoupe directement ton étude des RFC réseau.

---

## Projets

### Jalon 1 — Serveur/client TCP echo (`echo_server.c` + `echo_client.c`)

**Objectif** : maîtriser l'API sockets BSD de bout en bout.

```c
// echo_server.c : socket → bind → listen → accept (bloquant) → recv → send → close
// echo_client.c : socket → connect → send → recv → close
```

```sh
make echo_server echo_client
./echo_server 9000 &
./echo_client 127.0.0.1 9000 "bonjour"
# Attendu : "bonjour" renvoyé par le serveur
nc 127.0.0.1 9000  # test manuel
```

**Critère** : nc renvoie exactement ce qu'on lui envoie.

---

### Jalon 2 — Boucle d'événements : select/poll (`event_loop.c`)

**Objectif** : gérer plusieurs connexions simultanées sans threads.

```c
// Serveur multi-clients avec select() d'abord, puis poll()
// Comparer la lisibilité et les limitations (FD_SETSIZE pour select)
```

**Critère** : 3 clients simultanés reçoivent chacun leur écho sans bloquer les autres.

---

### Jalon 3 — Boucle d'événements : kqueue/kevent (`event_kqueue.c`)

**Objectif** : la version scalable, propre FreeBSD/macOS.

```c
// kq = kqueue();
// EV_SET(&kev, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
// kevent(kq, &kev, 1, NULL, 0, NULL);   // enregistrer l'intérêt
// n = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);  // attendre
```

```sh
make event_kqueue
./event_kqueue 9001 &
# Connecter 10 clients simultanés avec un script :
for i in $(seq 1 10); do nc 127.0.0.1 9001 <<< "client $i" & done
```

**Critère** : 10 connexions simultanées gérées correctement.

---

### Jalon 4 — Benchmark comparatif (`bench_io.c`)

**Objectif** : mesurer et comprendre pourquoi kqueue surpasse select/poll à grande échelle.

```sh
make bench_io
./bench_io --method select --connections 100
./bench_io --method poll   --connections 100
./bench_io --method kqueue --connections 100
# Comparer les temps — noter les résultats dans reference/notes/
```

---

### Jalon 5 — Mbuf jouet (`mbuf_toy.c`)

**Objectif** : réimplémenter la structure mbuf pour comprendre pourquoi les vraies piles réseau l'utilisent.

```c
// struct mbuf_toy { char data[128]; size_t len; size_t offset; struct mbuf_toy *m_next; };
// Implémenter : mbuf_alloc, mbuf_free, mbuf_prepend (ajout d'en-tête sans copie), mbuf_chain
```

**Critère** : simuler l'encapsulation TCP/IP — ajouter en-tête IP, puis TCP, puis données, sans copier.

---

### Jalon 6 — Serveur HTTP/1.1 minimal (`http_server.c`)

**Objectif** : le projet qui recoupe directement ton étude des RFC 2616/9110.

Fonctionnalités à implémenter :
- [ ] Parsing de la Request-Line (méthode, URI, version)
- [ ] Parsing des headers (Host, Content-Length, Connection)
- [ ] Réponse GET avec fichier statique (lire depuis le disque)
- [ ] Réponse 404 pour les URIs non trouvés
- [ ] Support keep-alive (Connection: keep-alive)
- [ ] Concurrence via kqueue (pas de threads)

```sh
make http_server
./http_server 8080 ./www/ &
curl -v http://127.0.0.1:8080/index.html
curl -v http://127.0.0.1:8080/notfound
```

**Critère** : curl reçoit une réponse HTTP/1.1 valide avec les bons headers et le bon corps.

**Croisement RFC** :
- RFC 9110 §8.3 : Content-Length — comment le calculer et l'envoyer correctement ?
- RFC 9110 §9.3.1 : GET — quelles contraintes sur le corps de la réponse ?
- RFC 9112 §9.3 : keep-alive — comment détecter la fin d'une requête ?

---

## Sources FreeBSD clés

| Fichier | Ce qu'on cherche |
|---------|-----------------|
| `sys/kern/uipc_socket.c` | `socreate()`, `sobind()`, `solisten()`, `soaccept()`, `soreceive()`, `sosend()` |
| `sys/kern/uipc_usrreq.c` | Implémentation des PRU pour AF_UNIX |
| `sys/kern/uipc_mbuf.c`   | `m_get()`, `m_gethdr()`, `m_prepend()`, `m_cat()`, `m_freem()` |
| `sys/sys/socketvar.h`    | `struct socket`, `struct sockbuf` |
| `sys/sys/mbuf.h`         | `struct mbuf`, macros d'accès |
| `sys/kern/kern_event.c`  | Implémentation de kqueue/kevent |

**Questions de trace prioritaires** :
1. Dans `uipc_socket.c`, comment `soreceive()` déplace-t-il les données du sockbuf vers l'espace utilisateur ? Y a-t-il une copie ?
2. Dans `uipc_mbuf.c`, comment `m_prepend()` ajoute-t-il un en-tête sans copier les données ? Que se passe-t-il si le mbuf de tête n'a plus de place ?

---

## Schémas OBLIGATOIRES pour ce module

1. **Chaîne de mbufs** : paquet TCP avec en-têtes IP + TCP + données répartis sur 3 mbufs.
2. **struct socket** : champs so_rcv et so_snd (sockbuf), so_proto, so_state.
3. **Chemin d'un send()** : espace utilisateur → syscall → sosend() → protocole → mbuf → interface.
4. **Architecture kqueue** : kqueue fd, kevent, EVFILT_READ/WRITE, relation avec les fd surveillés.
