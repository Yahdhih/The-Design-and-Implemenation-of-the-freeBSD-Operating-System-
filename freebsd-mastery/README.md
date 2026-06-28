# freebsd-mastery

Environnement d'auto-apprentissage personnel du livre
**The Design and Implementation of the FreeBSD Operating System**, 2ᵉ éd.
McKusick / Neville-Neil / Watson

---

## Ce qu'est ce dépôt

Un **système d'étude quotidien** à 1 heure par jour. Il ne remplace pas le livre : il structure l'effort, force la pratique, et garantit que tu construis une vraie compréhension — pas seulement une familiarité de lecture.

Contient : un curriculum complet (`PLAN.md`), un suivi quotidien (`PROGRESS.md`), des fiches-jour minutées (`days/`), des projets C progressifs (`code/`), un paquet de cartes de rappel (`flashcards/`), des ressources méthodologiques (`setup/`), et un espace de notes personnelles (`reference/`).

---

## La méthode en 5 lignes

1. **Rappel actif d'abord** — ouvre le jour, fais les 5 min de récupération mémoire, *puis* lis ou code.
2. **Predict-then-verify** — écris 2 lignes de prédiction AVANT de lire chaque section, corrige après.
3. **Construire pour comprendre** — chaque sous-système a un projet C jouet. Si tu ne peux pas le réimplémenter ou le redessiner de mémoire, tu ne l'as pas compris.
4. **Espacer les révisions** — importe `flashcards/cards.tsv` dans Anki et fais tes cartes tous les jours avant de commencer.
5. **Revue hebdo** — le jour 7, 14, 21… est toujours une revue : pas de matière neuve, seulement consolidation.

---

## Mode d'emploi quotidien

```
1. Lance Anki → fais TOUTES les cartes dues (≤5 min)
2. Ouvre days/day-NNN.md du jour courant
3. Lis le « Rappel méthode » en haut
4. Exécute la session (rappel → tâche principale → consolidation)
5. Coche PROGRESS.md
6. Commit tes notes si tu veux un historique git
```

---

## Les 3 règles affichées en gros

> **① PREDICT-THEN-VERIFY**
> Avant de lire une section, écris en 2 lignes ce que tu crois déjà.
> Lis. Corrige tes erreurs par écrit. Cet écart *est* l'apprentissage.

> **② SI JE NE PEUX PAS LE RÉIMPLÉMENTER OU LE REDESSINER DE MÉMOIRE, JE NE L'AI PAS COMPRIS.**
> C'est la règle d'or. Applique-la à chaque concept, chaque structure de données, chaque chemin de code.

> **③ RÈGLE DES 2 JOURS**
> Ne jamais sauter 2 jours de suite.
> Si tu sautes un jour : **ne double pas pour rattraper** — reprends simplement là où tu étais.
> La continuité prime sur la quantité.

---

## Environnement recommandé

- **VM FreeBSD 14.x ou 15.x** (bhyve, QEMU ou VirtualBox) — pour que sources et appels système correspondent exactement au livre et aux outils (`truss`, `ktrace`, Capsicum, jails).
- `kqueue` fonctionne aussi sur macOS ; les fonctionnalités FreeBSD-spécifiques (Capsicum, jails, MAC) nécessitent la VM.
- Voir `setup/00-setup.md` pour les étapes complètes d'installation.

---

## Comment étendre le plan

Ce dépôt est généré par phases. Les jours 1-21 sont déjà présents en détail.
Pour générer la tranche suivante (jours 22-42), dis à Claude Code :

```
Génère days/day-022.md à days/day-042.md selon PLAN.md et PROGRESS.md,
en suivant le gabarit days/_TEMPLATE.md et la progression de la Phase 1.
```

---

## Commence par

```
setup/00-setup.md   →   puis   days/day-001.md
```
