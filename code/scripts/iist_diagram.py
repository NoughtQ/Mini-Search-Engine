import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

def func(x, a, b):
    return a * x * np.log(x) + b

plt.rc("font", family="Noto Serif SC", size=13, weight="bold")


words = [100000, 200000, 400000, 600000, 800000, 880000]
time = [0.199, 0.370, 0.732, 1.159, 1.598, 1.688]
  

plt.figure(figsize=(15, 6))
plt.ticklabel_format(style='plain')
plt.title("Curve Chart of the Speed Tests for Inverted Index", weight="bold", size=25)
plt.xlabel("Number of Words", weight="bold", size=15)
plt.ylabel("Duration/s", weight="bold", size=15)
plt.grid(linewidth=0.7)
plt.xlim(0, 1000000)
plt.ylim(0, 2)

ax = plt.gca()
# ax.spines['left'].set_position(('axes', 0.5))
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)
ax.minorticks_on()
ax.grid(which='minor', color='gray', linestyle=':', linewidth=0.5)

# log_model = np.polyfit(words, time, 1) #, w=words * np.log(words)
# log_model_fn = np.poly1d(log_model)
# x_s = (0, 1000000)

popt, pcov = curve_fit(func, words, time)
xfit = np.linspace(0, 1000000)
yfit = func(xfit, *popt)

plt.plot(xfit, yfit, color='green')
plt.scatter(words, time, color='blue')

plt.savefig('../../template/images/iist_diagram.png')
plt.show()




 # type: ignore
