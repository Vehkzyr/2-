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

######################################################### VALORES -O0 ###################################################

# Lee los valores desde el archivo de texto de los valores del apartado 1
with open('resultados_apartado4_8hilos.txt', 'r') as f:
    valuesAp1 = [float(x) for x in f.readlines()]

# Lee los valores desde el archivo de texto de los valores del apartado 2
with open('resultados_apartado4_16hilos.txt', 'r') as fp:
    valuesAp2 = [float(x) for x in fp.readlines()]

# Lee los valores desde el archivo de texto de los valores del apartado 3
with open('resultados_apartado4_32hilos.txt', 'r') as fpd:
    valuesAp3 = [float(x) for x in fpd.readlines()]

# Lee los valores desde el archivo de texto de los valores del apartado 3
with open('resultados_apartado4_64hilos.txt', 'r') as fpd:
    valuesAp4 = [float(x) for x in fpd.readlines()]



# crear un array de arrays dividido en los grupos de 10 necesarios para la realizacion de
# las graficas, ergo groups[0] = {list: 10} [...], y asi hasta los 8 grupos necesarios, para los diferentes valores de N
groupsAp1 = [valuesAp1[i:i + 10] for i in range(0, len(valuesAp1), 10)]
groupsAp2 = [valuesAp2[i:i + 10] for i in range(0, len(valuesAp2), 10)]
groupsAp3 = [valuesAp3[i:i + 10] for i in range(0, len(valuesAp3), 10)]
groupsAp4 = [valuesAp4[i:i + 10] for i in range(0, len(valuesAp4), 10)]

# Calcula la mediana de cada grupo de valores
medians1 = [sorted(group)[5] for group in groupsAp1]
medians2 = [sorted(group)[5] for group in groupsAp2]
medians3 = [sorted(group)[5] for group in groupsAp3]
medians4 = [sorted(group)[5] for group in groupsAp4]

######################################################### VALORES -O2 ###################################################

# Lee los valores desde el archivo de texto de los valores del apartado 1
with open('resultados_apartado4_8hilosO2.txt', 'r') as f:
    valuesAp5 = [float(x) for x in f.readlines()]

# Lee los valores desde el archivo de texto de los valores del apartado 2
with open('resultados_apartado4_16hilosO2.txt', 'r') as fp:
    valuesAp6 = [float(x) for x in fp.readlines()]

# Lee los valores desde el archivo de texto de los valores del apartado 3
with open('resultados_apartado4_32hilosO2.txt', 'r') as fpd:
    valuesAp7 = [float(x) for x in fpd.readlines()]

# Lee los valores desde el archivo de texto de los valores del apartado 3
with open('resultados_apartado4_64hilosO2.txt', 'r') as fpd:
    valuesAp8 = [float(x) for x in fpd.readlines()]


# crear un array de arrays dividido en los grupos de 10 necesarios para la realizacion de
# las graficas, ergo groups[0] = {list: 10} [...], y asi hasta los 8 grupos necesarios, para los diferentes valores de N
groupsAp5 = [valuesAp5[i:i + 10] for i in range(0, len(valuesAp5), 10)]
groupsAp6 = [valuesAp6[i:i + 10] for i in range(0, len(valuesAp6), 10)]
groupsAp7 = [valuesAp7[i:i + 10] for i in range(0, len(valuesAp7), 10)]
groupsAp8 = [valuesAp8[i:i + 10] for i in range(0, len(valuesAp8), 10)]

# Calcula la mediana de cada grupo de valores
medians5 = [sorted(group)[5] for group in groupsAp5]
medians6 = [sorted(group)[5] for group in groupsAp6]
medians7 = [sorted(group)[5] for group in groupsAp7]
medians8 = [sorted(group)[5] for group in groupsAp8]

########################################################################################################################

# Crea una gráfica con múltiples líneas
fig, ax = plt.subplots()

# Ajustamos el tamaño de la figura
fig.set_size_inches(7, 6)

# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp1):
    if i == 0:
        ax.plot(N_parameters, medians1, marker='o', markersize=5, color='gold',label = '8 Hilos -O0')
    else:
        ax.plot(N_parameters, medians1, marker='o', markersize=5, color='gold')


# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp2):
    if i == 0:
        ax.plot(N_parameters, medians2, marker='o', markersize=5, color='blueviolet', label='16 Hilos -O0')
    else:
        ax.plot(N_parameters, medians2, marker='o', markersize=5, color='blueviolet')


# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp3):
    if i == 0:
        ax.plot(N_parameters, medians3, marker='o', markersize=5, color='crimson', label='32 hilos -O0')
    else:
        ax.plot(N_parameters, medians3, marker='o', markersize=5, color='crimson')

# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp4):
    if i == 0:
        ax.plot(N_parameters, medians4, marker='o', markersize=5, color='mediumturquoise', label='64 hilos -O0')
    else:
        ax.plot(N_parameters, medians4, marker='o', markersize=5, color='mediumturquoise')

# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp5):
    if i == 0:
        ax.plot(N_parameters, medians5, marker='x', markersize=7, color='gold',label = '8 Hilos -O2')
    else:
        ax.plot(N_parameters, medians5, marker='x', markersize=7, color='gold')


# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp6):
    if i == 0:
        ax.plot(N_parameters, medians6, marker='x', markersize=7, color='blueviolet', label='16 Hilos -O2')
    else:
        ax.plot(N_parameters, medians6, marker='x', markersize=7, color='blueviolet')


# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp7):
    if i == 0:
        ax.plot(N_parameters, medians7, marker='x', markersize=7, color='crimson', label='32 Hilos -O2')
    else:
        ax.plot(N_parameters, medians7, marker='x', markersize=7, color='crimson')

# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsAp8):
    if i == 0:
        ax.plot(N_parameters, medians8, marker='x', markersize=7, color='mediumturquoise', label='64 Hilos -O2')
    else:
        ax.plot(N_parameters, medians8, marker='x', markersize=7, color='mediumturquoise')


# Configuramos los ticks del eje y en notación científica
ax.ticklabel_format(style='sci', axis='y', scilimits=(0,0))

# Título de la gráfica
ax.set_title('Mediana de los ciclos de reloj para todas las N del Apartado 1 y 2')

# Nombre de los ejes x e y
ax.set_xlabel('Valor de N')
ax.set_ylabel('Ciclos de reloj (En notacion cientifica)')

# Añadimos la leyenda
ax.legend(loc = 'best')

# Guardamos la gráfica en un archivo png
plt.savefig('Grafica2-1.png')

print("Por favor, compruebe que se ha generado el archivo grafica.png en la siguiente lista: ", os.listdir())
