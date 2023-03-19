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

# Le paso los valores de L y de D para usarlos en las graficas posteriormente
D_parameters = [2, 32, 128, 1024, 8192]
L_parameters = [384, 1152, 10240, 15360, 40960, 81930, 163930]

# Lee los valores desde el archivo de texto de los valores medios de double
with open('menoresDatosDouble.txt', 'r') as f:
    valuesDouble = [float(x) for x in f.readlines()]

# Lee los valores desde el archivo de texto de los valores medios de int
with open('menoresDatosInt.txt', 'r') as fp:
    valuesInt = [float(x) for x in fp.readlines()]

# Lee los valores desde el archivo de texto de los valores medios de acceso directo
with open('menoresDatosDirecto.txt', 'r') as fpd:
    valuesDirectAccess = [float(x) for x in fpd.readlines()]

# crear un array de arrays dividido en los grupos de 7 necesarios para la realizacion de
# las graficas, ergo groups[0] = {list: 7} [...], y asi hasta los 5 grupos necesarios
groupsDouble = [valuesDouble[i:i + 7] for i in range(0, len(valuesDouble), 7)]
groupsInt = [valuesInt[i:i + 7] for i in range(0, len(valuesInt), 7)]
groupsDirectAccess = [valuesDirectAccess[i:i + 7] for i in range(0, len(valuesDirectAccess), 7)]

# Crea una gráfica con múltiples líneas
fig, ax = plt.subplots()

#Ajustamos el tamaño de la figura
fig.set_size_inches(7, 5)

# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsDouble):
    if i == 0:
        ax.plot(L_parameters, group, marker = 'o', markersize = 5, color='gold', label = 'Double')
    else:
        ax.plot(L_parameters, group, marker = 'o', markersize = 5, color='gold')


# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsInt):
    if i == 0:
        ax.plot(L_parameters, group, marker = 'o', markersize = 5, color='blueviolet', label = 'Int')
    else:
        ax.plot(L_parameters, group, marker = 'o', markersize = 5, color='blueviolet')


# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groupsDirectAccess):
    if i == 0:
        ax.plot(L_parameters, group, marker = 'o', markersize = 5, color='crimson', label = 'Acceso Directo')
    else:
        ax.plot(L_parameters, group, marker = 'o', markersize = 5, color='crimson')


# Título de la gráfica
ax.set_title('Número de ciclos por cada L y D')

# Nombre de los ejes x e y
ax.set_xlabel('Parámetros de L')
ax.set_ylabel('Número de ciclos')

# Añadimos la leyenda
ax.legend(loc = 'best')

# Guardamos la gráfica en un archivo png
plt.savefig('grafica.png')

print("Por favor, compruebe que se ha generado el archivo grafica.png en la siguiente lista: ", os.listdir())
