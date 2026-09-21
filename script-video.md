# Script narration — vidéo solveur de solitaire (~10 min)

> Voix off pédagogique, public mixte (accessible mais on garde les vrais termes).
> Débit visé ~145 mots/min. Les `[…]` sont des indications de montage / pauses.
> **Pas de code affiché à l'écran** : on reste sur le terminal ncurses (recherche
> en cours) puis sur l'animation SVG (`anim.html`) qui rejoue la solution en boucle.
> Minutage indicatif — pas de rushs enregistrés pour l'instant, donc pas
> d'incises calées au timecode près comme pour 2048qt. À ajuster une fois le
> premier enregistrement fait.

---

## Plan de minutage global

| Chapitre | Fenêtre |
|:---------|:--------|
| 0. Accroche | 0:00 → 0:45 |
| 1. Le jeu : le solitaire français | 0:45 → 2:00 |
| 2. Pourquoi c'est difficile | 2:00 → 3:15 |
| 3. L'algorithme du solveur | 3:15 → 6:00 |
| 4. Du texte à l'animation | 6:00 → 7:45 |
| 5. Le détail qui change tout : le trou de départ | 7:45 → 9:00 |
| 6. Conclusion | 9:00 → 10:00 |

---

## 0. Accroche — 0:00 → 0:45

[Plan d'ouverture : le plateau vide en SVG, ou le terminal ncurses qui affiche
le plateau de départ.]

Vous connaissez peut-être ce jeu : un plateau percé de trous, des billes
partout sauf un trou vide, et une seule règle — sauter une bille par-dessus sa
voisine pour l'éliminer. Le but : finir avec une seule bille sur le plateau.

Ce que je vais vous montrer, ce n'est pas moi en train d'y jouer. C'est un
programme qui a exploré, en silence, toutes les façons possibles de gagner —
et qui en a gardé une.

