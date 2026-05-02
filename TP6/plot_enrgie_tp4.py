#!/usr/bin/env python3
import re
import matplotlib.pyplot as plt

log_file = "log_energy_2.txt"

frames = []
em_values = []

current_frame = None

frame_pattern = re.compile(r"Frame\s+(\d+)/\d+")
em_pattern = re.compile(r"Em\s*=\s*([+-]?\d+(?:\.\d+)?(?:e[+-]?\d+)?)", re.IGNORECASE)

with open(log_file, "r") as f:
    for line in f:
        m_frame = frame_pattern.search(line)
        if m_frame:
            current_frame = int(m_frame.group(1))
            continue

        m_em = em_pattern.search(line)
        if m_em and current_frame is not None:
            frames.append(current_frame)
            em_values.append(float(m_em.group(1)))
            current_frame = None

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