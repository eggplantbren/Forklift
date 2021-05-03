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

    def logz_and_info(self, temperatures):
        Tx, Ty = temperatures
        logls = self.xs/Tx + self.ys/Ty
        logz = logsumexp(self.logws + logls)
        info = np.sum(np.exp(self.logws + logls - logz)*(logls - logz))
        return dict(logz=logz, info=info)


results = Results()
print(results.logz_and_info((20.0, 1.0)))


results.plot_scalars()

