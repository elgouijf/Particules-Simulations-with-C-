import numpy as np
import matplotlib.pyplot as plt
import os


def load_data(filename="lj.txt"):
    """
    Charge les données du potentiel de Lennard-Jones depuis un fichier.

    Parameters
    ----------
    filename : str
        Nom du fichier contenant les données (r, U(r)).

    Returns
    -------
    tuple of np.ndarray
        r : distances
        U : potentiel associé

    Raises
    ------
    FileNotFoundError
        Si le fichier n'existe pas.
    """
    if not os.path.exists(filename):
        raise FileNotFoundError(f"Fichier {filename} introuvable")

    data = np.loadtxt(filename)
    return data[:, 0], data[:, 1]


def plot_lj(r, U):
    """
    Trace le potentiel de Lennard-Jones.

    Parameters
    ----------
    r : np.ndarray
        Distances
    U : np.ndarray
        Valeurs du potentiel
    """
    plt.figure()
    plt.plot(r, U, label="Lennard-Jones")

    # Ligne horizontale U=0 (référence)
    plt.axhline(0, linestyle='--')

    plt.xlabel("r")
    plt.ylabel("U(r)")
    plt.title("Potentiel de Lennard-Jones")
    plt.legend()
    plt.grid()

    plt.show()


def main():
    """
    Fonction principale :
    - charge les données
    - trace le potentiel
    """
    try:
        r, U = load_data()
        plot_lj(r, U)
    except Exception as e:
        print(f"Erreur: {e}")


if __name__ == "__main__":
    main()