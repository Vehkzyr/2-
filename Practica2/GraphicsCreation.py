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
with open('Datos.txt', 'r') as f:
    values = [float(x) for x in f.readlines()]

# crear un array de arrays dividido en los grupos de 8 necesarios para la realizacion de
# las graficas, ergo groups[0] = {list: 8} [...], y asi hasta los 4 grupos necesarios, para los diferentes codigos
groups = [values[i:i + 8] for i in range(0, len(values), 8)]

# Crea una gráfica con múltiples líneas
fig, ax = plt.subplots()

#Ajustamos el tamaño de la figura
fig.set_size_inches(12, 8)

# Añade cada conjunto de datos como una nueva línea en la misma gráfica
for i, group in enumerate(groups):
    ax.plot(N_parameters, group, marker = 'o', markersize = 5, label = f'N = {N_parameters[i]}')

# Título de la gráfica
ax.set_title('Numero de Ciclos por cada tamaño de Matriz')

# Nombre de los ejes x e y
ax.set_xlabel('Parámetros de N')
ax.set_ylabel('Número de ciclos')

# Añadimos la leyenda
ax.legend(loc = 'best')

# Guardamos la gráfica en un archivo png
plt.savefig('grafica.png')

print("Por favor, compruebe que se ha generado el archivo grafica.png en la siguiente lista: ", os.listdir())