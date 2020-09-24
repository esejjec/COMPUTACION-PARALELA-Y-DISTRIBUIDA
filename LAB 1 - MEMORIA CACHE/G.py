import matplotlib.pyplot as plt
import numpy as np

x = [100,200,300,400,500,600,700,800,900,1000]
y = [0.000352 #100
	,0.001161 #200
	,0.002449 #300
	,0.004305 #400
	,0.005118 #500
	,0.010385 #600
	,0.013503 #700
	,0.013717 #800
	,0.014689 #900
	,0.017446]

x1 = [100,200,300,400,500,600,700,800,900,1000]
y1 = [0.00043	#100
	 ,0.001107	#200
	 ,0.00254	#300
	 ,0.004553	#400
	 ,0.0063	#500
	 ,0.007013	#600
	 ,0.015533	#700
	 ,0.01935	#800
	 ,0.018184	#900
	 ,0.030918]

plt.plot(x,y,color='g')
plt.plot(x1,y1,color='r')
plt.xlabel('N DATOS')
plt.ylabel('TIEMPO')
plt.plot(x, y, color="g", linewidth=2.5, linestyle="-", label="FOR 1")
plt.plot(x1, y1, color="r",  linewidth=2.5, linestyle="-", label="FOR 2")
plt.legend(loc='upper left')
plt.title('FOR 1 - FOR 2')
plt.show()