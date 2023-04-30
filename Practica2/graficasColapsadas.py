'''
@auhor: Pablo Seijo
@date: 19/3/2023
@company: USC ETSE
@description: Este codigo coge valores de las medias geometricas de tes medciones de los ciclos por lineas de cache de tres
txt, y siendo D el tamaño de salto en doubles (cada uno es 8 bytes) y nos devuelve un png con una donde comparamos las graficas
cuando el experimento se hace con double, int y con acceso directo.
'''

import matplotlib.pyplot as plt
import os


# Le paso los valores de N, que lo utilizaremos como el tamaño de matriz de las matrices
N_parameters = [250, 500, 750, 1000, 1500, 2000, 2550, 3000]

# Lee los valores desde el archivo de texto de los valores del apartado 1
with open('datosAp1.txt', 'r') as f:
    valuesAp1 = [float(x) for x in f.readlines()]

# Lee los valores desde el archivo de texto de los valores del apartado 2
with open('datosAp2.txt', 'r') as fp:
    valuesAp2 = [float(x) for x in fp.readlines()]

# Lee los valores desde el archivo de texto de los valores del apartado 3
with open('datosAp3.txt', 'r') as fpd:
    valuesAp3 = [float(x) for x in fpd.readlines()]

# Lee los valores desde el archivo de texto de los valores del apartado 4
with open('datosAp4.txt', 'r') as fpd4:
    valuesAp4 = [float(x) for x in fpd4.readlines()]

# crear un array de arrays dividido en los grupos de 10 necesarios para la realizacion de
# las graficas, ergo groups[0] = {list: 10} [...], y asi hasta los 8 grupos necesarios, para los diferentes valores de N
groupsAp1 = [valuesAp1[i:i + 10] for i in range(0, len(valuesAp1), 10)]
groupsAp2 = [valuesAp2[i:i + 10] for i in range(0, len(valuesAp2), 10)]
groupsAp3 = [valuesAp3[i:i + 10] for i in range(0, len(valuesAp3), 10)]
groupsAp4 = [valuesAp4[i:i + 10] for i in range(0, len(valuesAp4), 10)]

# Crea una gráfica con múltiples líneas
fig, ax = plt.subplots()

#Ajustamos el tamaño de la figura
fig.set_size_inches(7, 5)

# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp1):
    if i == 0:
        ax.plot(range(1, 11), group, marker='o', markersize=5, color='gold',label = 'Apartado 1')
    else:
        ax.plot(range(1, 11), group, marker='o', markersize=5, color='gold')


# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp2):
    if i == 0:
        ax.plot(range(1, 11), group, marker='o', markersize=5, color='blueviolet', label='Apartado 2')
    else:
        ax.plot(range(1, 11), group, marker='o', markersize=5, color='blueviolet')


# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp3):
    if i == 0:
        ax.plot(range(1, 11), group, marker='o', markersize=5, color='mediumturquoise', label='Apartado 3')
    else:
        ax.plot(range(1, 11), group, marker='o', markersize=5, color='mediumturquoise')

# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp4):
    if i == 0:
        ax.plot(range(1, 11), group, marker='o', markersize=5, color='crimson', label='Apartado 4')
    else:
        ax.plot(range(1, 11), group, marker='o', markersize=5, color='crimson')

# Título de la gráfica
ax.set_title('Número de ciclos por cada tamalo de matriz')

# Nombre de los ejes x e y
ax.set_xlabel('Parámetros de N')
ax.set_ylabel('Número de ciclos')

# Añadimos la leyenda
ax.legend(loc = 'best')

# Guardamos la gráfica en un archivo png
plt.savefig('grafica.png')

print("Por favor, compruebe que se ha generado el archivo grafica.png en la siguiente lista: ", os.listdir())
