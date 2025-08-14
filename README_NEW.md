# KDTreeImageClustering

Hierarchical image clustering using a KD-Tree, served via a simple Node/Express UI.

## Prerequisites

- Windows 10/11
- Node.js (LTS 18+ recommended) and npm
- Optional: MinGW‑w64 g++ if you want to rebuild the C++ binaries

Prebuilt executables `project.exe` and `histogram.exe` are already present in `Project/`.

## Run the app (Windows)

1) Open PowerShell and go to the server folder:

```powershell
cd "C:\Users\HP\Desktop\KDTreeImageClustering\Project"
```

2) Install dependencies:

```powershell
npm install
```

3) Start the server:

```powershell
npm start
```

4) Open your browser at:

http://localhost:8000

5) In the web UI:

- Upload an image (stored in `Project/Images/`).
- Enter the number of results and click Search. Results render below.

## Optional: Rebuild C++ binaries

Only if you modify sources or your environment requires it. From `Project/`:

```powershell
# Build project.exe (KD-Tree clustering)
 g++ -std=gnu++17 -O2 -Wall project.cpp -o project.exe

# Build histogram.exe (histogram generator)
# If filesystem linking fails on old GCC, try adding -lstdc++fs
 g++ -std=gnu++17 -O2 -Wall histogram.cpp -o histogram.exe
```

Alternatively, if you have mingw32-make installed:

```powershell
mingw32-make.exe all
```

## How it works

- `histogram.exe` scans `Project/Images/` and writes color histograms to `output.json`.
- `project.exe` reads `output.json` and `input.json` (fields: `target`, `num`), clusters via KD‑Tree, and writes the `numbers` array to `input.json`.
- `server.js` wires it together and serves the static UI in `public/`.

## Troubleshooting

- Run npm from `Project/` (not repo root). The `package.json` is in `Project/`.
- g++: unrecognized `-std=c++20`
  - Use `-std=gnu++17` (already set in the makefile and commands above).
- g++: `KdTree.cpp: No such file or directory`
  - There is no `KdTree.cpp`; only `KdTree.hpp`. Build `project.cpp` directly.
- Filesystem linker errors with very old MinGW
  - Rebuild `histogram.exe` with: `-lstdc++fs`.
- Port 8000 busy
  - Edit `const PORT = 8000;` in `server.js` and restart.

## Layout

```
Project/
  histogram.cpp, project.cpp, KdTree.hpp, json.hpp, stb_image.h
  histogram.exe, project.exe
  output.json, input.json
  public/ (index.html, index.js)
  Images/
  server.js, package.json, makefile
```
