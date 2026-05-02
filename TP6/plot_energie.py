#!/usr/bin/env python3
import re
import matplotlib.pyplot as plt

log_file = "log_energy.txt"

frames = []
ec_values = []
em_values = []

pattern = re.compile(
    r"Frame\s+(\d+)/\d+.*?Ec\s*=\s*([+-]?\d+(?:\.\d+)?(?:e[+-]?\d+)?)"
    r".*?Em\s*=\s*([+-]?\d+(?:\.\d+)?(?:e[+-]?\d+)?)",
    re.IGNORECASE
)

with open(log_file, "r") as f:
    for line in f:
        m = pattern.search(line)
        if m:
            frames.append(int(m.group(1)))
            ec_values.append(float(m.group(2)))
            em_values.append(float(m.group(3)))

dt = 0.0005
times = [frame * dt for frame in frames]

plt.figure()
plt.plot(times, em_values)
plt.xlabel("Temps")
plt.ylabel("Énergie mécanique")
plt.title("Évolution de l'énergie mécanique")
plt.grid(True)
plt.tight_layout()
plt.savefig("energie_mecanique.png", dpi=200)
plt.show()