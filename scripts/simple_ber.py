import numpy as np
import matplotlib.pyplot as plt
import random
import subprocess
from tqdm import tqdm

OUT = './build/plot.png'

BIN = './build/main'
NITER = 500
p = np.linspace(0, .1, 100);
ber = np.zeros(p.shape)

random.seed(42)

for i, pp in tqdm(list(enumerate(p))):
    acc_err = 0
    acc_n = 0
    for _ in range(NITER):
        args = [BIN, pp, random.randint(1, 10**6)]
        sargs = [str(a) for a in args]
        r = subprocess.run(sargs, capture_output=True)
        t = r.stdout.decode().strip()
        kv = {}
        try:
            for l in t.split('\n'):
                l = l.split('\t')
                kv[l[0]] = int(l[1])
        except Exception as e:
            print(args, sargs, t)
            break

        acc_err += kv['n_errors']
        acc_n += kv['n_bits_total']

    ber[i] = acc_err/acc_n

fig, ax = plt.subplots(dpi = 300)

ax.plot(p, ber)
ax.set_xlabel('Bit flip probability')
ax.set_ylabel('BER')

fig.tight_layout()
fig.savefig(OUT)
