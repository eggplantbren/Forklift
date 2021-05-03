import apsw
import matplotlib.pyplot as plt
import numpy as np

conn = apsw.Connection("output/forklift.db", flags=apsw.SQLITE_OPEN_READONLY)
db = conn.cursor()

num_particles = db.execute("SELECT num_particles FROM constants;")\
                    .fetchone()[0]


particles_per_stripe = db.execute("SELECT COUNT(*) FROM particles\
                                   GROUP BY stripe_id;").fetchall()
particles_per_stripe = np.array(particles_per_stripe).flatten()

logws, xs, ys = [], [], []
for row in db.execute("SELECT stripe_id, iteration, x, y FROM particles;"):
    stripe_id, iteration, x, y = row
    xs.append(x)
    ys.append(y)
    lpm_stripe = np.log(1.0 - np.exp(-1.0)) - stripe_id
    logws.append(lpm_stripe - np.log(particles_per_stripe[stripe_id]))
xs = np.array(xs)
ys = np.array(ys)
logws = np.array(logws)

import dnest4.classic as dn4

plt.plot(logws)
plt.show()

logz = dn4.logsumexp(logws + xs/20.0 + ys)
print(f"logz(20, 1) = {logz}. True value = -843.7673103203256.")

plt.plot(xs, ys, ".", markersize=1, alpha=0.3)
plt.show()
