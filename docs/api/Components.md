# Composants ECS — Documentation et exemples

**Navigation**: [Accueil](../index.md) · [ECS header](Graphitech.ECS.ECS.md) · [ECS::Scene (fonctions)](Scene.md)

Les composants suivants sont documentés car utilisés par le renderer (`Rendering/SceneRenderer.cpp`). Ils servent d'exemples d'utilisation et de documentation publique pour l'API.

---

## `ECS::PositionComponent`

**Description**

Composant indiquant la position d'une entité dans l'espace de jeu.

**Champs**

- `float x`
- `float y`

**Exemple**

```cpp
auto *p = scene.getComponent<ECS::PositionComponent>(e);
if (p) {
    // déplacer l'entité
    p->x += 1.0f;
}
```

---

## `ECS::RenderLine`

**Description**

Composant décrivant une ligne à dessiner.

**Champs** (usage observé dans le renderer)

- `float x1, y1, x2, y2` — coordonnées des deux extrémités
- `float r, g, b` — couleur
- `float width` — largeur de la ligne

**Exemple**

```cpp
auto *ln = scene.getComponent<ECS::RenderLine>(e);
if (ln) {
    // dessin effectué par le renderer
}
```

---

## `ECS::RenderCircle`

**Description**

Composant décrivant un cercle (possiblement rempli) à dessiner.

**Champs**

- `float radius`
- `float r, g, b` — couleur
- `bool filled` — rempli si vrai

**Exemple**

```cpp
auto *rc = scene.getComponent<ECS::RenderCircle>(e);
if (rc) {
    // renderer utilise rc->radius, rc->filled, rc->r/g/b
}
```

---

## `ECS::RenderCross`

**Description**

Composant décrivant une petite croix centrée sur la position de l'entité.

**Champs**

- `float half` — demi-largeur du cross
- `float r, g, b` — couleur
- `float width` — largeur des lignes

**Exemple**

```cpp
auto *xr = scene.getComponent<ECS::RenderCross>(e);
if (xr) {
    // renderer dessine deux lignes diagonales de longueur 2*half
}
```

---

## `ECS::UIItem`

**Description**

Structure utilisée pour décrire un élément d'UI rendu par le renderer (ex : texte + petit carré visuel).

**Champs** (convention observée)

- `std::string text`
- `int x, y` — position (optionnelle)
- `float r, g, b` — couleur

**Exemple**

```cpp
auto *ui = scene.getMapValue<std::vector<ECS::UIItem>>(scene.getCurrentMap(), "ui");
if (ui) {
    for (auto &it : *ui) {
        // it.text, it.x, it.y, it.r, it.g, it.b
    }
}
```
