# Jour NNN — Phase {n} · {thème du jour}

> ⏱ 60 min  |  🔁 5 min · {📖 Lecture | 💻 Code | 🔍 Trace} 40 min · 🧠 15 min
> 💡 Rappel méthode : {une astuce tournante parmi les 10 principes}

---

## 🔁 Réveil mémoire — 5 min

- **Page blanche** : {consigne de récupération sur le jour/chapitre précédent — ferme tes notes, écris de mémoire}.
- **Cartes dues** : ouvre Anki → fais toutes les cartes avec les tags `{ch0X}` dues aujourd'hui. Si tu n'as pas Anki, passe en revue les cartes `flashcards/cards.tsv` filtrées sur ces tags.

---

## {📖 Lecture | 💻 Code | 🔍 Trace} — 40 min

<!-- SECTION LECTURE -->
### Si c'est un jour de lecture :

- Lis **Chapitre X, §Y** (p. ___ → p. ___, voir `reference/toc.md`).
- **Predict-then-verify** : avant de lire, écris ici en 2 lignes ce que tu prédis :

  > *Prédiction :* ___

  Après lecture, note la correction :

  > *Correction :* ___

- Lis en annotant le **pourquoi** des décisions de conception (pas seulement le quoi).
- Si un schéma t'aiderait → dessine-le sur papier maintenant, pas après.

<!-- SECTION CODE -->
### Si c'est un jour de code :

- **Objectif** : {ce qu'on construit ou observe en une phrase}.
- **Fichier(s)** : `code/{sous-système}/fichier.c` (créer/éditer).
- **Build** :
  ```sh
  make -C code/{sous-système} {cible}
  ```
- **Comportement attendu** : {ce qu'on doit observer pour savoir que c'est bon}.
- **Ne pas avancer** tant que le critère n'est pas atteint.

<!-- SECTION TRACE -->
### Si c'est un jour de trace :

- **Point d'entrée** : `{fonction ou syscall}` dans `sys/kern/{fichier}.c`.
- **Méthode** :
  1. Lis d'abord la définition de `struct {X}` dans le `.h`.
  2. Descends la chaîne d'appels depuis le point d'entrée — note chaque fonction sur papier.
  3. Réponds aux 2 questions ci-dessous **avant** de regarder la réponse dans le code.
- **Questions ciblées** :
  1. {Question 1}
  2. {Question 2}

---

## 🧠 Consolidation — 15 min

- **Résumé** (3-5 puces, tes mots) → copie dans `reference/notes/jour-NNN.md` :
  - •
  - •
  - •

- **Nouvelles cartes** (2-3) → ajoute à `flashcards/cards.tsv` :
  - `{question}<TAB>{réponse}<TAB>ch0X`
  - *(exemple concret dans chaque réponse)*

- **Schéma** → esquisse dans `reference/diagrams/jour-NNN.txt` ou photo papier.

- **Question ouverte** (ce qui résiste ou te surprend) :
  > ___

---

## ✅ Terminé quand

- [ ] Rappel mémoire fait (page blanche 5 min + cartes Anki)
- [ ] {Critère principal : lecture faite / code compile et fonctionne / questions trace répondues}
- [ ] Résumé écrit dans `reference/notes/jour-NNN.md`
- [ ] 2-3 cartes ajoutées à `flashcards/cards.tsv`
- [ ] `PROGRESS.md` coché

## ↪ Demain

{Teaser en une ligne sur le contenu du jour suivant.}
