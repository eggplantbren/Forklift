import apsw
import matplotlib.pyplot as plt
import numpy as np
import os

conn = apsw.Connection("output/forklift.db", flags=apsw.SQLITE_OPEN_READONLY)
db = conn.cursor()

os.system("rm output/atoms*")

i = 0
plt.figure(figsize=(7, 7))
for row in db.execute("SELECT stripe_id, iteration, params\
                        FROM particles\
                        WHERE params IS NOT NULL;"):
    stripe, iteration, params = row
    num_bytes = len(params)
    xs = np.frombuffer(params[0:num_bytes//2], dtype=np.float64)
    ys = np.frombuffer(params[num_bytes//2:],  dtype=np.float64)

    plt.clf()
    plt.plot(xs, ys, ".")
    plt.axis([0, 1, 0, 1])
    plt.title(f"Stripe {stripe}, iteration {iteration}.")
    fname = "output/atoms" + "%0.6d" % (i + 1) + ".png"
    plt.savefig(fname)
    print(f"Saved {fname}.", flush=True)
    i += 1

os.system("ffmpeg -r 10 -i output/atoms%06d.png -c:v libx264 -b:v 5M output/atoms.mp4")

