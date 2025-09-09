# Containers (Qt6 C++ Project)

<img width="809" height="640" alt="image" src="https://github.com/user-attachments/assets/3e549617-05e2-48fc-ae53-dda432b5f914" />
<img width="811" height="640" alt="image" src="https://github.com/user-attachments/assets/1f70d992-e434-4244-bbd4-38e17a0ef7bf" />

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
