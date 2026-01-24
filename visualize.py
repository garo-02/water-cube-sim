import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
import glob

# ----------------------------
# ADJUSTABLE PARAMETERS
# ----------------------------

# Folder where CSV frames live
FRAME_DIR = "output/frame_*.csv"

# Save video?
SAVE_MP4 = True
MP4_NAME = "water_cube.mp4"

# Frames per second
FPS = 30

# ----------------------------

# Load all frames
files = sorted(glob.glob(FRAME_DIR))
frames = [np.loadtxt(f, delimiter=",") for f in files]

N = frames[0].shape[0]

# Create coordinate grid in [0,1]
x = np.linspace(0, 1, N)
y = np.linspace(0, 1, N)
X, Y = np.meshgrid(x, y)

# Set up figure
fig = plt.figure()
ax = fig.add_subplot(projection="3d")

ax.set_xlim(0, 1)
ax.set_ylim(0, 1)
ax.set_zlim(0, 1)

ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Height")

# Initial surface
surf = ax.plot_surface(X, Y, frames[0],
                        cmap="Blues",
                        alpha=0.8)

def update(frame_idx):
    global surf
    surf.remove()  # remove previous surface
    surf = ax.plot_surface(X, Y, frames[frame_idx], cmap="Blues", alpha=0.8)
    return (surf,)


ani = animation.FuncAnimation(
    fig,
    update,
    frames=len(frames),
    interval=1000 // FPS
)

if SAVE_MP4:
    ani.save(MP4_NAME, fps=FPS)
else:
    plt.show()
