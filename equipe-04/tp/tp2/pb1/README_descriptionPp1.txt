# Projet Robot INF1900 – PB1 : Machine à états finis (FSM)

## Description
Cette machine à états implémente la logique suivante :  
- Trois séquences **appui + relâchement** sur le bouton.  
- Après la 3ᵉ séquence, la DEL s’allume **verte pendant 2 secondes**.  
- Puis le système retourne à l’état initial `INIT`.  
- La logique est de type **Moore** (la sortie dépend uniquement de l’état courant).  

---

## Table des états (Moore)

| État       | Entrée attendue       | Prochain état | Sortie (LED) |
|------------|-----------------------|---------------|--------------|
| **INIT**   | P=0 (peu importe R)   | INIT          | 0 (éteinte)  |
|            | P=1                   | INTER1        | 0            |
| **INTER1** | R=0                   | INTER1        | 0            |
|            | R=1                   | E1            | 0            |
| **E1**     | P=0                   | E1            | 0            |
|            | P=1                   | INTER2        | 0            |
| **INTER2** | R=0                   | INTER2        | 0            |
|            | R=1                   | E2            | 0            |
| **E2**     | P=0                   | E2            | 0            |
|            | P=1                   | INTER3        | 0            |
| **INTER3** | R=0                   | INTER3        | 0            |
|            | R=1                   | E3            | 1 (vert)     |
| **E3**     | Timer < 2s            | E3            | 1 (vert)     |
|            | Timer = 2s            | INIT          | 0 (éteinte)  |

---

## Notes
- `P` = Pression (front montant détecté).  
- `R` = Relâchement (front descendant détecté).  
- `Sortie = 1` : DEL verte allumée.  
- Le **timer 2s** est lancé à l’entrée dans `E3`.  
- Pendant `E3`, les entrées bouton sont ignorées.  

---
