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
with open('resultados_apartado1-O3.txt', 'r') as f:
    values = [float(x) for x in f.readlines()]

# crear un array de arrays dividido en los grupos de 10 necesarios para la realizacion de
# las graficas, ergo groups[0] = {list: 10} [...], y asi hasta los 8 grupos necesarios, para los diferentes valores de N
groups = [values[i:i + 10] for i in range(0, len(values), 10)]

# Calcula la mediana de cada grupo de valores
medians = [sorted(group)[5] for group in groups]

# Crea una gráfica con los valores de la mediana en el eje y
fig, ax = plt.subplots()

# Ajustamos el tamaño de la figura
fig.set_size_inches(12, 8)

# Establecemos el estilo de línea a "None"
ax.plot(N_parameters, medians, color='crimson', marker='o', markersize = 5)

# Configuramos los ticks del eje y en notación científica
ax.ticklabel_format(style='sci', axis='y', scilimits=(0,0))

# Título de la gráfica
ax.set_title('Mediana de los tics de reloj para todas las N en la tercera ejecucion del apartado 1')

# Nombre de los ejes x e y
ax.set_xlabel('Valor de N')
ax.set_ylabel('Tics de reloj (En notacion cientifica)')

# Guardamos la gráfica en un archivo png
plt.savefig('GraficaApartado1-O3-mediana.png')

print("Por favor, compruebe que se ha generado el archivo GraficaApartado1-O0-mediana.png en la siguiente lista: ", os.listdir())
