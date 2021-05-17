import matplotlib.pyplot as plt
import numpy as np
#import twinpeaks2018 as tp

## Set up the fonts
#tp.plot_settings()

## Compute the true partition function
#limits = [1E-1, 1E2, 1E-1, 1E2]
#truth = tp.demo_truth.compute_truth(limits=limits, n=51)

## Plot the results
#plt.figure(1, figsize=(9, 6))
#plt.subplot(1, 2, 1)
#plt.imshow(truth["ln_Z"], origin="lower", extent=np.log10(limits))
#plt.xlabel("$\\log_{10}(T_1)$")
#plt.ylabel("$\\log_{10}(T_2)$")
#plt.title("$\\ln(Z)$")

#plt.subplot(1, 2, 2)
#plt.imshow(truth["H"], origin="lower", extent=np.log10(limits))
#plt.xlabel("$\\log_{10}(T_1)$")
#plt.title("$H$")

#plt.savefig("figures/demo_truth.pdf")
#print("Saved figures/demo_truth.pdf")

x = np.linspace(0.0, 1.0, 1001)
def s(x, T1, T2):
    s = -((x - 0.5)/0.1)**2/T1 - np.sin(10.0*np.pi*x)**2/T2
    return np.exp(s)

plt.figure(2, figsize=(7, 6))
plt.rcParams["font.size"] = 14
S = s(x, 10.0, 10.0)
S = S / np.trapz(S, x=x)
plt.plot(x, S, label="$(T_1, T_2) = (1, 1)$")
S = s(x, 10.0, 1.0)
S = S / np.trapz(S, x=x)
plt.plot(x, S, label="$(T_1, T_2) = (10, 1)$")
S = s(x, 1.0, 10.0)
S = S / np.trapz(S, x=x)
plt.plot(x, S, label="$(T_1, T_2) = (1, 10)$")
plt.xlabel("$x$")
plt.ylabel("$p(x; T_1, T_2)$")
plt.legend()
plt.savefig("figures/demo_canonical.pdf")
print("Saved figures/demo_canonical.pdf")

