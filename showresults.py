import apsw
import matplotlib.pyplot as plt
import numba
import numpy as np
import numpy.random as rng

@numba.njit("float64(float64[:])")
def logsumexp(xs):
    top = np.max(xs)
    return np.log(np.sum(np.exp(xs - top))) + top


MAX_LOAD_SIZE = 20000000

@numba.njit("float64[:,:,:](float64[:], float64[:], float64[:], float64[:], int64, boolean)")
def _canonical_grid(limits, xs, ys, logws, num, resid):
    """
    Compute canonical distributions on a grid
    """
    Tx = np.exp(np.linspace(np.log(limits[0]), np.log(limits[1]), num))
    Ty = np.exp(np.linspace(np.log(limits[2]), np.log(limits[3]), num))[::-1]
    logzs_infos = np.empty((4, num, num))
    for i in range(num):
        for j in range(num):
            logls = xs/Tx[j] + ys/Ty[i]
            logz = logsumexp(logws + logls)
            ps = np.exp(logws + logls - logz)
            info = np.sum(ps*(logls - logz))
            if resid:
                x = np.linspace(0.0, 1.0, 20001)
                y = np.exp(-0.5*1E4*(x - 0.5)**2/Tx[j] - 1E2*x/Ty[i])
                p = y/np.trapz(y, x=x)
                logz -= 20.0*np.log(np.trapz(y, x=x))
                info -= 20.0*np.trapz(p*np.log(p + 1E-300), x=x)
            logzs_infos[0, i, j] = logz
            logzs_infos[1, i, j] = info
            logzs_infos[2, i, j] = np.sum(ps*xs)
            logzs_infos[3, i, j] = np.sum(ps*ys)
        print("Finished row", str(i+1))
    return logzs_infos


class Results:
    def __init__(self):
        self.conn = apsw.Connection("output/forklift.db",
                                    flags=apsw.SQLITE_OPEN_READONLY)
        self.db = self.conn.cursor()
        self.db2 = self.conn.cursor()

        self.wconn = apsw.Connection("output/forklift.db")
        self.writer = self.wconn.cursor()

        self.load_scalars()

    def __del__(self):
        self.conn.close()
        self.wconn.close()

    def load_scalars(self):
        size = self.db.execute("SELECT COUNT(*) FROM particles;")\
                                .fetchone()[0]
        thin = 1.0
        if size > MAX_LOAD_SIZE:
            thin = MAX_LOAD_SIZE / size

        self.num_particles = self.db.execute("SELECT num_particles\
                                              FROM constants;")\
                                                .fetchone()[0]

        logws, xs, ys = [], [], []
        for row in self.db.execute("SELECT stripe_id, COUNT(*) FROM particles\
                                    GROUP BY stripe_id\
                                    ORDER BY stripe_id ASC;"):
            stripe_id = row[0]
            print(f"Processing stripe {stripe_id}...", end="", flush=True)
            data = []
            for row in self.db2.execute("SELECT iteration, x, y\
                                     FROM particles\
                                     WHERE stripe_id = ?;",
                                    (stripe_id, )):
                if thin >= 1.0 or rng.rand() <= thin:
                    data.append(row)
            data = np.array(data)
            stripe_lpm = np.log(1.0 - np.exp(-1.0)) - stripe_id
            lpms = -data[:,0]/self.num_particles

            self.writer.execute("BEGIN;")
            for i in range(data.shape[0]):
                self.writer.execute("UPDATE particles SET lpm = ?\
                                WHERE stripe_id = ?\
                                AND iteration = ?;",
               (lpms[i], stripe_id, int(data[i,0])))
            self.writer.execute("COMMIT;")


            lpms += stripe_lpm - logsumexp(lpms)
            logws = np.hstack([logws, lpms])
            xs = np.hstack([xs, data[:,1]])
            ys = np.hstack([ys, data[:,2]])

            print("done.", flush=True)

        self.logws, self.xs, self.ys = logws, xs, ys

    def plot_scalars(self):
        if len(self.xs) > 1E6:
            print("Skipping plot because there are too many points.")
            return

        plt.plot(self.xs, self.ys, ".", markersize=1, alpha=0.3)
        plt.show()

    def logz_and_info(self, temperatures):
        Tx, Ty = temperatures
        logls = self.xs/Tx + self.ys/Ty
        logz = logsumexp(self.logws + logls)
        info = np.sum(np.exp(self.logws + logls - logz)*(logls - logz))
        return dict(logz=logz, info=info)

    def canonical_grid(self, limits, num=31, resid=False):
        return _canonical_grid(np.array(limits),
                               self.xs, self.ys, self.logws, num, resid)


results = Results()
print(results.logz_and_info((20.0, 1.0)))

results.plot_scalars()

# Extent for canonical distributions
limits = [1.0, 100.0, 1.0, 100.0]
num = 51
resid = False
logzs_infos = results.canonical_grid(limits, num, resid)

# Extent for plotting
dx = (np.log10(limits[1]) - np.log10(limits[0]))/(num - 1)
dy = (np.log10(limits[3]) - np.log10(limits[2]))/(num - 1)
extent = np.log10(limits)
extent += np.array([-0.5*dx, 0.5*dx, -0.5*dy, 0.5*dy])
cmap = "coolwarm" if resid else "viridis"

plt.figure(figsize=(8, 8))
plt.subplot(2, 2, 1)
plt.imshow(logzs_infos[0, :, :], extent=extent, cmap=cmap)
plt.ylabel("$\\log_{10}\\left(T_y\\right)$")
plt.title("$\\log Z$")

plt.subplot(2, 2, 2)
plt.imshow(logzs_infos[1, :, :], extent=extent, cmap=cmap)
plt.title("$H$")

plt.subplot(2, 2, 3)
plt.imshow(logzs_infos[2, :, :], extent=extent)
plt.xlabel("$\\log_{10}\\left(T_x\\right)$")
plt.ylabel("$\\log_{10}\\left(T_y\\right)$")
plt.title("$<f>$")

plt.subplot(2, 2, 4)
plt.imshow(logzs_infos[3, :, :], extent=extent, cmap=cmap)
plt.xlabel("$\\log_{10}\\left(T_x\\right)$")
plt.title("$<g>$")

plt.show()

