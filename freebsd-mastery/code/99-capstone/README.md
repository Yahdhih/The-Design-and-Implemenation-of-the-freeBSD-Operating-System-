# code/99-capstone/ — Projet intégrateur

**Phase 5 — Synthèse finale.**
Ce projet combine toutes les couches étudiées.

---

## Serveur HTTP concurrent, sandboxé, avec cache disque

```
┌─────────────────────────────────────────────────────────────────┐
│  Serveur HTTP/1.1 (kqueue)     ← Ch. 12, 13, 14               │
│    ↓                                                            │
│  Sandbox Capsicum (cap_enter)  ← Ch. 5                         │
│    ↓                                                            │
│  Cache disque maison (mini-FS) ← Ch. 9 + projet 04-filesystems │
│    ↓                                                            │
│  Fichiers statiques (VFS)      ← Ch. 7                         │
└─────────────────────────────────────────────────────────────────┘
```

---

## Fonctionnalités cibles

- [ ] Serveur HTTP/1.1 avec boucle kqueue (comme `05-ipc-sockets/http_server.c` mais amélioré)
- [ ] Support de plusieurs connexions simultanées sans threads
- [ ] Sandbox Capsicum : toutes les ressources ouvertes avant `cap_enter()`, puis isolation
- [ ] Cache disque : les réponses fréquentes sont sérialisées dans une image mini-FS
- [ ] Statistiques : nb de requêtes, hit/miss du cache, connexions actives
- [ ] Graceful shutdown sur SIGTERM (ferme les connexions proprement)

---

## Jalons

- [ ] **Jalon 1** : HTTP + kqueue fonctionnel (réutilise `05-ipc-sockets/`)
- [ ] **Jalon 2** : intégration mini-FS comme cache (réutilise `04-filesystems/`)
- [ ] **Jalon 3** : sandbox Capsicum (réutilise `08-security/`)
- [ ] **Jalon 4** : stats + graceful shutdown
- [ ] **Jalon 5** : test de charge (`wrk` ou `ab`) + profiling

```sh
make capstone
./capstone --port 8080 --root ./www/ --cache ./cache.img
# Dans un autre terminal :
wrk -t4 -c100 -d10s http://127.0.0.1:8080/index.html
```

**Critère final** : fonctionne sous Capsicum, sert des fichiers, cache les réponses, survit à 100 connexions simultanées.

---

## Ce que ce projet démontre

Si tu arrives ici et que tu peux le construire, tu as atteint le niveau « savoir s'en servir » :
- Tu comprends comment le noyau gère les sockets, les fichiers, et la mémoire.
- Tu sais instrumenter et raisonner sur les performances.
- Tu peux appliquer la sécurité par conception (Capsicum).
- Tu as lu et compris les sources FreeBSD correspondantes.
