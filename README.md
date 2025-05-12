# Coursework: Graphics Operations (Stage 2)

## Build and Run
### Linux
```bash
git clone git@github.com:matw0x/class-diagram-tool.git && cd class-diagram-tool && mkdir build && cd build && cmake .. && cmake --build . -j$(nproc) && ./ClassDiagramTool
```
---
### Windows (MinGW)
```powershell
git clone git@github.com:matw0x/class-diagram-tool.git && cd class-diagram-tool && mkdir build && cd build && cmake -G "MinGW Makefiles" .. && cmake --build . && ClassDiagramTool.exe
```