'''
@auhor: Pablo Seijo
@date: 18/3/2023
@company: USC ETSE
@description: Este cdigo coge valores de las medias geometricas de una medicion de los ciclos por lineas de cache de un
txt, y siendo D el tamaño de salto en doubles (cada uno es 8 bytes) y nos devuelve un png con una grafica donde podemos
comparar los ciclos de las D.
'''

import matplotlib.pyplot as plt
import os

# Le paso los valores de L y de D para usarlos en las graficas posteriormente
D_parameters = [2, 32, 128, 1024, 8192]
L_parameters = [384, 1152, 10240, 15360, 40960, 81930, 163930]

# Lee los valores desde el archivo de texto
with open('menoresDatosDirecto.txt', 'r') as f:
    values = [float(x) for x in f.readlines()]

# crear un array de arrays dividido en los grupos de 7 necesarios para la realizacion de
# las graficas, ergo groups[0] = {list: 7} [...], y asi hasta los 5 grupos necesarios
groups = [values[i:i + 7] for i in range(0, len(values), 7)]

# Crea una gráfica con múltiples líneas
fig, ax = plt.subplots()

#Ajustamos el tamaño de la figura
fig.set_size_inches(7, 5)

# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groups):
    ax.plot(L_parameters, group, marker = 'o', markersize = 5, label = f'D = {D_parameters[i]}')

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
