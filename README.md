# Containers (Qt6 C++ Project)

<img width="807" height="637" alt="image" src="https://github.com/user-attachments/assets/3b8f242e-5b8d-4ea5-8f6e-24dad148d11d" />


## Overview
**Containers** is a desktop application built with **Qt6 (C++)** for managing and visualizing containers and pallets.  
It follows clean software design principles and uses multiple **design patterns** such as:

- **Singleton** → Global managers (e.g. PalletManager).
- **Factory** → Creating containers dynamically.
- **Observer** → Updating UI when container data changes.
- **Memento** → Undo/restore previous states of containers.

The project is modular, easy to extend, and provides XML-based persistence.

---

## Features
- GUI built with **Qt Widgets**
- Manage pallets and containers
- Import/export **XML transaction logs**
- Undo/redo support with **Memento pattern**
- Follows **modern CMake** structure

---
