import sys
import matplotlib.pyplot as plt

values = []
for line in sys.stdin:
    try:
        values.append(float(line) / 309.1 - 1)
    except:
        pass

plt.plot(range(len(values)), values)
plt.xlim(xmax=360)
plt.show()
