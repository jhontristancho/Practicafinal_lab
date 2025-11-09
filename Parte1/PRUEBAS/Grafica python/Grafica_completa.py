# graficar_profesional_mejorado.py
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import re
from scipy.interpolate import make_interp_spline

# ------------------ Config ------------------
CAJA_ANCHO = 500
CAJA_ALTO = 350

obstaculos = [
    (200, 120, 50),
    (320, 100, 50),
    (260, 200, 40),
    (150, 150, 40)
]

colors = ["red", "blue", "green", "purple"]
fusion_color = "gold"

# ------------------ Parsers ------------------
regex_xy = re.compile(r"\(\s*([\-0-9\.]+)\s*,\s*([\-0-9\.]+)\s*\)")
regex_t = re.compile(r"Tiempo\s+([0-9\.]+)")
regex_fusion = re.compile(r"Fusion\s+particulas\s*([0-9]+)\s*y\s*([0-9]+)", re.IGNORECASE)

# Estructuras para almacenar datos
# particulas[id] = list of (x,y,t)
particulas = {0: [], 1: [], 2: [], 3: []}
collision_points = []   # list of (x,y,t)
fusion_events = []      # list of (t, A, B)

with open("salida.txt", "r") as f:
    lineas = f.readlines()

id_local = 0
tiempo_actual = 0.0
pending_collision = False

for linea in lineas:
    s = linea.strip()

    # Detectar tiempo
    m_t = regex_t.search(s)
    if m_t:
        tiempo_actual = float(m_t.group(1))
        id_local = 0
        pending_collision = False
        continue

    # Detectar fusión
    m_f = regex_fusion.search(s)
    if m_f:
        a = int(m_f.group(1))
        b = int(m_f.group(2))
        fusion_events.append((tiempo_actual, a, b))
        pending_collision = False
        continue

    # Detectar colisión/choque (texto libre)
    if ("Colision" in s) or ("Choque" in s) or ("Colisión" in s) or ("Choque con") in s:
        # marcamos que la próxima coordenada encontrada corresponde a un choque
        pending_collision = True
        continue

    # Detectar coordenadas
    m_xy = regex_xy.search(s)
    if m_xy:
        x = float(m_xy.group(1))
        y = float(m_xy.group(2))

        # Guardar coordenada asociada a id_local
        if id_local in particulas:
            particulas[id_local].append((x, y, tiempo_actual))
        else:
            particulas[id_local] = [(x, y, tiempo_actual)]

        # Si antes había una línea de colisión, asociamos ese punto como choque
        if pending_collision:
            collision_points.append((x, y, tiempo_actual))
            pending_collision = False

        id_local += 1
        continue

    # cualquier otra línea la ignoramos
    pending_collision = False

# ------------------ Interpretar evento de fusión (si hay) ------------------
# Suponemos (como en tu C++) que la fusión "A y B" deja a A como la sobreviviente y B desaparece.
fusion_info = None
if fusion_events:
    # tomamos la primera fusión (si hay varias, podríamos manejar todas)
    tf, A_idx, B_idx = fusion_events[0]
    fusion_info = (tf, A_idx, B_idx)

# ------------------ Graficar ------------------
plt.figure(figsize=(15, 8))
ax = plt.gca()
plt.title("Simulación: Trayectorias + Colisiones + Fusión (mejorado)", fontsize=16)

# Caja
box = patches.Rectangle((0, 0), CAJA_ANCHO, CAJA_ALTO, linewidth=3, edgecolor="black", facecolor="none")
ax.add_patch(box)

# Obstáculos
for (ox, oy, lado) in obstaculos:
    ax.add_patch(patches.Rectangle((ox, oy), lado, lado, linewidth=1.5,
                                   edgecolor="orange", facecolor="orange", alpha=0.35))

