import apsw
import matplotlib.pyplot as plt

conn = apsw.Connection("output/forklift.db", flags=apsw.SQLITE_OPEN_READONLY)
db = conn.cursor()

xs, ys = [], []
for row in db.execute("SELECT x, y FROM particles;"):
    xs.append(row[0])
    ys.append(row[1])


plt.plot(xs, ys, ".", alpha=0.3)
plt.show()
