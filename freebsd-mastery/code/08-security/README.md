# code/08-security/ — Capsicum, jails, MAC

Correspond à **Ch. 5 — Sécurité**.
**Nécessite la VM FreeBSD** — Capsicum et jails sont FreeBSD-spécifiques.

---

## Projets

### Jalon 1 — Bac à sable Capsicum (`capsicum_demo.c`)

**Objectif** : comprendre le modèle à capacités en le pratiquant.

```c
#include <sys/capsicum.h>

// Séquence typique :
// 1. Ouvre toutes les ressources nécessaires (fichiers, sockets) AVANT cap_enter()
// 2. cap_enter() : entre en mode capacité → plus de nouvelles ressources globales
// 3. cap_rights_limit(fd, ...) : restreint les droits sur chaque fd
// 4. Travaille normalement avec les fds ouverts

// Exemple : serveur qui lit un répertoire www/ et répond sur un socket
// → ouvre le socket et le répertoire avant cap_enter()
// → après cap_enter() : ne peut plus ouvrir de nouveaux fichiers
```

```sh
make capsicum_demo
./capsicum_demo
# Doit servir des fichiers depuis www/ en mode Capsicum
# Tenter d'ouvrir un fichier hors www/ depuis le handler → ECAPMODE
```

**Critère** : le programme fonctionne en mode Capsicum et refuse correctement les accès non autorisés.

---

### Jalon 2 — Expérience jail (`jail_demo.sh`)

**Objectif** : monter un jail minimal et observer l'isolation.

```sh
#!/bin/sh
# Créer un jail minimal :
mkdir -p /jails/test
bsdtar -xf /usr/obj/usr/src/amd64.amd64/release/base.txz -C /jails/test

# Monter et entrer dans le jail :
jail -c path=/jails/test host.hostname=testjail command=/bin/sh

# Dans le jail :
# - ps ne voit que les processus du jail
# - ifconfig ne voit pas les interfaces de l'hôte
# - /proc est vide ou absent
# - kill -9 d'un PID hôte → ESRCH (not found)
```

**Questions à explorer** :
1. Peut-on faire un `kldload` depuis l'intérieur d'un jail ?
2. Comment un jail peut-il avoir sa propre adresse IP ?

---

### Jalon 3 — Observation MAC (optionnel)

**Objectif** : charger un module MAC et observer comment il filtre les accès.

```sh
# Charger le module Biba (intégrité obligatoire) :
kldload mac_biba
# Configurer une règle simple et observer le refus d'accès
```

---

## Sources FreeBSD clés

| Fichier | Ce qu'on cherche |
|---------|-----------------|
| `sys/kern/sys_capability.c` | `cap_enter()`, `cap_rights_limit()`, vérification des droits |
| `sys/sys/capsicum.h` | Droits Capsicum (`CAP_READ`, `CAP_WRITE`, `CAP_CONNECT`…) |
| `sys/kern/kern_jail.c` | Création et gestion des jails |
| `sys/security/mac/mac_framework.c` | Point d'entrée du framework MAC |
| `sys/security/mac/mac_policy.h` | Interface des modules de politique MAC |

**Questions de trace** :
1. Dans `sys_capability.c`, comment le noyau vérifie-t-il qu'un fd a le droit `CAP_READ` avant d'autoriser `read()` ?
2. Dans `kern_jail.c`, quelles ressources sont partagées entre le jail et l'hôte, et lesquelles sont isolées ?
