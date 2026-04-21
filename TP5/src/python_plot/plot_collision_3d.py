import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


def load_frames(filename="frames_3d/frames.txt"):
    """
    Charge les données de simulation 3D depuis un fichier texte.

    Le fichier doit contenir cinq colonnes :
    - frame : identifiant de la frame
    - x, y, z : coordonnées de la particule
    - type : type de particule

    Parameters
    ----------
    filename : str
        Chemin vers le fichier de données.

    Returns
    -------
    tuple
        df : pandas.DataFrame
            Tableau contenant toutes les particules de toutes les frames.
        frames : list
            Liste triée des identifiants de frame.
    """
    df = pd.read_csv(
        filename,
        sep=r"\s+",
        header=None,
        names=["frame", "x", "y", "z", "type"]
    )

    frames = sorted(df["frame"].unique())
    return df, frames


def create_animation(df, frames):
    """
    Crée une animation 3D de la simulation.

    Les particules de type 1 et 2 sont affichées avec deux nuages
    de points distincts afin de visualiser la collision ou l'évolution
    des deux ensembles de particules.

    Parameters
    ----------
    df : pandas.DataFrame
        Données complètes de simulation.
    frames : list
        Liste des identifiants de frame.

    Returns
    -------
    matplotlib.animation.FuncAnimation
        Objet animation créé.
    """
    fig = plt.figure(figsize=(8, 6))
    ax = fig.add_subplot(111, projection="3d")

    ax.set_xlim(df["x"].min(), df["x"].max())
    ax.set_ylim(df["y"].min(), df["y"].max())
    ax.set_zlim(df["z"].min(), df["z"].max())

    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")

    scat1 = ax.scatter([], [], [], s=8, label="Type 1")
    scat2 = ax.scatter([], [], [], s=8, label="Type 2")

    ax.legend()

    def update(frame_id):
        """
        Met à jour l'affichage pour une frame donnée.

        Parameters
        ----------
        frame_id : int
            Identifiant de la frame à afficher.
        """
        data = df[df["frame"] == frame_id]

        data1 = data[data["type"] == 1]
        data2 = data[data["type"] == 2]

        scat1._offsets3d = (data1["x"], data1["y"], data1["z"])
        scat2._offsets3d = (data2["x"], data2["y"], data2["z"])

        ax.set_title(f"Frame {frame_id}")
        return scat1, scat2

    ani = FuncAnimation(fig, update, frames=frames, interval=80, repeat=True)
    return ani


def main():
    """
    Point d'entrée principal du script.

    Ce script charge les frames d'une simulation 3D puis affiche
    une animation des particules.
    """
    df, frames = load_frames()
    ani = create_animation(df, frames)
    plt.show()
    return ani


if __name__ == "__main__":
    main()