# Dibujar partículas por ID, pero separando tramo previo/posterior a la fusión
for pid in sorted(particulas.keys()):
    datos = particulas[pid]
    if not datos:
        continue

    xs = np.array([d[0] for d in datos])
    ys = np.array([d[1] for d in datos])
    ts = np.array([d[2] for d in datos])

    # Si hay fusión que involucra este pid, calculemos el índice de corte
    if fusion_info is not None:
        tf, A_idx, B_idx = fusion_info
        if pid == A_idx:
            # A_idx survives: plot before fusion with normal color, after fusion in fusion_color
            mask_before = ts <= tf + 1e-9
            mask_after = ts > tf + 1e-9

            if mask_before.sum() > 0:
                xb = xs[mask_before]; yb = ys[mask_before]
                plt.plot(xb, yb, color=colors[pid % len(colors)], linewidth=2.2)
                plt.scatter(xb, yb, s=12, color=colors[pid % len(colors)], alpha=0.9)

            if mask_after.sum() > 0:
                xa = xs[mask_after]; ya = ys[mask_after]
                # suavizar solo el segmento posterior si hay suficientes puntos
                try:
                    if len(xa) > 3:
                        Xs = np.linspace(xa.min(), xa.max(), 200)
                        Ys = make_interp_spline(xa, ya, k=3)(Xs)
                        plt.plot(Xs, Ys, color=fusion_color, linewidth=2.6, label=f"Partícula {pid} (fusionada)")
                    else:
                        plt.plot(xa, ya, color=fusion_color, linewidth=2.6, label=f"Partícula {pid} (fusionada)")
                    plt.scatter(xa, ya, s=14, color=fusion_color, edgecolors="black")
                except Exception:
                    plt.plot(xa, ya, color=fusion_color, linewidth=2.6, label=f"Partícula {pid} (fusionada)")
                    plt.scatter(xa, ya, s=14, color=fusion_color, edgecolors="black")
        elif pid == B_idx:
            # B_idx disappears: plot only up to fusion time
            mask_before = ts <= tf + 1e-9
            if mask_before.sum() > 0:
                xb = xs[mask_before]; yb = ys[mask_before]
                plt.plot(xb, yb, color=colors[pid % len(colors)], linewidth=2.2, label=f"Partícula {pid} (desaparece)")
                plt.scatter(xb, yb, s=10, color=colors[pid % len(colors)])
        else:
            # pid no involucrada en la fusión: plot normal
            try:
                if len(xs) > 3:
                    Xs = np.linspace(xs.min(), xs.max(), 300)
                    Ys = make_interp_spline(xs, ys, k=3)(Xs)
                    plt.plot(Xs, Ys, color=colors[pid % len(colors)], linewidth=2.2, label=f"Partícula {pid}")
                else:
                    plt.plot(xs, ys, color=colors[pid % len(colors)], linewidth=2.2, label=f"Partícula {pid}")
                plt.scatter(xs, ys, s=10, color=colors[pid % len(colors)])
            except Exception:
                plt.plot(xs, ys, color=colors[pid % len(colors)], linewidth=2.2, label=f"Partícula {pid}")
                plt.scatter(xs, ys, s=10, color=colors[pid % len(colors)])
    else:
        # Sin fusión en el archivo → dibujar normal
        try:
            if len(xs) > 3:
                Xs = np.linspace(xs.min(), xs.max(), 300)
                Ys = make_interp_spline(xs, ys, k=3)(Xs)
                plt.plot(Xs, Ys, color=colors[pid % len(colors)], linewidth=2.2, label=f"Partícula {pid}")
            else:
                plt.plot(xs, ys, color=colors[pid % len(colors)], linewidth=2.2, label=f"Partícula {pid}")
            plt.scatter(xs, ys, s=10, color=colors[pid % len(colors)])
        except Exception:
            plt.plot(xs, ys, color=colors[pid % len(colors)], linewidth=2.2, label=f"Partícula {pid}")
            plt.scatter(xs, ys, s=10, color=colors[pid % len(colors)])

# Dibujar puntos de colisión (ya asociados a la coordenada correcta)
if collision_points:
    cx = [p[0] for p in collision_points]
    cy = [p[1] for p in collision_points]
    ct = [p[2] for p in collision_points]
    plt.scatter(cx, cy, color="red", s=60, zorder=6, label="Colisiones")

# Dibujar marcador de fusión en coordenada real (si existe)
if fusion_info is not None:
    tf, A_idx, B_idx = fusion_info
    # buscar la coordenada de A en el instante tf (preferente) o la más cercana posterior
    coordsA = particulas.get(A_idx, [])
    fusion_xy = None
    for (x, y, t) in coordsA:
        if abs(t - tf) < 1e-6:
            fusion_xy = (x, y)
            break
    if fusion_xy is None and coordsA:
        # buscar el punto más cercano en tiempo
        times = np.array([p[2] for p in coordsA])
        idx = np.argmin(np.abs(times - tf))
        fusion_xy = (coordsA[idx][0], coordsA[idx][1])
    if fusion_xy:
        plt.scatter([fusion_xy[0]], [fusion_xy[1]], marker="*", s=300, color="gold", edgecolors="black", zorder=8, label="Fusión")

plt.gca().invert_yaxis()
plt.xlim(-10, CAJA_ANCHO + 10)
plt.ylim(-10, CAJA_ALTO + 10)
plt.xlabel("X")
plt.ylabel("Y")
plt.grid(True, linestyle="--", alpha=0.35)
plt.legend(fontsize=10)
plt.tight_layout()
plt.show()
