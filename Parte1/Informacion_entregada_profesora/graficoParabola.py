import matplotlib.pyplot as plt
import numpy as np

# Leer datos del archivo generado por el programa C++
data = np.loadtxt("trayectoria_particula.txt")

# Separar columnas x e y
x = data[:, 0]
y = data[:, 1]

# Crear el gráfico
plt.figure(figsize=(8, 5))
plt.plot(x, y, 'b-', linewidth=2)
plt.title("Trayectoria de la partícula")
plt.xlabel("Posición X")
plt.ylabel("Posición Y")
plt.grid(True)
plt.gca().invert_yaxis()  # Invertir eje Y si Y crece hacia abajo (como en tu código)
plt.show()
