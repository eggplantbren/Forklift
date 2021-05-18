import apsw
import matplotlib.pyplot as plt
import numpy as np
import os

conn = apsw.Connection("output/forklift.db", flags=apsw.SQLITE_OPEN_READONLY)
db = conn.cursor()

os.system("rm output/potts*")

i = 0
plt.figure(figsize=(7, 7))
for row in db.execute("SELECT stripe_id, iteration, params\
                        FROM particles\
                        WHERE params IS NOT NULL;"):
    stripe, iteration, params = row
    num_bytes = len(params)
    pixels = np.frombuffer(params, dtype=np.int32)
    pixels = pixels.reshape((32, 32))

    plt.clf()
    plt.imshow(pixels, vmin=0, vmax=5)
    plt.title(f"Stripe {stripe}, iteration {iteration}.")
    fname = "output/potts" + "%0.6d" % (i + 1) + ".png"
    plt.savefig(fname)
    print(f"Saved {fname}.", flush=True)
    i += 1

os.system("ffmpeg -r 10 -i output/potts%06d.png -c:v libx264 -b:v 5M output/potts.mp4")

