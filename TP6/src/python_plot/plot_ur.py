import numpy as np
import matplotlib.pyplot as plt
import os
import sys


def load_data(filename):
    if not os.path.exists(filename):
        raise FileNotFoundError(f"Fichier {filename} introuvable")

    data = np.loadtxt(filename)

    return data[:, 0], data[:, 1]


def plot_lj(r, U, output_file):
    plt.figure(figsize=(8, 5))

    plt.plot(r, U, label="Lennard-Jones")
    plt.axhline(0, linestyle="--")

    plt.xlabel("r")
    plt.ylabel("U(r)")
    plt.title("Potentiel de Lennard-Jones")

    plt.legend()
    plt.grid()
    plt.tight_layout()

    os.makedirs(os.path.dirname(output_file), exist_ok=True)

    plt.savefig(output_file, dpi=300)
    plt.close()

    print(f"Figure sauvegardee dans : {output_file}")


def main():
    if len(sys.argv) != 2:
        print("Usage : python3 src/python_plot/plot_ur.py potentials/lj.txt")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = "plots/potentiel_lennard_jones.png"

    try:
        r, U = load_data(input_file)
        plot_lj(r, U, output_file)

    except Exception as e:
        print(f"Erreur : {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()