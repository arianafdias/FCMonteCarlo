import numpy as np



import matplotlib.pyplot as plt

with open('Teste.txt') as file:
    lst = []
    for line in file:
        lst.append([float(x) for x in line.split()])
        
col1 = [(x[0]) for x in lst]
#col2 = [(x[1]) for x in lst]



#x = [i*10**(-5) for i in range(len(col1))]
#print(sum(col1))
#col2 = [-(x*31.9613)/100000 for x in range(len(col1))] 
#print(col2)
#plt.plot(col1, col2, 'o', label = 'simulação')
#plt.plot(x,col2, label = 'analítico')
#plt.xlabel('x (m)',fontsize = 20)
#plt.ylabel(r'log$\dfrac{I}{I_0}$',fontsize = 20)
plt.plot(col1)
plt.xticks(fontsize = 20)
plt.yticks(fontsize = 20)
#plt.legend(fontsize = 20)
plt.show()


