import apsw
import matplotlib.pyplot as plt
import numpy as np


def logsumexp(xs):
    top = np.max(xs)
    return np.log(np.sum(np.exp(xs - top))) + top


class Results:
    def __init__(self):
        self.conn = apsw.Connection("output/forklift.db",
                                    flags=apsw.SQLITE_OPEN_READONLY)
        self.db = self.conn.cursor()
        self.db2 = self.conn.cursor()
        self.load_scalars()

    def __del__(self):
        self.conn.close()

    def load_scalars(self):
        self.num_particles = self.db.execute("SELECT num_particles\
                                              FROM constants;")\
                                                .fetchone()[0]

        logws, xs, ys = [], [], []
        for row in self.db.execute("SELECT stripe_id, COUNT(*) FROM particles\
                                    GROUP BY stripe_id\
                                    ORDER BY stripe_id ASC;"):
            stripe_id = row[0]
            print(f"Processing stripe {stripe_id}...", end="", flush=True)
            data = self.db2.execute("SELECT iteration, x, y\
                                     FROM particles\
                                     WHERE stripe_id = ?;",
                                    (stripe_id, )).fetchall()
            data = np.array(data)
            stripe_lpm = np.log(1.0 - np.exp(-1.0)) - stripe_id
            lpms = -data[:,0]/self.num_particles
            lpms += stripe_lpm - logsumexp(lpms)
            logws = np.hstack([logws, lpms])
            xs = np.hstack([xs, data[:,1]])
            ys = np.hstack([ys, data[:,2]])

            print("done.", flush=True)

        self.logws, self.xs, self.ys = logws, xs, ys

    def plot_scalars(self):
        plt.plot(self.xs, self.ys, ".", markersize=1, alpha=0.3)
        plt.show()

    def compute_logz(self, temperatures):
        Tx, Ty = temperatures
        return logsumexp(self.logws + self.xs/Tx + self.ys/Ty)


results = Results()
logz = results.compute_logz((20.0, 1.0))
print(f"logz(20, 1) = {logz}. True value = -843.7673103203256.")


results.plot_scalars()