[Laisser l'animation démarrer en fond, sans commentaire.]

---

## 1. Le jeu : le solitaire français — 0:45 → 2:00

Le plateau utilisé ici est le plateau **français**, aussi appelé européen.
C'est une grille de sept cases sur sept, à laquelle on retire les quatre coins
en escalier. Résultat : des lignes de 3, 5, 7, 7, 7, 5 et 3 cases — trente-sept
trous en tout.

[Montrer le plateau vide, éventuellement avec les coins retirés surlignés.]

Au départ, toutes les cases sont occupées par une bille, sauf une, laissée
vide. Un coup consiste à faire sauter une bille par-dessus une voisine — en
haut, en bas, à gauche ou à droite — pour atterrir dans un trou vide juste
derrière. La bille sautée est retirée du plateau.

On enchaîne les sauts. Et l'objectif est simple à énoncer, terriblement
difficile à réussir : ne garder qu'**une seule bille** à la fin.

---

## 2. Pourquoi c'est difficile — 2:00 → 3:15

[Le terminal ncurses affiche des plateaux qui défilent, plusieurs branches
explorées.]

Trente-sept trous, une bille en moins à chaque coup : il faut donc enchaîner
trente et un sauts, sans erreur, jusqu'au bout. Et à chaque étape, plusieurs
sauts sont possibles — le nombre de parties différentes explose très vite.

Un humain joue à l'instinct, essaie, recule, recommence. Le programme, lui,
n'a pas d'instinct. Il doit **tout explorer**, méthodiquement, jusqu'à trouver
une suite de coups qui va au bout.

Il y a un piège supplémentaire, et c'est un résultat mathématique connu :
sur ce plateau français à trente-sept trous, si on part avec le trou vide
**au centre**, il n'existe **aucune solution**. C'est démontrable, pas juste
une malchance. Le programme doit donc non seulement trouver une solution,
mais d'abord trouver **un point de départ pour lequel une solution existe**.

---

## 3. L'algorithme du solveur — 3:15 → 6:00

[Rester sur le terminal ncurses, qui tourne pendant tout ce chapitre.]

Voici comment le programme s'y prend, sans une ligne de code à l'écran — juste
l'idée.

**Première étape : explorer large.** Le solveur part de tous les plateaux
possibles avec une seule bille manquante — potentiellement les trente-sept
positions de départ. Depuis chacun, il essaie chaque coup possible, obtient de
nouveaux plateaux, réessaie depuis ceux-là, et ainsi de suite, couche après
couche, jusqu'à ce qu'un de ces plateaux ne contienne plus qu'une bille.

[Beat.]

**Deuxième étape, et c'est la plus importante : ne pas refaire deux fois le
même travail.** Beaucoup de plateaux qu'on obtient en cours de route sont en
fait **identiques**, une fois qu'on tourne ou qu'on retourne le plateau. Un
coup joué en haut à gauche, ou son équivalent en bas à droite par symétrie,
mènent à des situations qui se valent exactement. Le programme calcule donc,
pour chaque plateau, une sorte d'empreinte qui reste la même quelle que soit
la rotation ou la symétrie appliquée. Si cette empreinte a déjà été vue, il
jette le plateau au lieu de l'explorer une deuxième fois.

[Montrer, si possible, deux plateaux visuellement symétriques l'un de l'autre.]

Sans ça, le nombre de plateaux à explorer serait ingérable. Avec cette
déduplication, la recherche reste possible en un temps raisonnable — le
programme ne regarde jamais deux fois la même situation déguisée.

**Troisième étape : remonter la solution.** Une fois qu'un plateau à une seule
bille a été atteint, il suffit de remonter le chemin qui y a mené, coup après
coup, jusqu'au tout début. C'est cette suite de sauts — et le trou de départ
qui les rend possibles — que le programme écrit en sortie : la solution
complète, prête à être rejouée.

---

## 4. Du texte à l'animation — 6:00 → 7:45

[Transition : on quitte le terminal, on ouvre `anim.html` dans le navigateur.]

À ce stade, le solveur n'a produit qu'une liste de coups, en texte brut :
quel trou est vide au départ, puis pour chaque saut, quelle bille bouge et
dans quelle direction.

Un second outil prend le relais. Il relit cette liste et la transforme en une
séquence d'animations : pour chaque saut, la bille qui glisse jusqu'à sa
nouvelle case, puis la bille sautée qui disparaît. Cette séquence pilote le
plateau dessiné en SVG, directement dans le navigateur.

[Laisser l'animation tourner, en boucle, pendant une bonne partie du
chapitre — c'est le moment le plus satisfaisant à regarder.]

Résultat : une page web toute simple, sans base de données ni serveur, qui
rejoue indéfiniment la solution trouvée par le solveur — saut après saut,
jusqu'à la dernière bille.

---

## 5. Le détail qui change tout : le trou de départ — 7:45 → 9:00

[Revenir un instant sur le plateau au repos, trou de départ visible.]

Un dernier point, parce qu'il est facile à manquer en regardant juste
l'animation tourner : la partie ne commence *pas* n'importe où. Le trou vide
que vous voyez au tout début n'est pas un choix arbitraire ni le centre du
plateau — c'est précisément l'un des points de départ pour lesquels le
programme a réussi à prouver qu'une solution complète existe.

Changez ce trou de départ, et rien ne garantit qu'une solution existe encore.
C'est là toute la différence entre "un programme qui joue au solitaire" et
"un programme qui a d'abord dû comprendre, par la recherche, où il avait le
droit de commencer."

---

## 6. Conclusion — 9:00 → 10:00

[Plan large sur l'animation qui continue de tourner en boucle.]

Voilà ce que ça donne : un jeu vieux de plusieurs siècles, une règle unique
et minuscule, et pourtant un espace de possibilités assez vaste pour qu'il
faille une recherche méthodique — et un peu d'astuce sur les symétries —
pour le dompter. Pas d'intuition, pas d'essai-erreur : juste une exploration
complète, qui ne garde que ce qui marche.

Le code est disponible sur le dépôt si vous voulez creuser. Merci de l'avoir
regardé jouer.

[Respiration finale sur l'animation en boucle, puis fin.]

---

### Notes de minutage
- Total narration ≈ 950-1050 mots → un peu plus de 7 min de voix à 145 mots/min,
  le reste (≈2-3 min) en respirations, plans muets sur l'animation qui tourne,
  et transitions terminal → navigateur.
- Contrairement à 2048qt, pas de jalons à synchroniser au frame près : la
  partie dure quelques secondes une fois lancée, donc l'animation peut être
  mise en boucle librement pendant les chapitres 4 à 6 sans contrainte de
  timecode.
- Une fois un premier enregistrement fait (terminal + navigateur), recaler ce
  script dessus et, si besoin, ajouter des incises ★ comme dans le script
  2048qt.
