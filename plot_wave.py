import sys
import matplotlib.pyplot as plt

values = []
for line in sys.stdin:
    try:
        values.append(float(line))
    except:
        pass

plt.plot(range(len(values)), values)
plt.xlim(xmax=360)
plt.show()
