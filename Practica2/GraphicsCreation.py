'''
@auhor: Pablo Seijo and Pablo Lobato, helped by chatGPT-3
@date: 18/3/2023
@company: USC ETSE
'''

import matplotlib.pyplot as plt
import os

# Le paso los valores de N, que lo utilizaremos como el tamaño de matriz de las matrices
N_parameters = [250, 500, 750, 1000, 1500, 2000, 2550, 3000]

# Lee los valores desde el archivo de texto
with open('grafica.txt', 'r') as f:
    values = [float(x) for x in f.readlines()]

# crear un array de arrays dividido en los grupos de 10 necesarios para la realizacion de
# las graficas, ergo groups[0] = {list: 10} [...], y asi hasta los 8 grupos necesarios, para los diferentes valores de N
groups = [values[i:i + 10] for i in range(0, len(values), 10)]

# Crea una gráfica con múltiples líneas para las primeras 4 N
fig1, ax1 = plt.subplots()

#Ajustamos el tamaño de la figura
fig1.set_size_inches(12, 8)

# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groups[:4]):
    ax1.plot(range(1, 11), group, marker = 'o', markersize = 5, label = f'N = {N_parameters[i]}')

# Título de la gráfica
ax1.set_title('Tics de reloj para las primeras 4 N')

# Nombre de los ejes x e y
ax1.set_xlabel('Número de ejecución')
ax1.set_ylabel('Tics de reloj')

# Añadimos la leyenda
ax1.legend(loc = 'best')

# Guardamos la gráfica en un archivo png
plt.savefig('grafica1.png')

# Crea una gráfica con múltiples líneas para las últimas 4 N
fig2, ax2 = plt.subplots()

#Ajustamos el tamaño de la figura
fig2.set_size_inches(12, 8)

colors = ['b', 'g', 'r', 'm']  # colores diferentes para cada línea

# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groups[4:]):
    ax2.plot(range(1, 11), group, color=colors[i], marker='o', markersize=5, label=f'N = {N_parameters[i+4]}')

# Título de la gráfica
ax2.set_title('Tics de reloj para las últimas 4 N')

# Nombre de los ejes x e y
ax2.set_xlabel('Número de ejecución')
ax2.set_ylabel('Tics de reloj')

# Añadimos la leyenda
ax2.legend(loc = 'best')

# Guardamos la gráfica en un archivo png
plt.savefig('grafica2.png')

print("Por favor, compruebe que se han generado los archivos grafica1.png y grafica2.png en la siguiente lista: ", os.listdir())
