#!/usr/bin/env python3
import sys
import matplotlib.pyplot as plt
import matplotlib.animation as animation

### C'est un script pour visualiser avec matplotlib l'évolution du système

def main():
    if len(sys.argv) < 2:
        print("Il faut donner le fichier .txt en argument.")
        return

    fichier_txt = sys.argv[1]

    corps_data = {}
    temps = []

    with open(fichier_txt) as f:
        lignes = f.readlines()

    type_corps = None
    x = None

    for ligne in lignes:
        ligne = ligne.strip()
        if not ligne:
            continue

        morceaux = ligne.split()
        if len(morceaux) != 3:
            continue

        cle = morceaux[0]
        value = morceaux[2]

        if cle == "t":
            temps.append(float(value))

        elif cle == "type":
            type_corps = value
            if type_corps not in corps_data:
                corps_data[type_corps] = {"x": [], "y": []}

        elif cle == "x":
            x = float(value)

        elif cle == "y":
            y = float(value)
            if type_corps is not None and x is not None:
                corps_data[type_corps]["x"].append(x)
                corps_data[type_corps]["y"].append(y)

    if not corps_data:
        print("Aucune donnée de corps trouvée dans le fichier.")
        return

    fig, ax = plt.subplots(figsize=(8, 8))
    ax.set_facecolor("black")
    fig.patch.set_facecolor("black")

    noms = {"0": "soleil","1": "terre","2": "jupiter","3": "haley"}

    couleurs = {"0": "yellow","1": "deepskyblue","2": "orange","3": "white"}

    tailles = {"0": 120,"1": 40,"2": 80,"3": 15}

    trajectoires = {}
    points = {}

    for type_corps in corps_data:
        couleur = couleurs.get(type_corps, "white")
        label = noms.get(type_corps, f"type {type_corps}")
        taille = tailles.get(type_corps, 30)

        traj, = ax.plot([], [], color=couleur, linewidth=0.5, alpha=0.5)
        point, = ax.plot([], [], 'o', color=couleur,
                         markersize=taille**0.5, label=label)

        trajectoires[type_corps] = traj
        points[type_corps] = point

    titre = ax.set_title("t = 0.000", color="white")
    ax.legend(loc="upper right", facecolor="black", labelcolor="white")
    ax.tick_params(colors="white")

    for spine in ax.spines.values():
        spine.set_edgecolor("white")

    all_x = [x for d in corps_data.values() for x in d["x"]]
    all_y = [y for d in corps_data.values() for y in d["y"]]

    margin = 2
    ax.set_xlim(min(all_x) - margin, max(all_x) + margin)
    ax.set_ylim(min(all_y) - margin, max(all_y) + margin)

    n_frames = min(len(temps), min(len(d["x"]) for d in corps_data.values()))

    def update(frame):
        for type_corps in corps_data:
            xs = corps_data[type_corps]["x"][:frame+1]
            ys = corps_data[type_corps]["y"][:frame+1]

            trajectoires[type_corps].set_data(xs, ys)

            if xs:
                points[type_corps].set_data([xs[-1]], [ys[-1]])

        titre.set_text(f"t = {temps[frame]:.3f}")
        return list(trajectoires.values()) + list(points.values()) + [titre]

    ani = animation.FuncAnimation(
        fig, update, frames=n_frames,
        interval=20, blit=True, repeat=False
    )

    plt.tight_layout()
    plt.show()

main()