# Projet Robot INF1900 – PB2 : Machine à états finis (FSM)

## Description
Cette machine à états implémente un cycle de couleurs de la DEL bicolore en fonction
des appuis et relâchements successifs sur le bouton poussoir.  
Le comportement est de type **Moore** : la sortie dépend uniquement de l’état courant.  

---

## Table des états (Moore)

| État     | Sortie (DEL) | Événement | État suivant |
|----------|--------------|-----------|--------------|
| **INIT** | Rouge (10)   | Press     | **E1** |
| **E1**   | Ambre (11)   | Release   | **E2** |
| **E2**   | Vert (01)    | Press     | **E3** |
| **E3**   | Rouge (10)   | Release   | **E4** |
| **E4**   | Off (00)     | Press     | **E5** |
| **E5**   | Vert (01)    | Release   | **INIT** |

---

## Notes
- Codage des sorties (DEL bicolore) :  
  - `10` → Rouge (LED rouge active, verte éteinte)  
  - `01` → Vert (LED verte active, rouge éteinte)  
  - `11` → Ambre (rouge + verte actives simultanément)  
  - `00` → Off (aucune LED active)  
- `Press` = front montant (passage de relâché → pressé).  
- `Release` = front descendant (passage de pressé → relâché).  
- Le cycle est circulaire : après **E5**, retour en **INIT**.

---
