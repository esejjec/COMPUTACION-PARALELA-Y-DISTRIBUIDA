import matplotlib.pyplot as plt
import numpy as np

x = [50,100,150,200,250,300,350,400,450,500]
y = [0.003222
	,0.009616
	,0.009616
	,0.052804
	,0.106328
	,0.163165
	,0.237373
	,0.343125
	,0.483996
	,0.800261]

x1 = [50,100,150,200,250,300,350,400,450,500]
y1 = [0.002842
	 ,0.029138
	 ,0.038924
	 ,0.069616
	 ,0.090603
	 ,0.155851
	 ,0.206351
	 , 0.284718
	 ,0.385019
	 ,0.721142]

x2 = [50,100,150,200,250,300,350,400,450,500]
y2 = [0.004142,0.02747,0.041794
		,0.058636,0.088519,0.140157
		,0.20327,0.269684,0.372575,0.511707]



plt.plot(x,y,color='g')
plt.plot(x1,y1,color='r')
plt.plot(x2,y2,color='b')
plt.xlabel('N DATOS')
plt.ylabel('TIEMPO')
plt.plot(x, y, color="g", linewidth=2.5, linestyle="-", label="Multiplicación Clasica")
plt.plot(x1, y1, color="r",  linewidth=2.5, linestyle="-", label="Multiplicación por Bloque")
plt.plot(x2, y2, color="b",  linewidth=2.5, linestyle="-", label="Multiplicación con 6 FOR")
plt.legend(loc='upper left')
plt.title('Multiplicación - Tres bucles')
plt.show()